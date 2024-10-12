#include "mario_actions_automatic_headers.h"

s32 update_hang_moving(struct MarioState *m) {
    s32 stepResult;
    Vec3f nextPos;
    f32 maxSpeed = 4.0f;

    m->forwardVel += 1.0f;
    if (m->forwardVel > maxSpeed) {
        m->forwardVel = maxSpeed;
    }

    m->faceAngle[1] =
        m->intendedYaw - approach_s32((s16)(m->intendedYaw - m->faceAngle[1]), 0, 0x800, 0x800);

    m->slideYaw = m->faceAngle[1];
    m->slideVelX = m->forwardVel * sins(m->faceAngle[1]);
    m->slideVelZ = m->forwardVel * coss(m->faceAngle[1]);

    m->vel[0] = m->slideVelX;
    m->vel[1] = 0.0f;
    m->vel[2] = m->slideVelZ;

    nextPos[0] = m->pos[0] - m->ceil->normal.y * m->vel[0];
    nextPos[2] = m->pos[2] - m->ceil->normal.y * m->vel[2];
    nextPos[1] = m->pos[1];

    stepResult = perform_hanging_step(m, nextPos);

    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);
    return stepResult;
}
