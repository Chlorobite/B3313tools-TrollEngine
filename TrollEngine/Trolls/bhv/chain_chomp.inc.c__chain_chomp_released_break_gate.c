#include "bhv_headers.h"

void chain_chomp_released_break_gate(void) {
    if (!o->oChainChompHitGate) {
        // If hit wall, assume it's the gate and bounce off of it
        //! The wall may not be the gate
        if (o->oMoveFlags & OBJ_MOVE_HIT_WALL) {
            o->oChainChompHitGate = TRUE;
            o->oMoveAngleYaw = cur_obj_reflect_move_angle_off_wall();
            o->oForwardVel *= 0.4f;
        }
    }
    // Removed else to prevent gbj
    if (o->oMoveFlags & OBJ_MOVE_MASK_ON_GROUND || o->oPosY < o->oHomeY) {
        o->oChainChompHitGate = TRUE;
        o->oChainChompReleaseStatus = CHAIN_CHOMP_RELEASED_JUMP_AWAY;
        o->oHomeX = 3288.0f;
        o->oHomeZ = -1770.0f;
        o->oMoveAngleYaw = cur_obj_angle_to_home();
        o->oForwardVel = cur_obj_lateral_dist_to_home() / 50.0f;
        o->oVelY = 120.0f;
    }
}
