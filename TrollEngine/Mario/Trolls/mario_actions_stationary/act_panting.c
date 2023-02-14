#include "../mario_headers.h"


s32 act_panting(struct MarioState *m) {
    if (m->input & INPUT_STOMPED) {
        return set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

    if (m->floor->type == SURFACE_ELECTRIC && m->actionTimer >= 4) {
		return hurt_and_set_mario_action(m, ACT_ELECTRIC_FLOOR_IDLE, 0, 4);
	}

    if (m->health >= 0x500) {
        return set_mario_action(m, ACT_IDLE, 0);
    }

    if (check_common_idle_cancels(m)) {
        return TRUE;
    }

    if (set_mario_animation(m, MARIO_ANIM_WALK_PANTING) == 1) {
        m->actionTimer++;
    }

    stationary_ground_step(m);
    m->marioBodyState->eyeState = MARIO_EYES_HALF_CLOSED;
    return FALSE;
}
