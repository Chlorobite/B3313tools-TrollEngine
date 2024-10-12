#include "mario_actions_airborne_headers.h"

s32 check_fall_damage(struct MarioState *m, u32 hardFallAction) {
    f32 fallHeight;
    f32 damageHeight;

    fallHeight = m->peakHeight - m->pos[1];

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma GCC diagnostic ignored "-Wtautological-constant-out-of-range-compare"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif

    //! Never true
    if (m->actionState == ACT_GROUND_POUND) {
        damageHeight = 600.0f;
    } else {
        damageHeight = 1150.0f;
    }

#pragma GCC diagnostic pop

    if (m->action != ACT_TWIRLING && m->floor->type != SURFACE_BURNING) {
        if (m->vel[1] < -55.0f) {
            if (fallHeight > 3000.0f) {
                m->hurtCounter += (m->flags & MARIO_CAP_ON_HEAD) ? 16 : 24;
#if ENABLE_RUMBLE
                queue_rumble_data(5, 80);
#endif
                // set_camera_shake_from_hit(SHAKE_FALL_DAMAGE);
                play_sound(SOUND_MARIO_ATTACKED, m->marioObj->header.gfx.cameraToObject);
                return drop_and_set_mario_action(m, hardFallAction, 4);
            } else if (fallHeight > damageHeight && !mario_floor_is_slippery(m)) {
                m->hurtCounter += (m->flags & MARIO_CAP_ON_HEAD) ? 8 : 12;
                m->squishTimer = 30;
#if ENABLE_RUMBLE
                queue_rumble_data(5, 80);
#endif
                // set_camera_shake_from_hit(SHAKE_FALL_DAMAGE);
                play_sound(SOUND_MARIO_ATTACKED, m->marioObj->header.gfx.cameraToObject);
            }
        }
    }

    return FALSE;
}
