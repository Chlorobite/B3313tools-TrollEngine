#include "mario_actions_submerged_headers.h"

void update_metal_water_walking_speed(struct MarioState *m) {
    f32 val = m->intendedMag / 1.5f;

    if (m->forwardVel <= 0.0f) {
        m->forwardVel += 1.1f;
    } else if (m->forwardVel <= val) {
        m->forwardVel += 1.1f - m->forwardVel / 43.0f;
    } else if (m->floor->normal.y >= 0.95f) {
        m->forwardVel -= 1.0f;
    }

    if (m->forwardVel > 32.0f) {
        m->forwardVel = 32.0f;
    }

    m->faceAngle[1] =
        m->intendedYaw - approach_s32((s16)(m->intendedYaw - m->faceAngle[1]), 0, 0x800, 0x800);

    m->slideVelX = m->forwardVel * sins(m->faceAngle[1]);
    m->slideVelZ = m->forwardVel * coss(m->faceAngle[1]);

    m->vel[0] = m->slideVelX;
    m->vel[1] = 0.0f;
    m->vel[2] = m->slideVelZ;
}
