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

#define POS_OP_SAVE_POSITION 0
#define POS_OP_COMPUTE_VELOCITY 1
#define POS_OP_RESTORE_POSITION 2

#define o gCurrentObject


extern struct Surface *sObjFloor;


enum sMotosActions {
    MOTOS_ACT_WAIT,
    MOTOS_ACT_PLAYER_SEARCH,
    MOTOS_ACT_PLAYER_CARRY,
    MOTOS_ACT_PLAYER_PITCH,
    MOTOS_ACT_CARRY_RUN,
    MOTOS_ACT_THROWN,
    MOTOS_ACT_RECOVER,
    MOTOS_ACT_DEATH,
    MOTOS_ACT_FIREBALL
};

enum sMotosAnimations {
    MOTOS_ANIM_BASE,
    MOTOS_ANIM_CARRY,
    MOTOS_ANIM_CARRY_RUN,
    MOTOS_ANIM_CARRY_START,
    MOTOS_ANIM_DOWN_RECOVER,
    MOTOS_ANIM_DOWN_STOP,
    MOTOS_ANIM_PITCH,
    MOTOS_ANIM_SAFE_DOWN,
    MOTOS_ANIM_WAIT,
    MOTOS_ANIM_WALK,
    MOTOS_ANIM_END
};

// All changes made to the original code have been commented.

// Modified, original did not make sense, it checked if Mario was close and in front, but he is always close and in front when being held
// Now it goes off either a timer or when reached an edge
s32 bhv_motos_do_throw_mario() {
    return (o->oTimer > 45) || (o->oMoveFlags & OBJ_MOVE_HIT_EDGE);
    // Original:
    // return (cur_obj_rotate_yaw_toward(o->oAngleToMario, 0x200)) || (o->oDistanceToMario < 500.f)
}

void bhv_motos_hand_loop(void) {
    register struct Object *_o = o;

    s32 boss = _o->parentObj->oBehParams2ndByte == 1;

    _o->oParentRelativePosY = -30.f;
    _o->oParentRelativePosX = -70.f; // Changed relative pos to make Mario fit in the hand better

    common_anchor_mario_behavior(50.f, boss ? -75.f : 30.f, 64); // Used common func instead of repeating code
    // Also vert speed increased from 0 to 50
    
    if (_o->parentObj->oChuckyaUnk88 == 1) {
        vec3f_copy(gMarioState->pos, gMarioObject->header.gfx.pos); // Added to fix camera
        _o->oTimer = 0;
    }
    else if (boss) {
        if (_o->parentObj->oChuckyaUnk88 == 4) {
            cur_obj_play_sound_2(SOUND_OBJ_FLAME_BLOWN);
            obj_spit_fire(0, 0, 0, 5.0f, MODEL_RED_FLAME_SHADOW, 30.0f, 25.0f, 0x1000);
            _o->parentObj->oChuckyaUnk88 = 0;
        }
        
        if (_o->oTimer < 10)
            gMarioState->peakHeight = gMarioState->pos[1] + 3000.f; // fall damage
    }
}

void bhv_motos_wait(void) {
    register struct Object *_o = o;
    s32 boss = _o->oBehParams2ndByte == 1;

    _o->oForwardVel = 0.f;
    _o->oVelY = 0.f;
    
    cur_obj_init_animation_with_sound(MOTOS_ANIM_WAIT);
    
    if (boss) {
        if (_o->oDistanceToMario < 1500.f && (gMarioState->pos[1] == _o->oPosY || _o->oBehParams & 0xFF)) {
            _o->oAction = MOTOS_ACT_PLAYER_SEARCH;
            _o->oTimer = 0;
            if (!(_o->oBehParams & 0xFF)) {
                play_music(SEQ_PLAYER_LEVEL, SEQUENCE_ARGS(4, SEQ_EVENT_BOSS), 0);
                _o->oBehParams |= 1;
            }
        }
    }
    else if (_o->oDistanceToMario < 500.f) {
        _o->oAction = MOTOS_ACT_PLAYER_SEARCH;
        _o->oTimer = 0;
    }
}

