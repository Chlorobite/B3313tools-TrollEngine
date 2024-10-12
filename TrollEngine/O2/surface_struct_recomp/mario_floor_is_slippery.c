#include "mario_headers.h"

/**
 * Determines if a surface is slippery based on the surface class.
 */
u32 mario_floor_is_slippery(struct MarioState *m) {
    f32 normY;

    if ((m->area->terrainType & TERRAIN_MASK) == TERRAIN_SLIDE
        && m->floor->normal.y < 0.9998477f //~cos(1 deg)
    ) {
        return TRUE;
    }

    switch (mario_get_floor_class(m)) {
        case SURFACE_VERY_SLIPPERY:
            normY = 0.9848077f; //~cos(10 deg)
            break;

        case SURFACE_SLIPPERY:
            normY = 0.9396926f; //~cos(20 deg)
            break;

        default:
            normY = 0.7880108f; //~cos(38 deg)
            break;

        case SURFACE_NOT_SLIPPERY:
            normY = 0.0f;
            break;
    }

    return m->floor->normal.y <= normY;
}
