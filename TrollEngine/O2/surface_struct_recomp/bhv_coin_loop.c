#include "Trolls/bhv/bhv_headers.h"

void bhv_coin_loop(void) {
    struct Surface *sp1C;

    cur_obj_update_floor_and_walls();
    cur_obj_if_hit_wall_bounce_away();
    cur_obj_move_standard(-62);

    if ((sp1C = o->oFloor) != NULL) {
        if (o->oMoveFlags & OBJ_MOVE_ON_GROUND) {
            o->oSubAction = 1;
        }
        if (o->oSubAction == 1) {
            o->oBounciness = 0;
            if (sp1C->normal.y < 0.9) {
                s16 sp1A = atan2s(sp1C->normal.z, sp1C->normal.x);
                cur_obj_rotate_yaw_toward(sp1A, 0x400);
            }
        }
    }

    if (o->oTimer == 0) {
        cur_obj_play_sound_2(SOUND_GENERAL_COIN_SPURT);
    }

    if (o->oVelY < 0) {
        cur_obj_become_tangible();
    }

    if (o->oMoveFlags & OBJ_MOVE_LANDED) {
        if (o->oMoveFlags & OBJ_MOVE_ABOVE_LAVA)
        {
            obj_mark_for_deletion(o);
        }
    }

    if (o->oMoveFlags & OBJ_MOVE_BOUNCE) {
        cur_obj_play_sound_2(SOUND_GENERAL_COIN_DROP);
    }

    if (cur_obj_wait_then_blink(400, 20)) {
        obj_mark_for_deletion(o);
    }

    bhv_coin_sparkles_init();
}
