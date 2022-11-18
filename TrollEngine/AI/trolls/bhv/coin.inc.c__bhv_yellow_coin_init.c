#include "bhv_headers.h"

extern struct ObjectHitbox sYellowCoinHitbox;

void bhv_yellow_coin_init(void) {
    register struct Object *_o = o;
    cur_obj_set_behavior(bhvYellowCoin);
    obj_set_hitbox(_o, &sYellowCoinHitbox);
    bhv_init_room();
    cur_obj_update_floor_height();

    if (_o->oFloorHeight < FLOOR_LOWER_LIMIT_MISC) {
        obj_mark_for_deletion(_o);
    }
    
    personalize_coin(_o, 0);
}

