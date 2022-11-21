#include "bhv_headers.h"
#include "AI/stats_tracking.h"

extern struct ObjectHitbox sWigglerHitbox;
extern u8 sWigglerAttackHandlers[];
extern f32 sWigglerSpeeds[];


void wiggler_act_walk(void) {
    s16 yawTurnSpeed;

    o->oWigglerWalkAnimSpeed = 0.06f * o->oForwardVel;

    // Update text if necessary
    if (o->oWigglerTextStatus < WIGGLER_TEXT_STATUS_COMPLETED_DIALOG) {
        if (o->oWigglerTextStatus == WIGGLER_TEXT_STATUS_AWAIT_DIALOG) {
            seq_player_lower_volume(SEQ_PLAYER_LEVEL, 60, 40);
            o->oWigglerTextStatus = WIGGLER_TEXT_STATUS_SHOWING_DIALOG;
        }

        // If Mario is positioned below the wiggler, assume he entered through the
        // lower cave entrance, so don't display text.
        if (gMarioObject->oPosY < o->oPosY || cur_obj_update_dialog_with_cutscene(
            MARIO_DIALOG_LOOK_UP, DIALOG_FLAG_NONE, CUTSCENE_DIALOG, DIALOG_150)) {
            o->oWigglerTextStatus = WIGGLER_TEXT_STATUS_COMPLETED_DIALOG;
        }
    } else {
        //! Every object's health is initially 2048, and wiggler's doesn't change
        //  to 4 until after this runs the first time. It indexes out of bounds
        //  and uses the value 113762.3 for one frame on US. This is fixed up
        //  in wiggler_init_segments if AVOID_UB is defined.
        obj_forward_vel_approach(sWigglerSpeeds[o->oHealth - 1] * TRACKER_difficulty_modifier_sqrt_half, 1.0f);

        if (o->oWigglerWalkAwayFromWallTimer != 0) {
            o->oWigglerWalkAwayFromWallTimer--;
        } else {
            if (o->oDistanceToMario >= 25000.0f) {
                // If >1200 away from home, turn to home
                o->oWigglerTargetYaw = o->oAngleToMario;
            }

            if (obj_bounce_off_walls_edges_objects(&o->oWigglerTargetYaw)) {
                //! If the wiggler could self-intersect, or intersect a different
                //  non-mario object, this could potentially be used to force
                //  the wiggler to walk straight - past his usual radius
                o->oWigglerWalkAwayFromWallTimer = random_linear_offset(30, 30);
            } else {
                if (o->oHealth < 4) {
                    o->oWigglerTargetYaw = o->oAngleToMario;
                } else if (o->oWigglerTimeUntilRandomTurn != 0) {
                    o->oWigglerTimeUntilRandomTurn--;
                } else {
                    o->oWigglerTargetYaw = o->oMoveAngleYaw + 0x4000 * (s16) random_sign();
                    o->oWigglerTimeUntilRandomTurn = random_linear_offset(30, 50);
                }
            }
        }

        // If moving at high speeds, could overflow. But can't reach such speeds
        // in practice
        yawTurnSpeed = (s16)(30.0f * o->oForwardVel);
        cur_obj_rotate_yaw_toward(o->oWigglerTargetYaw, yawTurnSpeed);
        obj_face_yaw_approach(o->oMoveAngleYaw, 2 * yawTurnSpeed);

        obj_face_pitch_approach(0, 0x320);

        // For the first two seconds of walking, stay invulnerable
        if (o->oTimer < 60) {
            obj_check_attacks(&sWigglerHitbox, o->oAction);
        } else if (obj_handle_attacks(&sWigglerHitbox, o->oAction, sWigglerAttackHandlers)) {
            if (o->oAction != WIGGLER_ACT_JUMPED_ON) {
                o->oAction = WIGGLER_ACT_KNOCKBACK;
            }

            o->oWigglerWalkAwayFromWallTimer = 0;
            o->oWigglerWalkAnimSpeed = 0.0f;
        }
    }
}

