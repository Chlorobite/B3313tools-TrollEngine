#include "mario_headers.h"

/**
 * Returns the slipperiness class of Mario's floor.
 */
s32 mario_get_floor_class(register struct MarioState *m) {
    register s32 floorClass = SURFACE_CLASS_DEFAULT;

    // The slide terrain type defaults to slide slipperiness.
    // This doesn't matter too much since normally the slide terrain
    // is checked for anyways.
    if ((m->area->terrainType & TERRAIN_MASK) == TERRAIN_SLIDE) {
        floorClass = SURFACE_CLASS_VERY_SLIPPERY;
    }

    if (m->floor != NULL) {
        // unrolled the switch case to prevent area overflow.
        // god help me.
        register s32 floorType = m->floor->type;
        
        if (floorType == SURFACE_NOT_SLIPPERY || floorType == SURFACE_HARD_NOT_SLIPPERY || floorType == SURFACE_SWITCH)
            floorClass = SURFACE_CLASS_NOT_SLIPPERY;
        else if (floorType == SURFACE_SLIPPERY || floorType == SURFACE_NOISE_SLIPPERY || floorType == SURFACE_HARD_SLIPPERY || floorType == SURFACE_NO_CAM_COL_SLIPPERY)
            floorClass = SURFACE_CLASS_SLIPPERY;
        else if (floorType == SURFACE_VERY_SLIPPERY || floorType == SURFACE_ICE || floorType == SURFACE_HARD_VERY_SLIPPERY || floorType == SURFACE_NOISE_VERY_SLIPPERY_73 || floorType == SURFACE_NOISE_VERY_SLIPPERY_74 || floorType == SURFACE_NOISE_VERY_SLIPPERY || floorType == SURFACE_NO_CAM_COL_VERY_SLIPPERY)
            floorClass = SURFACE_CLASS_VERY_SLIPPERY;
        /*switch (m->floor->type) {
            case SURFACE_NOT_SLIPPERY:
            case SURFACE_HARD_NOT_SLIPPERY:
            case SURFACE_SWITCH:
                floorClass = SURFACE_CLASS_NOT_SLIPPERY;
                break;

            case SURFACE_SLIPPERY:
            case SURFACE_NOISE_SLIPPERY:
            case SURFACE_HARD_SLIPPERY:
            case SURFACE_NO_CAM_COL_SLIPPERY:
                floorClass = SURFACE_CLASS_SLIPPERY;
                break;

            case SURFACE_VERY_SLIPPERY:
            case SURFACE_ICE:
            case SURFACE_HARD_VERY_SLIPPERY:
            case SURFACE_NOISE_VERY_SLIPPERY_73:
            case SURFACE_NOISE_VERY_SLIPPERY_74:
            case SURFACE_NOISE_VERY_SLIPPERY:
            case SURFACE_NO_CAM_COL_VERY_SLIPPERY:
                floorClass = SURFACE_CLASS_VERY_SLIPPERY;
                break;
        }*/
    }

    // Crawling allows Mario to not slide on certain steeper surfaces.
    if (m->action == ACT_CRAWLING && m->floor->normal.y > 0.5f && floorClass == SURFACE_CLASS_DEFAULT) {
        floorClass = SURFACE_CLASS_NOT_SLIPPERY;
    }

    return floorClass;
}
