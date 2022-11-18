#include "bhv_headers.h"


void bowser_act_hit_mine(void) {
    // Similar vel values from bowser_fly_back_dead
    if (o->oTimer == 0) {
        o->oForwardVel = -400.0f;
        o->oVelY = 100.0f;
        o->oMoveAngleYaw = o->oBowserAngleToCenter + 0x8000;
        o->oBowserEyesShut = TRUE; // close eyes
    }
    // Play flip animation
    if (o->oSubAction == BOWSER_SUB_ACT_HIT_MINE_START) {
        cur_obj_init_animation_with_sound(BOWSER_ANIM_FLIP);
        o->oSubAction++;
        o->oBowserTimer = 0;
    // Play flip animation again, extend it and play bounce effects
    } else if (o->oSubAction == BOWSER_SUB_ACT_HIT_MINE_FALL) {
        cur_obj_init_animation_with_sound(BOWSER_ANIM_FLIP);
        cur_obj_extend_animation_if_at_end();
        bowser_bounce_effects(&o->oBowserTimer);
        // Reset vel and stand up
        if (o->oBowserTimer > 2) {
            cur_obj_init_animation_with_sound(BOWSER_ANIM_STAND_UP_FROM_FLIP);
            o->oVelY = 0.0f;
            o->oForwardVel = 0.0f;
            o->oSubAction++;
        }
    // Play these actions once he is stand up
    } else if (o->oSubAction == BOWSER_SUB_ACT_HIT_MINE_STOP) {
        if (cur_obj_check_if_near_animation_end()) {
            o->oAction = BOWSER_ACT_DEFAULT;
            o->oBowserEyesShut = FALSE; // open eyes
        }
    }
}

