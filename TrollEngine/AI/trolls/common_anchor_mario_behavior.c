#include "Trolls/bhv/bhv_headers.h"

void common_anchor_mario_behavior(register f32 sp28, register f32 sp2C, register s32 sp30) {
	// ADD: personalization AI, scale throw force (chuckya, king bom) by difficulty modifier
    sp28 *= TRACKER_difficulty_modifier_sqrt_half;
    sp2C *= TRACKER_difficulty_modifier_sqrt_half;
	// END ADD
    
    switch (o->parentObj->oChuckyaUnk88) {
        case 1:
            obj_set_gfx_pos_at_obj_pos(gMarioObject, o);
            break;

        case 2:
            gMarioObject->oInteractStatus |= (INT_STATUS_MARIO_UNK2 + sp30);
            gMarioStates[0].forwardVel = sp28;
            gMarioStates[0].vel[1] = sp2C;
            o->parentObj->oChuckyaUnk88 = 0;
            break;

        case 3:
            gMarioObject->oInteractStatus |= (INT_STATUS_MARIO_UNK2 | INT_STATUS_MARIO_UNK6);
            gMarioStates[0].forwardVel = 10.0f;
            gMarioStates[0].vel[1] = 10.0f;
            o->parentObj->oChuckyaUnk88 = 0;
            break;
    }

    o->oMoveAngleYaw = o->parentObj->oMoveAngleYaw;

    if (o->parentObj->activeFlags == ACTIVE_FLAG_DEACTIVATED) {
        obj_mark_for_deletion(o);
    }
}
