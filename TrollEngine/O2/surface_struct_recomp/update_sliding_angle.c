#include "mario_actions_moving_headers.h"

void update_sliding_angle(struct MarioState *m, f32 accel, f32 lossFactor) {
    s32 newFacingDYaw;
    s16 facingDYaw;

    struct Surface *floor = m->floor;
    s16 slopeAngle = atan2s(floor->normal.z, floor->normal.x);
    f32 steepness = sqrtf(floor->normal.x * floor->normal.x + floor->normal.z * floor->normal.z);

    m->slideVelX += accel * steepness * sins(slopeAngle);
    m->slideVelZ += accel * steepness * coss(slopeAngle);

    m->slideVelX *= lossFactor;
    m->slideVelZ *= lossFactor;

    m->slideYaw = atan2s(m->slideVelZ, m->slideVelX);

    facingDYaw = m->faceAngle[1] - m->slideYaw;
    newFacingDYaw = facingDYaw;

    //! -0x4000 not handled - can slide down a slope while facing perpendicular to it
    if (newFacingDYaw > 0 && newFacingDYaw <= 0x4000) {
        if ((newFacingDYaw -= 0x200) < 0) {
            newFacingDYaw = 0;
        }
    } else if (newFacingDYaw > -0x4000 && newFacingDYaw < 0) {
        if ((newFacingDYaw += 0x200) > 0) {
            newFacingDYaw = 0;
        }
    } else if (newFacingDYaw > 0x4000 && newFacingDYaw < 0x8000) {
        if ((newFacingDYaw += 0x200) > 0x8000) {
            newFacingDYaw = 0x8000;
        }
    } else if (newFacingDYaw > -0x8000 && newFacingDYaw < -0x4000) {
        if ((newFacingDYaw -= 0x200) < -0x8000) {
            newFacingDYaw = -0x8000;
        }
    }

    m->faceAngle[1] = m->slideYaw + newFacingDYaw;

    m->vel[0] = m->slideVelX;
    m->vel[1] = 0.0f;
    m->vel[2] = m->slideVelZ;

    mario_update_moving_sand(m);
    mario_update_windy_ground(m);

    //! Speed is capped a frame late (butt slide HSG)
    m->forwardVel = sqrtf(m->slideVelX * m->slideVelX + m->slideVelZ * m->slideVelZ);
    if (m->forwardVel > 100.0f) {
        m->slideVelX = m->slideVelX * 100.0f / m->forwardVel;
        m->slideVelZ = m->slideVelZ * 100.0f / m->forwardVel;
    }

    if (newFacingDYaw < -0x4000 || newFacingDYaw > 0x4000) {
        m->forwardVel *= -1.0f;
    }
}
