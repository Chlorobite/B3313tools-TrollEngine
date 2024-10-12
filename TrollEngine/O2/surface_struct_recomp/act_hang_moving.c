#include "mario_actions_automatic_headers.h"

s32 act_hang_moving(struct MarioState *m) {
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
        set_mario_animation(m, MARIO_ANIM_MOVE_ON_WIRE_NET_RIGHT);
    } else {
        set_mario_animation(m, MARIO_ANIM_MOVE_ON_WIRE_NET_LEFT);
    }

    if (m->marioObj->header.gfx.animInfo.animFrame == 12) {
        play_sound(SOUND_ACTION_HANGING_STEP, m->marioObj->header.gfx.cameraToObject);
#if ENABLE_RUMBLE
        queue_rumble_data(1, 30);
#endif
    }

    if (is_anim_past_end(m)) {
        m->actionArg ^= 1;
        if (m->input & INPUT_UNKNOWN_5) {
            return set_mario_action(m, ACT_HANGING, m->actionArg);
        }
    }

    if (update_hang_moving(m) == HANG_LEFT_CEIL) {
        set_mario_action(m, ACT_FREEFALL, 0);
    }

    return FALSE;
}