void bhv_motos_player_search(void) {
    register struct Object *_o = o;
    s32 boss = _o->oBehParams2ndByte == 1;

    cur_obj_init_animation_with_sound(MOTOS_ANIM_WALK);
    _o->oForwardVel = boss ? 3.f : 5.f; // Sped up (was 2.f)
    cur_obj_rotate_yaw_toward(_o->oAngleToMario, boss ? 700 : 500); // Sped up (was 300)
    
    if (boss && _o->oDistanceToMario > 400.f && _o->oTimer > 40 && !(random_u16() & 0xF)) {
        _o->oTimer = 0;
        _o->oAction = MOTOS_ACT_FIREBALL;
    }
    if (_o->oInteractStatus & INT_STATUS_GRABBED_MARIO) {
        _o->oAction = MOTOS_ACT_PLAYER_CARRY;
        _o->oChuckyaUnk88 = 1;
    }
}

void bhv_motos_player_carry(void) {
    // Replaced with the similar MOTOS_ACT_CARRY_START, which allows for running
    cur_obj_init_animation_with_sound(MOTOS_ANIM_CARRY_START);
    
    if (cur_obj_check_if_near_animation_end()) {
        if (bhv_motos_do_throw_mario())
            o->oAction = MOTOS_ACT_PLAYER_PITCH;
        else
            o->oAction = MOTOS_ACT_CARRY_RUN;
    }
}

void bhv_motos_player_pitch(void) {
    register struct Object *_o = o;
    _o->oForwardVel = 0.f;
    cur_obj_init_animation_with_sound(MOTOS_ANIM_PITCH);
    
    if (cur_obj_check_anim_frame(14)) {
        _o->oChuckyaUnk88 = 2;
        _o->numCollidedObjs = 10;
    }
    
    if (cur_obj_check_if_near_animation_end()) {
        _o->oAction = MOTOS_ACT_WAIT;
        _o->oInteractStatus &= (~INT_STATUS_GRABBED_MARIO);
    }
}

void bhv_motos_carry_run(void) {
    o->oForwardVel = 15.f; // Sped up (was 5.f)
    cur_obj_init_animation_with_sound(MOTOS_ANIM_CARRY_RUN);
    
    if (bhv_motos_do_throw_mario())
        o->oAction = MOTOS_ACT_PLAYER_PITCH;
    // Useless else clause removed
}

void bhv_motos_thrown(void) {
    cur_obj_init_animation_with_sound(MOTOS_ANIM_WALK);
    
    if (o->oMoveFlags & OBJ_MOVE_LANDED) {
        o->oAction = MOTOS_ACT_RECOVER; // New action: recover (used to go straight back into wait)
        cur_obj_play_sound_2(SOUND_OBJ2_LARGE_BULLY_ATTACKED);
    }
}

void bhv_motos_recover(void) {
    register struct Object *_o = o;

    // These animations were originally unused. This plays the falling over animation and then he gets up.
    _o->oForwardVel = 0.f;
    if (_o->oSubAction == 0) {
        cur_obj_init_animation_with_sound(MOTOS_ANIM_DOWN_STOP);
        cur_obj_extend_animation_if_at_end();
        if (_o->oTimer > 35)
            _o->oSubAction++;
        
    } else /*if (_o->oSubAction == 1)*/ {
        cur_obj_init_animation_with_sound(MOTOS_ANIM_DOWN_RECOVER);
        if (cur_obj_check_if_near_animation_end())
            _o->oAction = MOTOS_ACT_WAIT;
    }
}

