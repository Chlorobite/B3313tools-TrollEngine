#include "bhv_headers.h"

void bhv_fish_group_loop(void) {
    if (gGlobalTimer & 1) {
        spawn_object(o, MODEL_WHITE_PARTICLE_SMALL, bhvSmallParticleBubbles);
    }
}
