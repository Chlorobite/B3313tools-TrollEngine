#include "mario_actions_airborne_headers.h"

s32 act_lava_boost(struct MarioState *m) {
#if ENABLE_RUMBLE
    if (!(m->flags & MARIO_MARIO_SOUND_PLAYED)) {
#endif
        play_sound_if_no_flag(m, SOUND_MARIO_ON_FIRE, MARIO_MARIO_SOUND_PLAYED);
#if ENABLE_RUMBLE
        queue_rumble_data(5, 80);
    }
#endif

    if (!(m->input & INPUT_NONZERO_ANALOG)) {
        m->forwardVel = approach_f32(m->forwardVel, 0.0f, 0.35f, 0.35f);
    }

    update_lava_boost_or_twirling(m);

    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            if (m->floor->type == SURFACE_BURNING) {
                m->actionState = 0;
                if (!(m->flags & MARIO_METAL_CAP)) {
                    m->hurtCounter += (m->flags & MARIO_CAP_ON_HEAD) ? 12 : 18;
                }
                m->vel[1] = 84.0f;
                play_sound(SOUND_MARIO_ON_FIRE, m->marioObj->header.gfx.cameraToObject);
#if ENABLE_RUMBLE
                queue_rumble_data(5, 80);
#endif
            } else {
                play_mario_heavy_landing_sound(m, SOUND_ACTION_TERRAIN_BODY_HIT_GROUND);
                if (m->actionState < 2 && m->vel[1] < 0.0f) {
                    m->vel[1] = -m->vel[1] * 0.4f;
                    mario_set_forward_vel(m, m->forwardVel * 0.5f);
                    m->actionState++;
                } else {
                    set_mario_action(m, ACT_LAVA_BOOST_LAND, 0);
                }
            }
            break;

        case AIR_STEP_HIT_WALL:
            mario_bonk_reflection(m, FALSE);
            break;

        case AIR_STEP_HIT_LAVA_WALL:
            lava_boost_on_wall(m);
            break;
    }

    set_mario_animation(m, MARIO_ANIM_FIRE_LAVA_BURN);
    if ((m->area->terrainType & TERRAIN_MASK) != TERRAIN_SNOW && !(m->flags & MARIO_METAL_CAP)
        && m->vel[1] > 0.0f) {
        m->particleFlags |= PARTICLE_FIRE;
        if (m->actionState == 0) {
            play_sound(SOUND_MOVING_LAVA_BURN, m->marioObj->header.gfx.cameraToObject);
        }
    }

    if (m->health < 0x100) {
        level_trigger_warp(m, WARP_OP_DEATH);
    }

    m->marioBodyState->eyeState = MARIO_EYES_DEAD;
#if ENABLE_RUMBLE
    reset_rumble_timers();
#endif
    return FALSE;
}
