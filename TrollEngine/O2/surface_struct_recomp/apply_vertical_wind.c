#include "mario_step_headers.h"

void apply_vertical_wind(register struct MarioState *m) {
    register f32 maxVelY;
    register f32 offsetY;

    if (m->action != ACT_GROUND_POUND) {
        offsetY = m->pos[1] - -1500.0f;

        if (m->floor->type == SURFACE_VERTICAL_WIND && -3000.0f < offsetY && offsetY < 2000.0f) {
            maxVelY = 50.0f;
            if (offsetY >= 0.0f) {
                maxVelY = 10000.0f / (offsetY + 200.0f);
            }

            if (m->vel[1] < maxVelY) {
                if ((m->vel[1] += maxVelY / 8.0f) > maxVelY) {
                    m->vel[1] = maxVelY;
                }
            }

            play_sound(SOUND_ENV_WIND2, m->marioObj->header.gfx.cameraToObject);
        }
    }
}
