#include "mario_headers.h"

/**
 * Determines if a surface is a slope based on the surface class.
 */
s32 mario_floor_is_slope(struct MarioState *m) {
    f32 normY;

    if ((m->area->terrainType & TERRAIN_MASK) == TERRAIN_SLIDE
        && m->floor->normal.y < 0.9998477f) { // ~cos(1 deg)
        return TRUE;
    }

    switch (mario_get_floor_class(m)) {
        case SURFACE_VERY_SLIPPERY:
            normY = 0.9961947f; // ~cos(5 deg)
            break;

        case SURFACE_SLIPPERY:
            normY = 0.9848077f; // ~cos(10 deg)
            break;

        default:
            normY = 0.9659258f; // ~cos(15 deg)
            break;

        case SURFACE_NOT_SLIPPERY:
            normY = 0.9396926f; // ~cos(20 deg)
            break;
    }

    return m->floor->normal.y <= normY;
}
