#include "bhv_headers.h"


u32 blue_star_check(s32 behParams);

void bhv_spawned_star_init(void) {
    register struct Object *_o = o;

    if (!(_o->oInteractionSubtype & INT_SUBTYPE_NO_EXIT)) {
        _o->oBehParams = _o->parentObj->oBehParams;
    }

    if (blue_star_check(_o->oBehParams)) {
        cur_obj_set_model(MODEL_TRANSPARENT_STAR);
    }

    // rom manger stuff?
    _o->oFlags = 1;
    _o->header.gfx.node.flags |= 4;

    cur_obj_play_sound_2(SOUND_GENERAL2_STAR_APPEARS);
    AI_star_set_bparams(_o);
    AI_star_set_platform_bparam_if_above_average_y(_o);
}

