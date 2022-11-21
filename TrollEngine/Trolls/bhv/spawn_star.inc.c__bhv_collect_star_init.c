#include "bhv_headers.h"


u32 blue_star_check(s32 behParams);

void bhv_collect_star_init(void) {
    o->header.gfx.sharedChild = gLoadedGraphNodes[blue_star_check(o->oBehParams) ? MODEL_TRANSPARENT_STAR : MODEL_STAR];

    // rom manger stuff?
    o->oFlags = 1;
    o->header.gfx.node.flags |= 4;

    obj_set_hitbox(o, (void*)0x80332784);
    AI_star_set_bparams(o);
}

