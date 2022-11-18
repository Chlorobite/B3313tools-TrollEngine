#include "bhv_headers.h"


void eyerok_hand_act_attacked(void) {
    if (cur_obj_init_anim_and_check_if_end(3)) {
        o->oAction = EYEROK_HAND_ACT_RECOVER;
        o->collisionData = (void*)ssl_seg7_collision_07028274;
    }

    if (o->oMoveFlags & OBJ_MOVE_MASK_ON_GROUND) {
        o->oForwardVel = 0.0f;
    }
}

