#include <PR/ultratypes.h>

#include "sm64.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "game_init.h"
#include "geo_misc.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_table.h"
#include "level_update.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "obj_behaviors_2.h"
#include "object_constants.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "platform_displacement.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "seq_ids.h"
#include "spawn_sound.h"

#define o gCurrentObject

enum oBlooperAction {
    ACT_B_SWIM_UP,
    ACT_B_IDLE,
    ACT_B_FALL_DOWN
};

static struct ObjectHitbox sBlooperHitbox = {
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 90,
    /* height:            */ 90,
    /* hurtboxRadius:     */ 70,
    /* hurtboxHeight:     */ 80,
};

void bhv_blooper_init(void) {
    obj_set_hitbox(o, &sBlooperHitbox);
}

void bhv_blooper_idle(void) {
    o->oForwardVel = 0.0f;
    if (o->oDistanceToMario < 2500.0f) {
        o->oForwardVel = 8.3f;
        if (o->oPosY < gMarioState->pos[1] - 90)
        o->oAction = ACT_B_SWIM_UP;
        if (o->oPosY > gMarioState->pos[1] + 60)
        o->oPosY -= 15.0f;
    }
}

void bhv_blooper_swim_up(void) {
    o->oForwardVel = 14.0f;
    if (o->oTimer == 2)
        cur_obj_play_sound_2(SOUND_GENERAL_SWISH_WATER);
    if (o->header.gfx.scale[0] > 0.80f)
        o->header.gfx.scale[0] = o->header.gfx.scale[2] = o->header.gfx.scale[0] - 0.06f;
    else
        o->header.gfx.scale[0] = o->header.gfx.scale[2] = 0.80f;
    if (o->oTimer < 30 && o->oPosY < find_water_level(o->oPosX, o->oPosZ) - 250) {
        o->oPosY = o->oPosY + (30 - o->oTimer);
        if(o->oPosY > gMarioState->pos[1] && o->oTimer < 30)
            o->oTimer = 30;
    } else {
        o->oAction = ACT_B_FALL_DOWN;
    }
}

void bhv_blooper_fall(void) {
    o->oForwardVel = 0.0f;
    if ((o->oPosY > gMarioState->pos[1] - 250.0f && o->oPosY > o->oFloorHeight + 50) || o->oTimer < 30) {
        if (o->oTimer < 10)
            o->oPosY -= o->oTimer;
        else
            o->oPosY -= 15.0f;
    } else {
        o->oAction = ACT_B_IDLE;
    }
}

void bhv_blooper_loop(void) {
    f32 ceilHeight;
    struct Surface *surface;

    cur_obj_rotate_yaw_toward(o->oAngleToMario, 0x400);
    
    if (obj_update_standard_actions(1.0f)) {
        o->oMoveAngleYaw = approach_s16_asymptotic(o->oMoveAngleYaw, o->oAngleToMario, 0x400);

        switch (o->oAction) {
        case ACT_B_IDLE:
            bhv_blooper_idle();
            break;
        case ACT_B_SWIM_UP:
            bhv_blooper_swim_up();
            break;
        case ACT_B_FALL_DOWN:
            bhv_blooper_fall();
            break;
        }

    o->oVelX = o->oForwardVel * sins(o->oMoveAngleYaw);
    o->oVelZ = o->oForwardVel * coss(o->oMoveAngleYaw);

    o->oPosX += o->oVelX;
    o->oPosZ += o->oVelZ;

        if (o->oAction != ACT_B_SWIM_UP) {
            if (o->header.gfx.scale[0] < 1.0f)
                o->header.gfx.scale[0] = o->header.gfx.scale[2] = o->header.gfx.scale[0] + 0.025f;
            else
                o->header.gfx.scale[0] = o->header.gfx.scale[2] = 1.0f;
        }
        
        o->oFloorHeight = find_floor_height(o->oPosX, o->oPosY, o->oPosZ);
        if (o->oPosY < o->oFloorHeight + 50) {
            o->oPosY = o->oFloorHeight + 50;
        }


        ceilHeight = find_ceil(o->oPosX, o->oPosY, o->oPosZ, &surface);
        if (o->oPosY > ceilHeight - 250)
            o->oPosY = ceilHeight - 250;
            
        o->oInteractStatus = 0;
    }
}
