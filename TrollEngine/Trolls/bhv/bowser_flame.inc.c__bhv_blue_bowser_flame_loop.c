#include "bhv_headers.h"

extern struct ObjectHitbox sGrowingBowserFlameHitbox;


void bhv_blue_bowser_flame_loop(void) {
    s32 i;

    obj_set_hitbox(o, &sGrowingBowserFlameHitbox);

    if (o->oFlameScale < 16.0f) {
        o->oFlameScale = o->oFlameScale + 0.5;
    }

    cur_obj_scale(o->oFlameScale);
    cur_obj_update_floor_and_walls();
    cur_obj_move_standard(78);

    if (o->oTimer > 20) {
        if (loadedObjectCount < 200) {
            if (o->oBehParams2ndByte == 0) {
                for (i = 0; i < 3; i++) {
                    spawn_object_relative_with_scale(0, 0, 0, 0, 5.0f, o, MODEL_RED_FLAME,
                                                    bhvFlameFloatingLanding);
                }
            } else {
                spawn_object_relative_with_scale(1, 0, 0, 0, 8.0f, o, MODEL_BLUE_FLAME,
                                                bhvFlameFloatingLanding);
                spawn_object_relative_with_scale(2, 0, 0, 0, 8.0f, o, MODEL_BLUE_FLAME,
                                                bhvFlameFloatingLanding);
            }
        }
        obj_mark_for_deletion(o);
    }
}