void bhv_motos_death(void) {
    register struct Object *_o = o;
    s32 boss = _o->oBehParams2ndByte == 1;
    
    // Taken from bully code to handle death
    if (obj_lava_death()) {
        if (boss) {
            // default dance
            spawn_default_star(130.0f, 1600.0f, -4335.0f);
        }
        else {
            struct Object *coin = spawn_object(_o, MODEL_BLUE_COIN, bhvBlueCoinMotos);
            cur_obj_play_sound_2(SOUND_GENERAL_COIN_SPURT);
            coin->oForwardVel = 10.0f;
            coin->oVelY = 100.0f;
            coin->oPosY = _o->oPosY + 310.0f;
            coin->oMoveAngleYaw = (f32)(_o->oFaceAngleYaw + 0x8000) + random_float() * 1024.0f;
        }
    }
}

void bhv_motos_fireball(void) {
    register struct Object *_o = o;
    _o->oForwardVel = 0.f;
    cur_obj_init_animation_with_sound(MOTOS_ANIM_PITCH);
    
    if (cur_obj_check_anim_frame(14)) {
        _o->oChuckyaUnk88 = 4;
    }
    
    if (cur_obj_check_if_near_animation_end()) {
        _o->oAction = MOTOS_ACT_WAIT;
        _o->oTimer = 0;
    }
}

void bhv_motos_main() {
    register struct Object *_o = o;

    f32 floorY;
    s32 boss = _o->oBehParams2ndByte == 1;

    cur_obj_update_floor_and_walls();

    switch (_o->oAction) {
        case MOTOS_ACT_WAIT:
            bhv_motos_wait();
            break;
        case MOTOS_ACT_PLAYER_SEARCH:
            bhv_motos_player_search();
            break;
        case MOTOS_ACT_PLAYER_CARRY:
            bhv_motos_player_carry();
            break;
        case MOTOS_ACT_PLAYER_PITCH:
            bhv_motos_player_pitch();
            break;
        case MOTOS_ACT_CARRY_RUN:
            bhv_motos_carry_run();
            break;
        case MOTOS_ACT_THROWN:
            bhv_motos_thrown();
            break;
        case MOTOS_ACT_RECOVER:
            bhv_motos_recover();
            break;
        case MOTOS_ACT_FIREBALL:
            bhv_motos_fireball();
            break;
    }
    
    cur_obj_move_standard(-78);
    
    // Added to detect lava death
    floorY = find_floor(_o->oPosX, _o->oPosY, _o->oPosZ, &sObjFloor);
    if (sObjFloor != NULL) {
        if ((floorY + 1.f > _o->oPosY) && (sObjFloor->type == SURFACE_BURNING || sObjFloor->type == SURFACE_INSTANT_QUICKSAND)) {
            _o->oAction = MOTOS_ACT_DEATH;
        }
    }
}

void bhv_motos_loop(void) {
    register struct Object *_o = o;

    s32 boss = _o->oBehParams2ndByte == 1;
    cur_obj_scale(boss ? 3.f : 2.f);
    _o->oInteractionSubtype |= INT_SUBTYPE_GRABS_MARIO;
    
    switch (_o->oHeldState) {
        case HELD_FREE:
            // If clause to prevent Motos from updating physics while dying from lava
            if (_o->oAction == MOTOS_ACT_DEATH)
                bhv_motos_death();
            else
                bhv_motos_main();
            break;
        case HELD_HELD:
            cur_obj_become_intangible();
            cur_obj_disable_rendering();
            cur_obj_init_animation_with_sound(MOTOS_ANIM_WALK);
            _o->oAction = MOTOS_ACT_WAIT;
            break;
        case HELD_THROWN:
        case HELD_DROPPED:
            cur_obj_get_thrown_or_placed(15.f, 35.f, MOTOS_ACT_THROWN);
            break;
    }
    
    _o->oInteractStatus = 0; // Added because it doesnt work without this, cmon Nintendo
    set_object_visibility(_o, boss ? 8000 : 3000); // Increased from 2000
}
