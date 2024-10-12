#include "mario_actions_automatic_headers.h"

s32 act_hanging(struct MarioState *m) {
    if (m->input & INPUT_NONZERO_ANALOG) {
        return set_mario_action(m, ACT_HANG_MOVING, m->actionArg);
    }

    if (!(m->input & INPUT_A_DOWN)) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_Z_PRESSED) {
        return set_mario_action(m, ACT_GROUND_POUND, 0);
    }

    if (m->ceil->type != SURFACE_HANGABLE) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->actionArg & 1) {
        set_mario_animation(m, MARIO_ANIM_HANDSTAND_LEFT);
    } else {
        set_mario_animation(m, MARIO_ANIM_HANDSTAND_RIGHT);
    }

    update_hang_stationary(m);

    return FALSE;
}
