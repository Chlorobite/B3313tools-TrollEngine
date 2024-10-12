#include "mario_actions_stationary_headers.h"

s32 check_common_idle_cancels(struct MarioState *m) {
    mario_drop_held_object(m);
    if (m->floor->normal.y < 0.29237169f) {
        return mario_push_off_steep_floor(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_STOMPED) {
        return set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

    if (m->input & INPUT_A_PRESSED) {
        return set_jumping_action(m, ACT_JUMP, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }

    if (m->input & INPUT_FIRST_PERSON) {
        return set_mario_action(m, ACT_FIRST_PERSON, 0);
    }

    if (m->input & INPUT_NONZERO_ANALOG) {
        m->faceAngle[1] = (s16) m->intendedYaw;
        return set_mario_action(m, ACT_WALKING, 0);
    }

    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_PUNCHING, 0);
    }

    if (m->input & INPUT_Z_DOWN) {
        return set_mario_action(m, ACT_START_CROUCHING, 0);
    }

    return FALSE;
}
