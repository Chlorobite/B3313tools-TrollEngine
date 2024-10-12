#include "mario_actions_airborne_headers.h"

s32 check_horizontal_wind(struct MarioState *m) {
    struct Surface *floor;
    f32 speed;
    s16 pushAngle;

    floor = m->floor;

    if (floor->type == SURFACE_HORIZONTAL_WIND) {
        pushAngle = floor->force << 8;

        m->slideVelX += 1.2f * sins(pushAngle);
        m->slideVelZ += 1.2f * coss(pushAngle);

        speed = sqrtf(m->slideVelX * m->slideVelX + m->slideVelZ * m->slideVelZ);

        if (speed > 48.0f) {
            m->slideVelX = m->slideVelX * 48.0f / speed;
            m->slideVelZ = m->slideVelZ * 48.0f / speed;
            speed = 32.0f; //! This was meant to be 48?
        } else if (speed > 32.0f) {
            speed = 32.0f;
        }

        m->vel[0] = m->slideVelX;
        m->vel[2] = m->slideVelZ;
        m->slideYaw = atan2s(m->slideVelZ, m->slideVelX);
        m->forwardVel = speed * coss(m->faceAngle[1] - m->slideYaw);

        play_sound(SOUND_ENV_WIND2, m->marioObj->header.gfx.cameraToObject);
        return TRUE;
    }

    return FALSE;
}
