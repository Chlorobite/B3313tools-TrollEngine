#include "bhv_headers.h"


void bowser_act_charge_mario(void) {
    s32 time;
    // Reset Speed to prepare charge
    if (o->oTimer == 0) {
        o->oForwardVel = 0.0f;
    }

    switch (o->oSubAction) {
        case BOWSER_SUB_ACT_CHARGE_START:
            // Start running
            o->oBowserTimer = 0;
            if (cur_obj_init_animation_and_check_if_near_end(BOWSER_ANIM_RUN_START)) {
                o->oSubAction = BOWSER_SUB_ACT_CHARGE_RUN;
            }
            break;

        case BOWSER_SUB_ACT_CHARGE_RUN:
            // Set speed to run
            o->oForwardVel = 50.0f;
            if (cur_obj_init_animation_and_check_if_near_end(BOWSER_ANIM_RUN)) {
                o->oBowserTimer++;
                // Split if 6 timer frames has passed
                if (o->oBowserTimer >= 6) {
                    o->oSubAction = BOWSER_SUB_ACT_CHARGE_SLIP;
                }
                // Slip if Mario has a differentiable angle and 2 timer frames has passed
                if (o->oBowserTimer >= 2) {
                    if (abs_angle_diff(o->oAngleToMario, o->oMoveAngleYaw) > 0x2000) {
                        o->oSubAction = BOWSER_SUB_ACT_CHARGE_SLIP;
                    }
                }
            }
            // Rotate to Mario
            cur_obj_rotate_yaw_toward(o->oAngleToMario, 0x200);
            break;

        case BOWSER_SUB_ACT_CHARGE_SLIP:
            // Spawn smoke puff while slipping
            o->oBowserTimer = 0;
            cur_obj_init_animation_with_sound(BOWSER_ANIM_RUN_SLIP);
            spawn_object_relative_with_scale(0, 100, -50, 0, 3.0f, o, MODEL_SMOKE, bhvWhitePuffSmoke2);
            spawn_object_relative_with_scale(0, -100, -50, 0, 3.0f, o, MODEL_SMOKE,
                                             bhvWhitePuffSmoke2);
            // End Charge once Bowser stops running
            if (approach_f32_signed(&o->oForwardVel, 0, -1.0f * TRACKER_difficulty_modifier_half)) {
                o->oSubAction = BOWSER_SUB_ACT_CHARGE_END;
            }
            cur_obj_extend_animation_if_at_end();
            break;

        case BOWSER_SUB_ACT_CHARGE_END:
            // Stop running
            o->oForwardVel = 0.0f;
            cur_obj_init_animation_with_sound(BOWSER_ANIM_RUN_STOP);
            if (cur_obj_check_if_near_animation_end()) {
                // Set time delay to go to default action
                if (o->oBehParams2ndByte == BOWSER_BP_BITS) {
                    time = 10;
                } else {
                    time = 30;
                }
                time /= TRACKER_difficulty_modifier_half;
                if (o->oBowserTimer > time) {
                    o->oAction = BOWSER_ACT_DEFAULT;
                }
                o->oBowserTimer++;
            }
            cur_obj_extend_animation_if_at_end();
            break;
    }

    // Bowser is close to falling so set hit edge action
    if (o->oMoveFlags & OBJ_MOVE_HIT_EDGE) {
        o->oAction = BOWSER_ACT_HIT_EDGE;
    }
}

