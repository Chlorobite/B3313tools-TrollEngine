#include "level_script_headers.h"

void level_cmd_begin_area(void) {
    register u8 areaIndex = CMD_GET(u8, 2);
    register void *geoLayoutAddr = CMD_GET(void *, 4);

    area_geo_load_intercept(areaIndex, (u32*)geoLayoutAddr);

	// REMOVE: area ID check (pointless)
    {
        struct GraphNodeRoot *screenArea =
            (struct GraphNodeRoot *) process_geo_layout(sLevelPool, geoLayoutAddr);
        struct GraphNodeCamera *node = (struct GraphNodeCamera *) screenArea->views[0];

        sCurrAreaIndex = areaIndex;
        screenArea->areaIndex = areaIndex;
        gAreas[areaIndex].unk04 = screenArea;

        if (node != NULL) {
            gAreas[areaIndex].camera = (struct Camera *) node->config.camera;
        } else {
            gAreas[areaIndex].camera = NULL;
        }
    }

    sCurrentCmd = CMD_NEXT;
}
