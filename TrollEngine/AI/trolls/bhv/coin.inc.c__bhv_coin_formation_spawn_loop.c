#include "bhv_headers.h"

extern struct ObjectHitbox sYellowCoinHitbox;

void bhv_coin_formation_spawn_loop(void) {
    if (o->oTimer == 0) {
        cur_obj_set_behavior(bhvYellowCoin);
        obj_set_hitbox(o, &sYellowCoinHitbox);
        bhv_init_room();

        if (o->oCoinUnkF8) {
            o->oPosY += 300.0f;
            cur_obj_update_floor_height();

            if (o->oPosY < o->oFloorHeight || o->oFloorHeight < FLOOR_LOWER_LIMIT_MISC) {
                obj_mark_for_deletion(o);
            } else {
                o->oPosY = o->oFloorHeight;
            }
        } else {
            cur_obj_update_floor_height();
        }
    
        personalize_coin(o, 1);
    } else {
        if (bhv_coin_sparkles_init()) {
            o->parentObj->oCoinUnkF4 |= bit_shift_left(o->oBehParams2ndByte);
        }
        o->oAnimState++;
    }

    if (o->parentObj->oAction == 2) {
        obj_mark_for_deletion(o);
    }
}

