#include "bhv_headers.h"


u32 blue_star_check(s32 behParams);

void bhv_collect_star_init(void) {
    register struct Object *_o = o;

    if (personalization_beeparams & 0x60) { // red and green star flags
        // as red and green stars are all in the high region, set the high flag
        _o->oBehParams |= 0x100;
    }

    _o->header.gfx.sharedChild = gLoadedGraphNodes[blue_star_check(_o->oBehParams) ? MODEL_TRANSPARENT_STAR : MODEL_STAR];

    // rom manger stuff?
    _o->oFlags = 1;
    _o->header.gfx.node.flags |= 4;

    obj_set_hitbox(_o, (void*)0x80332784);
    AI_star_set_bparams(_o);
}

