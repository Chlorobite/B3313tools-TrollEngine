#include "Trolls/bhv/bhv_headers.h"

/**
 * Makes Bowser jump back on stage after falling
 */
void bowser_act_jump_onto_stage(void) {
    s32 onDynamicFloor;
    struct Surface *floor = o->oFloor;

    // Set dynamic floor check (Object platforms)
    if (floor != NULL && floor->flags & SURFACE_FLAG_DYNAMIC) {
        onDynamicFloor = TRUE;
    } else {
        onDynamicFloor = FALSE;
    }
    // Set status Jump
    o->oBowserStatus |= BOWSER_STATUS_BIG_JUMP;

    switch (o->oSubAction) {
        // Stops Bowser and makes him invisible
        case BOWSER_SUB_ACT_JUMP_ON_STAGE_IDLE:
            if (o->oTimer == 0) {
                o->oFaceAnglePitch = 0;
                o->oFaceAngleRoll = 0;
            } //? missing else
            o->oFaceAnglePitch += 0x800;
            o->oFaceAngleRoll += 0x800;
            if (!(o->oFaceAnglePitch & 0xFFFF)) {
                o->oSubAction++;
            }
            bowser_set_goal_invisible();
            break;

        // Start jump animation and make him visible after an animation frame
        case BOWSER_SUB_ACT_JUMP_ON_STAGE_START:
            cur_obj_init_animation_with_sound(BOWSER_ANIM_JUMP_START);
            if (cur_obj_check_anim_frame(11)) {
                o->oMoveAngleYaw = o->oBowserAngleToCenter;
                o->oVelY = 150.0f;
                o->oBowserTargetOpacity = 255;
                o->oBowserTimer = 0;
                o->oSubAction++;
            } else {
                bowser_set_goal_invisible();
            }
            break;

        // Approach him back on stage
        case BOWSER_SUB_ACT_JUMP_ON_STAGE_LAND:
            if (o->oPosY > o->oHomeY) {
                o->oDragStrength = 0.0f;
                if (o->oBowserDistToCenter < 2500.0f) {
                    if (absf(o->oFloorHeight - o->oHomeY) < 100.0f) {
                        approach_f32_signed(&o->oForwardVel, 0, -5.0f);
                    } else {
                        cur_obj_forward_vel_approach_upward(150.0f, 2.0f);
                    }
                } else {
                    cur_obj_forward_vel_approach_upward(150.0f, 2.0f);
                }
            }
            // Land on stage
            if (bowser_land()) {
                o->oDragStrength = 10.0f;
                o->oSubAction++;
                // Spawn shockwave (BitS only) if is not on a platform
                if (onDynamicFloor == FALSE) {
                    bowser_spawn_shockwave();
                // If is on a dynamic floor in BitS, then jump
                // because of the falling platform
                } else if (o->oBehParams2ndByte == BOWSER_BP_BITS) {
                    o->oAction = BOWSER_ACT_BIG_JUMP;
                }
                // If is on a dynamic floor in BitFS, then tilt platform
                if (o->oBehParams2ndByte == BOWSER_BP_BITFS) {
                    o->oAction = BOWSER_ACT_TILT_LAVA_PLATFORM;
                }
            }
            // Reset him back on stage if he still didn't landed yet
            // Post-JP made this check as a separate function
            // but we are JP :trol:
            if (o->oVelY < 0.0f && o->oPosY < o->oHomeY - 300.0f) {
                o->oPosZ = 0.0f, o->oPosX = o->oPosZ;
                o->oPosY = o->oHomeY + 2000.0f;
                o->oVelY = 0.0f;
            }
            break;

        // Bowser landed, so reset action after he's done jumping
        case BOWSER_SUB_ACT_JUMP_ON_STAGE_STOP:
            if (cur_obj_check_if_near_animation_end()) {
                o->oAction = BOWSER_ACT_DEFAULT;
                o->oBowserStatus &= ~BOWSER_STATUS_BIG_JUMP;
                cur_obj_extend_animation_if_at_end();
            }
            break;
    }
}
