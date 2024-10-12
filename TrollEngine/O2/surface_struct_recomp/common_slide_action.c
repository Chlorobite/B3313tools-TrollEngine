#include "mario_actions_moving_headers.h"

void common_slide_action(struct MarioState *m, u32 endAction, u32 airAction, s32 animation) {
    Vec3f pos;

    vec3f_copy(pos, m->pos);
    play_sound(SOUND_MOVING_TERRAIN_SLIDE + m->terrainSoundAddend, m->marioObj->header.gfx.cameraToObject);

#if ENABLE_RUMBLE
    reset_rumble_timers();
#endif

    adjust_sound_for_speed(m);

    switch (perform_ground_step(m)) {
        case GROUND_STEP_LEFT_GROUND:
            set_mario_action(m, airAction, 0);
            if (m->forwardVel < -50.0f || 50.0f < m->forwardVel) {
                play_sound(SOUND_MARIO_HOOHOO, m->marioObj->header.gfx.cameraToObject);
            }
            break;

        case GROUND_STEP_NONE:
            set_mario_animation(m, animation);
            align_with_floor(m);
            m->particleFlags |= PARTICLE_DUST;
            break;

        case GROUND_STEP_HIT_WALL:
            if (!mario_floor_is_slippery(m)) {
                m->particleFlags |= PARTICLE_VERTICAL_STAR;
                slide_bonk(m, ACT_GROUND_BONK, endAction);
            } else if (m->wall != NULL) {
                s16 wallAngle = atan2s(m->wall->normal.z, m->wall->normal.x);
                f32 slideSpeed = sqrtf(m->slideVelX * m->slideVelX + m->slideVelZ * m->slideVelZ);

                if ((slideSpeed *= 0.9f) < 4.0f) {
                    slideSpeed = 4.0f;
                }

                m->slideYaw = wallAngle - (s16)(m->slideYaw - wallAngle) + 0x8000;

                m->vel[0] = m->slideVelX = slideSpeed * sins(m->slideYaw);
                m->vel[2] = m->slideVelZ = slideSpeed * coss(m->slideYaw);
            }

            align_with_floor(m);
            break;
    }
}
