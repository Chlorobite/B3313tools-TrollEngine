#include "../mario_headers.h"

#include "AI/stats_tracking.h"
#include "AI/personalization_helpers.h"


void update_mario_health(struct MarioState *m) {
    s32 terrainIsSnow;

    if (m->health >= 0x100) {
        // When already healing or hurting Mario, Mario's HP is not changed any more here.
        if (((u32) m->healCounter | (u32) m->hurtCounter) == 0) {
            if ((m->input & INPUT_IN_POISON_GAS) && !(m->action & ACT_FLAG_INTANGIBLE)) {
                if (!(m->flags & MARIO_METAL_CAP)) {
                    m->health -= 4;
                }
            } else if ((m->action & ACT_FLAG_SWIMMING) && !(m->action & ACT_FLAG_INTANGIBLE)) {
                terrainIsSnow = (m->area->terrainType & TERRAIN_MASK) == TERRAIN_SNOW;

                // When Mario is near the water surface, recover health (unless in snow),
                // when in snow terrains lose 3 health.
                // If using the debug level select, do not lose any HP to water.
                if ((m->pos[1] >= (m->waterLevel - 140)) && !terrainIsSnow) {
                    m->health += 0x1A;
                } else {
                    m->health -= (terrainIsSnow ? 3 : 1);
                }
            }
            else if (TRACKER_difficulty_modifier <= 1.5f) {
                // betaheal
                gMarioState->health += 1;
            }
        }

        if (m->healCounter > 0) {
            m->health += 0x40;
            m->healCounter--;
        }
        if (m->hurtCounter > 0) {
            m->health -= 0x40;
            m->hurtCounter--;
        }

        if (m->health > 0x880) {
            m->health = 0x880;
        }
        if (m->health < 0x100) {
            m->health = 0xFF;
        }
    }
}
