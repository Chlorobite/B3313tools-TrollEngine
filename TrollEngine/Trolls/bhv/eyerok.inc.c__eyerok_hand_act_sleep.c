#include "bhv_headers.h"


void eyerok_hand_act_sleep(void) {
    if (o->parentObj->oAction != EYEROK_BOSS_ACT_SLEEP
        && ++o->oEyerokHandWakeUpTimer > -3 * o->oBehParams2ndByte) {
        if (cur_obj_check_if_near_animation_end()) {
            o->parentObj->oEyerokBossNumHands++;
            o->oAction = EYEROK_HAND_ACT_IDLE;
            o->collisionData = (void*)ssl_seg7_collision_07028274;
        } else {
            approach_f32_ptr(&o->oPosX, o->oHomeX, 15.0f);
            o->oPosY = o->oHomeY
                       + (200 * o->oBehParams2ndByte + 400)
                             * sins((s16)(absf(o->oPosX - o->oHomeX) / 724.0f * 0x8000));
            obj_face_yaw_approach(o->oMoveAngleYaw, 400);
        }
    } else {
        if (o->oBehParams2ndByte < 0) {
            o->collisionData = (void*)ssl_seg7_collision_070284B0;
        } else {
            o->collisionData = (void*)ssl_seg7_collision_07028370;
        }

        cur_obj_reverse_animation();
        o->oPosX = o->oHomeX + 724.0f * o->oBehParams2ndByte;
    }
}

