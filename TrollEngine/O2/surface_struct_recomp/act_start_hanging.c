#include "mario_actions_automatic_headers.h"

s32 act_start_hanging(struct MarioState *m) {
#if ENABLE_RUMBLE
    if (m->actionTimer++ == 0) {
        queue_rumble_data(5, 80);
    }
#else
    m->actionTimer++;
#endif

    if ((m->input & INPUT_NONZERO_ANALOG) && m->actionTimer >= 31) {
        return set_mario_action(m, ACT_HANGING, 0);
    }

    if (!(m->input & INPUT_A_DOWN)) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_Z_PRESSED) {
        return set_mario_action(m, ACT_GROUND_POUND, 0);
    }

    //! Crash if Mario's referenced ceiling is NULL (same for other hanging actions)
    if (m->ceil->type != SURFACE_HANGABLE) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    set_mario_animation(m, MARIO_ANIM_HANG_ON_CEILING);
    play_sound_if_no_flag(m, SOUND_ACTION_HANGING_STEP, MARIO_ACTION_SOUND_PLAYED);
    update_hang_stationary(m);

    if (is_anim_at_end(m)) {
        set_mario_action(m, ACT_HANGING, 0);
    }

    return FALSE;
}
