#include "level_script_headers.h"

void level_cmd_create_instant_warp(void) {
    register s32 i;
    register struct InstantWarp *warp;

    if (sCurrAreaIndex != -1) {
        if (gAreas[sCurrAreaIndex].instantWarps == NULL) {
            gAreas[sCurrAreaIndex].instantWarps =
                alloc_only_pool_alloc(sLevelPool, 4 * sizeof(struct InstantWarp));

            for (i = INSTANT_WARP_INDEX_START; i < INSTANT_WARP_INDEX_STOP; i++) {
                gAreas[sCurrAreaIndex].instantWarps[i].id = 0;
            }
        }

        warp = gAreas[sCurrAreaIndex].instantWarps + CMD_GET(u8, 2);

        warp->id = 1;
        warp->area = CMD_GET(u8, 3);

        warp->displacement[0] = CMD_GET(s16, 4);
        warp->displacement[1] = CMD_GET(s16, 6);
        warp->displacement[2] = CMD_GET(s16, 8);
        
		// ADD: level scale
        scale_instant_warp(warp);
		// END ADD
    }

    sCurrentCmd = CMD_NEXT;
}
