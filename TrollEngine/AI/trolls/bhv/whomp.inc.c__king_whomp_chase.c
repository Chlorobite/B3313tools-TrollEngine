#include "bhv_headers.h"
#include "AI/stats_tracking.h"


void king_whomp_chase(void) {
    register struct Object *_o = o;
    
    cur_obj_init_animation_with_accel_and_sound(0, 1.0f);
    _o->oForwardVel = 3.0f;
    cur_obj_rotate_yaw_toward(_o->oAngleToMario, 0x200 * TRACKER_difficulty_modifier_half);

    if (_o->oTimer > 30) {
        if (abs_angle_diff(_o->oAngleToMario, _o->oMoveAngleYaw) < 0x2000) {
            if (_o->oDistanceToMario < 1500.0f) {
                _o->oForwardVel = 9.0f;
                cur_obj_init_animation_with_accel_and_sound(0, 3.0f);
            }
            if (_o->oDistanceToMario < 300.0f) {
                _o->oAction = 3;
            }
        }
    }
    _o->oForwardVel *= TRACKER_difficulty_modifier_sqrt_half;

    whomp_play_sfx_from_pound_animation();

    if (mario_is_far_below_object(1000.0f)) {
        _o->oAction = 0;
        stop_background_music(SEQUENCE_ARGS(4, SEQ_EVENT_BOSS));
    }
}

