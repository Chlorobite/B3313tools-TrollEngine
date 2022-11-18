#include "bhv_headers.h"


void eyerok_hand_act_open(void) {
    o->parentObj->oEyerokBossUnk1AC = o->oBehParams2ndByte;

    if (cur_obj_init_anim_and_check_if_end(4)) {
        o->oAction = EYEROK_HAND_ACT_SHOW_EYE;
        o->oEyerokHandUnkFC = 2;
        o->oEyerokHandUnk100 = 60;

        o->collisionData = (void*)ssl_seg7_collision_070282F8;

        if (o->parentObj->oEyerokBossNumHands != 2) {
            s16 sp1E = o->oAngleToMario;
            clamp_s16(&sp1E, -0x3000, 0x3000);
            o->oMoveAngleYaw = sp1E;
            o->oForwardVel = 50.0f;
        } else {
            o->oMoveAngleYaw = 0;
        }
    }
}

