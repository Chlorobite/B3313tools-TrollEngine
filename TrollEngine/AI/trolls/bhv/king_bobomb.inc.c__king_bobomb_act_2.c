#include "bhv_headers.h"


s32 mario_is_far_below_object(f32 arg0);
void king_bobomb_act_2(void) {
    register struct Object *_o = o;
    cur_obj_become_tangible();

    if (_o->oPosY - _o->oHomeY < -100.0f) { // Thrown off hill
        _o->oAction = 5;
        cur_obj_become_intangible();
    }

    if (_o->oKingBobombUnk100 == 0) {
        if (cur_obj_check_anim_frame(15)) {
            cur_obj_shake_screen(SHAKE_POS_SMALL);
        }

        if (cur_obj_init_animation_and_check_if_near_end(4)) {
            _o->oKingBobombUnk100++;
        }
    } else {
        if (_o->oKingBobombUnk100 == 1) {
            cur_obj_init_animation_and_anim_frame(11, 7);
            _o->oKingBobombUnk100 = 2;
        } else {
            cur_obj_init_animation_with_sound(11);
        }

        if (_o->oKingBobombUnk108 == 0) {
            _o->oForwardVel = 3.0f * TRACKER_difficulty_modifier_half;
            cur_obj_rotate_yaw_toward(_o->oAngleToMario, 0x100 * TRACKER_difficulty_modifier_half);
        } else {
            _o->oForwardVel = 0.0f;
            _o->oKingBobombUnk108--;
        }
    }

    if (cur_obj_check_grabbed_mario()) {
        TRACKER_boss_performance -= 1.0f;
        _o->oAction = 3;
    }

    if (mario_is_far_below_object(1200.0f)) {
        _o->oAction = 0;
        stop_background_music(SEQUENCE_ARGS(4, SEQ_EVENT_BOSS));
    }
}

