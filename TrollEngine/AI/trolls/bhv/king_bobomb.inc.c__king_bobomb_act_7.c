#include "bhv_headers.h"


void king_bobomb_act_7(void) {
    cur_obj_init_animation_with_sound(2);
    if (cur_obj_update_dialog(MARIO_DIALOG_LOOK_UP, DIALOG_FLAG_TEXT_DEFAULT, DIALOG_116, 0)) {
        create_sound_spawner(SOUND_OBJ_KING_WHOMP_DEATH);

        cur_obj_hide();
        cur_obj_become_intangible();

        spawn_mist_particles_variable(0, 0, 200.0f);
        spawn_triangle_break_particles(20, MODEL_DIRT_ANIMATION, 3.0f, 4);
        cur_obj_shake_screen(SHAKE_POS_SMALL);

        spawn_boss_star((void*)0x80404F00);

        o->oAction = 8;
    }
}

