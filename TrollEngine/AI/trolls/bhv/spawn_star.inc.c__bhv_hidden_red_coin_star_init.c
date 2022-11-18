#include "bhv_headers.h"


void bhv_hidden_red_coin_star_init(void) {
    register struct Object *_o = o;
    register s16 count = count_objects_with_behavior(bhvRedCoin);

    spawn_object(_o, MODEL_TRANSPARENT_STAR, bhvRedCoinStarMarker);

    if (count == 0) {
        register struct Object *star = spawn_object(_o, MODEL_STAR, bhvStar);
        star->oBehParams = _o->oBehParams;
        star->oBooTurningSpeed |= 0x02;
        _o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }

    _o->oHiddenStarTriggerCounter = 8 - count;
}

