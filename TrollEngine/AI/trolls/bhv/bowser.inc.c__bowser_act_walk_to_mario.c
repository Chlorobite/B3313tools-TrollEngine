#include "bhv_headers.h"


void bowser_act_walk_to_mario(void) {
    s16 turnSpeed = 0x400;
    s16 angleFromMario = abs_angle_diff(o->oMoveAngleYaw, o->oAngleToMario);

    // Set turning speed depending of the health
    // Also special case for BitFS
    if (o->oBehParams2ndByte != BOWSER_BP_BITFS) {
        // BOWSER_BP_BITDW or BOWSER_BP_BITS
        if (o->oHealth == 2) {
            turnSpeed = 0x300;
        } else if (o->oHealth == 1) {
            turnSpeed = 0x200;
        }
    }
    turnSpeed *= TRACKER_difficulty_modifier_half;
    if (turnSpeed > 0x800) {
        turnSpeed = 0x800;
    }

    cur_obj_rotate_yaw_toward(o->oAngleToMario, turnSpeed);

    if (o->oSubAction == 0) {
        o->oBowserTimer = 0;
        // Start walking
        if (bowser_set_anim_look_up_and_walk()) {
            if (o->oBehParams2ndByte == 3)
                o->oAction = BOWSER_ACT_DEFAULT;
            else
                o->oSubAction++;
        }
    } else if (o->oSubAction == 1) {
        // Keep walking slowly
        if (bowser_set_anim_slow_gait()) {
            o->oBowserTimer++;
            // Reset fire sky status
            if (o->oBowserStatus & BOWSER_STATUS_FIRE_SKY) {
                if (o->oBowserTimer > 4) {
                    o->oBowserStatus &= ~BOWSER_STATUS_FIRE_SKY;
                }
            // Do subaction below if angles is less than 0x2000
            } else if (angleFromMario < 0x2000) {
                o->oSubAction++;
            }
        }
    // Stop walking and set to default action
    } else if (bowser_set_anim_look_down_stop_walk()) {
        o->oAction = BOWSER_ACT_DEFAULT;
    }
}

