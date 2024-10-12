#include "mario_actions_airborne_headers.h"

s32 lava_boost_on_wall(struct MarioState *m) {
    m->faceAngle[1] = atan2s(m->wall->normal.z, m->wall->normal.x);

    if (m->forwardVel < 24.0f) {
        m->forwardVel = 24.0f;
    }

    if (!(m->flags & MARIO_METAL_CAP)) {
        m->hurtCounter += (m->flags & MARIO_CAP_ON_HEAD) ? 12 : 18;
    }

    play_sound(SOUND_MARIO_ON_FIRE, m->marioObj->header.gfx.cameraToObject);
    update_mario_sound_and_camera(m);
    return drop_and_set_mario_action(m, ACT_LAVA_BOOST, 1);
}
