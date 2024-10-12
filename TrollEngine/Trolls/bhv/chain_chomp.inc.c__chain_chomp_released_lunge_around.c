#include "bhv_headers.h"

/**
 * Lunge 4 times, each time moving toward mario +/- 0x2000 angular units.
 * Finally, begin a lunge toward x=1450, z=562 (near the gate).
 */
void chain_chomp_released_lunge_around(void) {
    register struct Object *_o = o;
    chain_chomp_restore_normal_chain_lengths();

    // Finish bounce
    if (_o->oMoveFlags & OBJ_MOVE_MASK_ON_GROUND) {
        // Before first bounce, turn toward mario and wait 2 seconds
        if (_o->oChainChompNumLunges == 0) {
            if (cur_obj_rotate_yaw_toward(_o->oAngleToMario, 800)) {
                if (_o->oTimer > 60) {
                    _o->oChainChompNumLunges++;
                    // enable wall collision
                    _o->oWallHitboxRadius = 200.0f;
                }
            } else {
                _o->oTimer = 0;
                _o->oHomeX = 0.0f;
                _o->oHomeZ = 0.0f;
            }
        } else {
            if (++_o->oChainChompNumLunges <= 5) {
                cur_obj_play_sound_2(SOUND_GENERAL_CHAIN_CHOMP1);
                _o->oMoveAngleYaw = _o->oAngleToMario + random_sign() * 0x2000;
                _o->oForwardVel = 30.0f;
                _o->oVelY = 50.0f;
            } else {
                register f32 velY = 50.f;
                register f32 forwardVelMultiplier = 1.0f/8.0f;
                
                if (_o->oHomeX == 0 && _o->oHomeZ == 0) {
                    // No gate, just jump away.
                    _o->oChainChompHitGate = TRUE;
                    _o->oChainChompReleaseStatus = CHAIN_CHOMP_RELEASED_JUMP_AWAY;
                    _o->oHomeX = 3288.0f;
                    _o->oHomeZ = -1770.0f;
                    forwardVelMultiplier = 1.0f/50.0f;
                    velY = 120.0f;
                }
                else {
                    // Lunge at gate.
                    _o->oChainChompReleaseStatus = CHAIN_CHOMP_RELEASED_BREAK_GATE;
                }
                _o->oMoveAngleYaw = cur_obj_angle_to_home();
                _o->oForwardVel = cur_obj_lateral_dist_to_home() * forwardVelMultiplier;
                _o->oVelY = velY;
            }
        }
    }
} 
