#include "mario_step_headers.h"

u32 mario_update_quicksand(struct MarioState *m, f32 sinkingSpeed) {
    if (m->action & ACT_FLAG_RIDING_SHELL) {
        m->quicksandDepth = 0.0f;
    } else {
        if (m->quicksandDepth < 1.1f) {
            m->quicksandDepth = 1.1f;
        }

        switch (m->floor->type) {
            case SURFACE_SHALLOW_QUICKSAND:
                if ((m->quicksandDepth += sinkingSpeed) >= 10.0f) {
                    m->quicksandDepth = 10.0f;
                }
                break;

            case SURFACE_SHALLOW_MOVING_QUICKSAND:
                if ((m->quicksandDepth += sinkingSpeed) >= 25.0f) {
                    m->quicksandDepth = 25.0f;
                }
                break;

            case SURFACE_QUICKSAND:
            case SURFACE_MOVING_QUICKSAND:
                if ((m->quicksandDepth += sinkingSpeed) >= 60.0f) {
                    m->quicksandDepth = 60.0f;
                }
                break;

            case SURFACE_DEEP_QUICKSAND:
            case SURFACE_DEEP_MOVING_QUICKSAND:
                if ((m->quicksandDepth += sinkingSpeed) >= 160.0f) {
                    update_mario_sound_and_camera(m);
                    return drop_and_set_mario_action(m, ACT_QUICKSAND_DEATH, 0);
                }
                break;

            case SURFACE_INSTANT_QUICKSAND:
            case SURFACE_INSTANT_MOVING_QUICKSAND:
                update_mario_sound_and_camera(m);
                return drop_and_set_mario_action(m, ACT_QUICKSAND_DEATH, 0);
                break;

            default:
                m->quicksandDepth = 0.0f;
                break;
        }
    }

    return FALSE;
}
