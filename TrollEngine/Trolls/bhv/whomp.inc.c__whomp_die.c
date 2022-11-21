#include "bhv_headers.h"


void whomp_die(void) {
    if (o->oBehParams2ndByte != 0) {
        if (cur_obj_update_dialog(MARIO_DIALOG_LOOK_UP, DIALOG_FLAG_TEXT_DEFAULT, DIALOG_115, 0)) {
            obj_set_angle(o, 0, 0, 0);
            cur_obj_hide();
            cur_obj_become_intangible();
            spawn_mist_particles_variable(0, 0, 200.0f);
            spawn_triangle_break_particles(20, MODEL_DIRT_ANIMATION, 3.0f, 4);
            cur_obj_shake_screen(SHAKE_POS_SMALL);
            spawn_boss_star((void*)0x80404F10);
            cur_obj_play_sound_2(SOUND_OBJ_KING_WHOMP_DEATH);
            o->oAction = 9;
        }
    } else {
        spawn_mist_particles_variable(0, 0, 100.0f);
        spawn_triangle_break_particles(20, MODEL_DIRT_ANIMATION, 3.0f, 4);
        cur_obj_shake_screen(SHAKE_POS_SMALL);
        create_sound_spawner(SOUND_OBJ_THWOMP);
        obj_mark_for_deletion(o);
    }
}

