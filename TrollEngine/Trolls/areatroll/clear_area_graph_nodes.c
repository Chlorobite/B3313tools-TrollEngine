#include "trolle.h"

void clear_area_graph_nodes(void) {
    register s32 i;

    if (gCurrentArea != NULL) {
        geo_call_global_function_nodes(&gCurrentArea->unk04->node, GEO_CONTEXT_AREA_UNLOAD);
        gCurrentArea = NULL;
        gWarpTransition.isActive = FALSE;
    }

    for (i = 0; i < AREA_COUNT; i++) {
        if (gAreaDataNew[i].unk04 != NULL) {
            geo_call_global_function_nodes(&gAreaDataNew[i].unk04->node, GEO_CONTEXT_AREA_INIT);
            gAreaDataNew[i].unk04 = NULL;
        }
    }
}
