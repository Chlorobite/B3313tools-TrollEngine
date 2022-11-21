#include "bhv_headers.h"


void king_bobomb_act_3(void) {
    if (o->oSubAction == 0) {
        o->oForwardVel = 0.0f;
        o->oKingBobombUnk104 = 0;
        o->oKingBobombUnkFC = 0;

        if (o->oTimer == 0) {
            cur_obj_play_sound_2(SOUND_OBJ_UNKNOWN3);
        }

        if (cur_obj_init_animation_and_check_if_near_end(0)) {
            o->oSubAction++;
            cur_obj_init_animation_and_anim_frame(1, 0);
        }
    } else if (o->oSubAction == 1) {
        cur_obj_init_animation_with_sound(1);
        o->oKingBobombUnkFC += player_performed_grab_escape_action();

        if (o->oKingBobombUnkFC > 10) {
            o->oKingBobombUnk88 = 3;
            o->oAction = 2;
            o->oKingBobombUnk108 = 35;
            o->oInteractStatus &= ~INT_STATUS_GRABBED_MARIO;
        } else {
            o->oForwardVel = 3.0f * TRACKER_difficulty_modifier_half;

            if (o->oKingBobombUnk104 > 20 && cur_obj_rotate_yaw_toward(0, 0x400)) {
                o->oSubAction++;
                cur_obj_init_animation_and_anim_frame(9, 22);
            }
        }

        o->oKingBobombUnk104++;
    } else {
        cur_obj_init_animation_with_sound(9);

        if (cur_obj_check_anim_frame(31)) {
            o->oKingBobombUnk88 = 2;
            cur_obj_play_sound_2(SOUND_OBJ_UNKNOWN4);
        } else if (cur_obj_check_if_near_animation_end()) {
            o->oAction = 1;
            o->oInteractStatus &= ~INT_STATUS_GRABBED_MARIO;
        }
    }
}

