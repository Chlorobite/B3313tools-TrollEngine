#include "bhv_headers.h"


void eyerok_boss_act_die(void) {
    if (o->oTimer == 60) {
        if (cur_obj_update_dialog(MARIO_DIALOG_LOOK_UP, DIALOG_FLAG_TEXT_DEFAULT, DIALOG_118, 0)) {
            spawn_boss_star((void*)0x80404F20);
        } else {
            o->oTimer--;
        }
    } else if (o->oTimer > 120) {
        stop_background_music(SEQUENCE_ARGS(4, SEQ_EVENT_BOSS));
        obj_mark_for_deletion(o);
    }
}

