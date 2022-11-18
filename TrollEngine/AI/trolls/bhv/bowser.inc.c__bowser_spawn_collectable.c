#include "bhv_headers.h"

/**
 * Spawn collectable that Bowser spawns after despawning
 * Spawns a Key in BitDW/BitFS or Grand Star in BitS
 */
void bowser_spawn_collectable(void) {
    if (o->oBehParams2ndByte == BOWSER_BP_BITS) {
        gSecondCameraFocus = spawn_object(o, MODEL_STAR, bhvGrandStar);
    } else {
		troll_spawn_bowser_star();
    }
}
