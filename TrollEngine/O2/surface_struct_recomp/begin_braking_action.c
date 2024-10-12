#include "mario_actions_moving_headers.h"

s32 begin_braking_action(struct MarioState *m) {
    mario_drop_held_object(m);

    if (m->actionState == 1) {
        m->faceAngle[1] = m->actionArg;
        return set_mario_action(m, ACT_STANDING_AGAINST_WALL, 0);
    }

    if (m->forwardVel >= 16.0f && m->floor->normal.y >= 0.17364818f) {
        return set_mario_action(m, ACT_DECELERATING, 0); // beeie code.
    }

    return set_mario_action(m, ACT_DECELERATING, 0);
}
