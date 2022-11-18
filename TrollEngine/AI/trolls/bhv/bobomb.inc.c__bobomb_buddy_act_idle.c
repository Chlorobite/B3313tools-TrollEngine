#include "bhv_headers.h"

void bobomb_buddy_act_idle(void) {
    s16 animFrame = o->header.gfx.animInfo.animFrame;

    o->oBobombBuddyPosXCopy = o->oPosX;
    o->oBobombBuddyPosYCopy = o->oPosY;
    o->oBobombBuddyPosZCopy = o->oPosZ;

    object_step();

    if (o->oDistanceToMario < 1000.0f && (u32)o->header.gfx.sharedChild != (u32)gLoadedGraphNodes[MODEL_WOODEN_SIGNPOST]) {
        o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oAngleToMario, 0x140);
    }

    if (o->oInteractStatus == INT_STATUS_INTERACTED) {
        o->oAction = BOBOMB_BUDDY_ACT_TURN_TO_TALK;
    }
}
