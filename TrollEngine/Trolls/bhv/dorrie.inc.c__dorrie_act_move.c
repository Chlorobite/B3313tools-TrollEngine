#include "bhv_headers.h"


void dorrie_act_move(void) {
    s16 startYaw = o->oMoveAngleYaw;
    s16 targetYaw;
    s16 targetSpeed;

    o->oDorrieNeckAngle = -0x26F4;
    cur_obj_init_animation_with_sound(1);

    if (o->oDorrieForwardDistToMario < 320.0f && o->oDorrieGroundPounded) {
        cur_obj_play_sound_2(SOUND_OBJ_DORRIE);
        o->collisionData = segmented_to_virtual((void*)0x0600FBB8);
        o->oAction = DORRIE_ACT_LOWER_HEAD;
        o->oForwardVel = 0.0f;
        o->oDorrieYawVel = 0;
    } else {
        if (gMarioObject->platform == o) {
            targetYaw = gMarioObject->oFaceAngleYaw;
            targetSpeed = 10;
        } else {
            s16 circularTurn = 0x4000 - atan2s(2000.0f * levelScaleH, o->oDorrieDistToHome - 2000.0f * levelScaleH);
            if ((s16)(o->oMoveAngleYaw - o->oDorrieAngleToHome) < 0) {
                circularTurn = -circularTurn;
            }

            targetYaw = o->oDorrieAngleToHome + circularTurn;
            targetSpeed = 5;
        }

        obj_forward_vel_approach(targetSpeed, 0.5f);
        o->oDorrieYawVel =
            approach_s16_symmetric(o->oDorrieYawVel, (s16)(targetYaw - o->oMoveAngleYaw) / 50, 5);
        o->oMoveAngleYaw += o->oDorrieYawVel;
    }

    o->oAngleVelYaw = o->oMoveAngleYaw - startYaw;
}

