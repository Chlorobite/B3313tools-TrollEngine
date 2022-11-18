#include "bhv_headers.h"

extern s32 sCapSaveFlags[];

void cap_switch_act_2(void) {
    if (o->oTimer < 5) {
        cur_obj_scale_over_time(2, 4, 0.5f, 0.1f);
        if (o->oBehParams2ndByte == 3)
            fuck_textures();
        if (o->oTimer == 4) {
            cur_obj_shake_screen(SHAKE_POS_SMALL);
            spawn_mist_particles();
            spawn_triangle_break_particles(60, MODEL_CARTOON_STAR, 0.3f, o->oBehParams2ndByte);
#if ENABLE_RUMBLE
            queue_rumble_data(5, 80);
#endif
        }
    } else {
        //! Neither of these flags are defined in this function so they do nothing.
        //  On an extra note, there's a specific check for this cutscene and 
        //  there's no dialog defined since the cutscene itself calls the dialog.
        if (troll_cap_switch_dialog(o->oBehParams2ndByte)) {
            o->oAction = 3;
        }
    }
}

