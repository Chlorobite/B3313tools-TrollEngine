#include "mario_actions_moving_headers.h"

void update_walking_speed(struct MarioState *m) {
    f32 maxTargetSpeed;
    f32 targetSpeed;

    if (m->floor != NULL && m->floor->type == SURFACE_SLOW) {
        maxTargetSpeed = 24.0f;
    } else {
        maxTargetSpeed = 32.0f;
    }

    targetSpeed = m->intendedMag < maxTargetSpeed ? m->intendedMag : maxTargetSpeed;

    if (m->quicksandDepth > 10.0f) {
        targetSpeed *= 6.25f / m->quicksandDepth;
    }

    if (m->forwardVel <= 0.0f) {
        m->forwardVel += 1.1f;
    } else if (m->forwardVel <= targetSpeed) {
        m->forwardVel += 1.1f - m->forwardVel / 43.0f;
    } else if (m->floor->normal.y >= 0.95f) {
        m->forwardVel -= 1.0f;
    }

    if (m->forwardVel > 48.0f) {
        m->forwardVel = 48.0f;
    }

    m->faceAngle[1] =
        m->intendedYaw - approach_s32((s16)(m->intendedYaw - m->faceAngle[1]), 0, 0x800, 0x800);
    apply_slope_accel(m);
}
