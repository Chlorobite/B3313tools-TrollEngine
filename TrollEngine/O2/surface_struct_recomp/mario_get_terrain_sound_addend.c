#include "mario_headers.h"

extern s8 sTerrainSounds[7][6];

/**
 * Computes a value that should be added to terrain sounds before playing them.
 * This depends on surfaces and terrain.
 */
u32 mario_get_terrain_sound_addend(struct MarioState *m) {
    s16 floorSoundType;
    s16 terrainType = m->area->terrainType & TERRAIN_MASK;
    s32 ret = SOUND_TERRAIN_DEFAULT << 16;
    s32 floorType;

    if (m->floor != NULL) {
        floorType = m->floor->type;

        if ((gCurrLevelNum != LEVEL_LLL) && (m->floorHeight < (m->waterLevel - 10))) {
            // Water terrain sound, excluding LLL since it uses water in the volcano.
            ret = SOUND_TERRAIN_WATER << 16;
        } else if (SURFACE_IS_QUICKSAND(floorType)) {
            ret = SOUND_TERRAIN_SAND << 16;
        } else {
            // unrolled the switch case to prevent area overflow.
            // god help me.
            floorSoundType = 0;
            
            if (floorType == SURFACE_NOT_SLIPPERY || floorType == SURFACE_HARD || floorType == SURFACE_HARD_NOT_SLIPPERY || floorType == SURFACE_SWITCH)
                floorSoundType = 1;
            else if (floorType == SURFACE_SLIPPERY || floorType == SURFACE_HARD_SLIPPERY || floorType == SURFACE_NO_CAM_COL_SLIPPERY)
                floorSoundType = 2;
            else if (floorType == SURFACE_VERY_SLIPPERY || floorType == SURFACE_ICE || floorType == SURFACE_HARD_VERY_SLIPPERY || floorType == SURFACE_NOISE_VERY_SLIPPERY_73 || floorType == SURFACE_NOISE_VERY_SLIPPERY_74 || floorType == SURFACE_NOISE_VERY_SLIPPERY || floorType == SURFACE_NO_CAM_COL_VERY_SLIPPERY)
                floorSoundType = 3;
            else if (floorType == SURFACE_NOISE_DEFAULT)
                floorSoundType = 4;
            else if (floorType == SURFACE_NOISE_SLIPPERY)
                floorSoundType = 5;
            /*
            switch (floorType) {
                default:
                    break;

                case SURFACE_NOT_SLIPPERY:
                case SURFACE_HARD:
                case SURFACE_HARD_NOT_SLIPPERY:
                case SURFACE_SWITCH:
                    floorSoundType = 1;
                    break;

                case SURFACE_SLIPPERY:
                case SURFACE_HARD_SLIPPERY:
                case SURFACE_NO_CAM_COL_SLIPPERY:
                    floorSoundType = 2;
                    break;

                case SURFACE_VERY_SLIPPERY:
                case SURFACE_ICE:
                case SURFACE_HARD_VERY_SLIPPERY:
                case SURFACE_NOISE_VERY_SLIPPERY_73:
                case SURFACE_NOISE_VERY_SLIPPERY_74:
                case SURFACE_NOISE_VERY_SLIPPERY:
                case SURFACE_NO_CAM_COL_VERY_SLIPPERY:
                    floorSoundType = 3;
                    break;

                case SURFACE_NOISE_DEFAULT:
                    floorSoundType = 4;
                    break;

                case SURFACE_NOISE_SLIPPERY:
                    floorSoundType = 5;
                    break;
            }*/

            ret = sTerrainSounds[terrainType][floorSoundType] << 16;
        }
    }

    return ret;
}
