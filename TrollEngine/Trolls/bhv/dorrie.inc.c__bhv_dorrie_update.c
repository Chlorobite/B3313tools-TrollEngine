#include "bhv_headers.h"


void bhv_dorrie_update(void) {
    f32 boundsShift;
    f32 maxOffsetY;

    if (!(o->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM)) {
        o->oDorrieForwardDistToMario = o->oDistanceToMario * coss(o->oAngleToMario - o->oMoveAngleYaw);

        obj_perform_position_op(0);
        cur_obj_move_using_fvel_and_gravity();

        o->oDorrieAngleToHome = cur_obj_angle_to_home();
        o->oDorrieDistToHome = cur_obj_lateral_dist_to_home();

        // Shift dorrie's bounds to account for her neck
        boundsShift =
            440.0f * coss(o->oDorrieNeckAngle) * coss(o->oMoveAngleYaw - o->oDorrieAngleToHome);

        if (clamp_f32(&o->oDorrieDistToHome, 1650.0f * levelScaleH + boundsShift, 2300.0f * levelScaleH + boundsShift)) {
            o->oPosX = o->oHomeX - o->oDorrieDistToHome * sins(o->oDorrieAngleToHome);
            o->oPosZ = o->oHomeZ - o->oDorrieDistToHome * coss(o->oDorrieAngleToHome);
        }

        o->oDorrieGroundPounded = cur_obj_is_mario_ground_pounding_platform();

        if (gMarioObject->platform == o) {
            maxOffsetY = -17.0f;
            if (o->oDorrieOffsetY >= 0.0f) {
                if (o->oDorrieGroundPounded) {
                    o->oDorrieVelY = -15.0f;
                } else {
                    o->oDorrieVelY = -6.0f;
                }
            }
        } else {
            maxOffsetY = 0.0f;
        }

        o->oDorrieOffsetY += o->oDorrieVelY;
        approach_f32_ptr(&o->oDorrieVelY, 3.0f, 1.0f);
        if (o->oDorrieVelY > 0.0f && o->oDorrieOffsetY > maxOffsetY) {
            o->oDorrieOffsetY = maxOffsetY;
        }

        o->oPosY = o->oHomeY + o->oDorrieOffsetY;

        switch (o->oAction) {
            case DORRIE_ACT_MOVE:
                dorrie_act_move();
                break;
            case DORRIE_ACT_LOWER_HEAD:
                dorrie_act_lower_head();
                break;
            case DORRIE_ACT_RAISE_HEAD:
                dorrie_act_raise_head();
                break;
        }

        obj_perform_position_op(1);
    }
}

