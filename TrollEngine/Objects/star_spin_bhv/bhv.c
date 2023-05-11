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

#define CELEB_STAR_ACT_SPIN_AROUND_MARIO 0
#define CELEB_STAR_ACT_FACE_CAMERA       1

// celebration_star.c.inc

void bhv_celebration_star_init_true(void) {
    o->oHomeX = gMarioObject->header.gfx.pos[0];
    o->oPosY = gMarioObject->header.gfx.pos[1] + 30.0f;
    o->oHomeZ = gMarioObject->header.gfx.pos[2];
    o->oMoveAngleYaw = gMarioObject->header.gfx.angle[1] + 0x8000;
    o->oCelebStarDiameterOfRotation = 100;
    if (gCurrLevelNum == LEVEL_BOWSER_1 || gCurrLevelNum == LEVEL_BOWSER_2) {
        o->header.gfx.sharedChild = gLoadedGraphNodes[MODEL_BOWSER_KEY];
        o->oFaceAnglePitch = 0;
        o->oFaceAngleRoll = 49152;
        cur_obj_scale(0.1f);
        o->oCelebStarUnkF4 = 1;
    } else {
        o->header.gfx.sharedChild = gLoadedGraphNodes[MODEL_STAR];
        o->oFaceAnglePitch = 0;
        o->oFaceAngleRoll = 0;
        cur_obj_scale(0.4f);
        o->oCelebStarUnkF4 = 0;
    }
    o->header.gfx.sharedChild = gLoadedGraphNodes[MODEL_STAR];
    cur_obj_scale(0.4f);
    o->oFaceAnglePitch = 0;
    o->oFaceAngleRoll = 0;
}

void celeb_star_act_spin_around_mario_true(void) {
    o->oPosX = o->oHomeX + sins(o->oMoveAngleYaw) * (f32)(o->oCelebStarDiameterOfRotation / 2);
    o->oPosZ = o->oHomeZ + coss(o->oMoveAngleYaw) * (f32)(o->oCelebStarDiameterOfRotation / 2);
    o->oPosY += 5.0f;
    //o->oFaceAngleYaw += 0x1000;
    o->oAnimState++;
    o->oMoveAngleYaw += 0x2000;

    if (o->oTimer == 40)
        o->oAction = CELEB_STAR_ACT_FACE_CAMERA;
    if (o->oTimer < 35) {
        spawn_object(o, MODEL_SPARKLES, bhvCelebrationStarSparkle);
        o->oCelebStarDiameterOfRotation++;
    } else
        o->oCelebStarDiameterOfRotation -= 20;
}

void celeb_star_act_face_camera_true(void) {

    if (o->oTimer < 7) {
        if (o->oCelebStarUnkF4 == 0) {
            cur_obj_scale((f32) o->oTimer / 6.0);
        } else {
            cur_obj_scale((f32) o->oTimer / 10.0);
        }
        cur_obj_scale((f32) o->oTimer / 6.0);
        //o->oFaceAngleYaw += 0x1000;
        o->oAnimState++;
    } else {
        //o->oFaceAngleYaw = gMarioObject->header.gfx.angle[1];
        o->oAnimState = 0;
    }

    if (o->oTimer == 59)
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
}

void bhv_celebration_star_loop(void) {
    switch (o->oAction) {
        case CELEB_STAR_ACT_SPIN_AROUND_MARIO:
            celeb_star_act_spin_around_mario_true();
            break;

        case CELEB_STAR_ACT_FACE_CAMERA:
            celeb_star_act_face_camera_true();
            break;
    }
}