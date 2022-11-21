#include "bhv_headers.h"


void king_bobomb_act_0(void) {
    if (o->oSubAction == 0) {
        cur_obj_become_intangible();
        gSecondCameraFocus = o;
        cur_obj_init_animation_with_sound(5);
        cur_obj_set_pos_to_home();
        o->oHealth = (int)(3 * TRACKER_difficulty_modifier_half);

        if (cur_obj_can_mario_activate_textbox_2(500.0f, 100.0f)) {
            o->oSubAction++;
            seq_player_lower_volume(SEQ_PLAYER_LEVEL, 60, 40);
        }
    } else if (cur_obj_update_dialog_with_cutscene(MARIO_DIALOG_LOOK_UP,
        DIALOG_FLAG_TURN_TO_MARIO, CUTSCENE_DIALOG, DIALOG_017)) {
        o->oAction = 2;
        o->oFlags |= OBJ_FLAG_HOLDABLE;
    }
}

