#include "mario_headers.h"

/**
 * Determines if a surface is steep based on the surface class.
 */
s32 mario_floor_is_steep(struct MarioState *m) {
    f32 normY;
    s32 result = FALSE;

    // Interestingly, this function does not check for the
    // slide terrain type. This means that steep behavior persists for
    // non-slippery and slippery surfaces.
    // This does not matter in vanilla game practice.
    if (!mario_facing_downhill(m, FALSE)) {
        switch (mario_get_floor_class(m)) {
            case SURFACE_VERY_SLIPPERY:
                normY = 0.9659258f; // ~cos(15 deg)
                break;

            case SURFACE_SLIPPERY:
                normY = 0.9396926f; // ~cos(20 deg)
                break;

            default:
                normY = 0.8660254f; // ~cos(30 deg)
                break;

            case SURFACE_NOT_SLIPPERY:
                normY = 0.8660254f; // ~cos(30 deg)
                break;
        }

        result = m->floor->normal.y <= normY;
    }

    return result;
}
