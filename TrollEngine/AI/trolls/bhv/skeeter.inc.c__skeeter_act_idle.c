#include "bhv_headers.h"


void skeeter_act_idle(void) {
    if (o->oMoveFlags & OBJ_MOVE_MASK_ON_GROUND && !(o->oMoveFlags & OBJ_MOVE_ABOVE_LAVA)) {
        cur_obj_init_animation_with_sound(3);
        o->oForwardVel = 0.0f;

        if (o->oTimer > o->oSkeeterWaitTime && cur_obj_check_if_near_animation_end()) {
            o->oAction = SKEETER_ACT_WALK;
        }
    } else {
        cur_obj_init_animation_with_sound(1);

        if (o->oMoveFlags & (OBJ_MOVE_AT_WATER_SURFACE | OBJ_MOVE_ABOVE_LAVA)) {
            skeeter_spawn_waves();
            if (o->oTimer > 60
                && obj_smooth_turn(&o->oSkeeterUnk1AC, &o->oMoveAngleYaw, o->oSkeeterTargetAngle,
                                   0.02f, 5, 50, 200)) {
                if (o->oSkeeterWaitTime != 0) {
                    o->oSkeeterWaitTime--;
                } else if (cur_obj_check_if_near_animation_end()) {
                    cur_obj_play_sound_2(SOUND_OBJ_WALKING_WATER);
                    o->oAction = SKEETER_ACT_LUNGE;
                    o->oForwardVel = 80.0f;
                    o->oSkeeterUnk1AC = 0;
                }
            }
        }
    }
}

