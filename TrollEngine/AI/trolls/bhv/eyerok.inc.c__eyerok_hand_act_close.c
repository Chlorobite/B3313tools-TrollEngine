#include "bhv_headers.h"


void eyerok_hand_act_close(void) {
    if (cur_obj_init_anim_check_frame(7, 1)) {
        o->collisionData = (void*)ssl_seg7_collision_07028274;

        if (o->parentObj->oEyerokBossNumHands != 2) {
            o->oAction = EYEROK_HAND_ACT_RETREAT;
            o->parentObj->oEyerokBossActiveHand = o->oBehParams2ndByte;
        } else if (o->parentObj->oEyerokBossActiveHand == 0) {
            o->oAction = EYEROK_HAND_ACT_IDLE;
            o->parentObj->oEyerokBossUnk1AC = 0;
        }
    }
}

