#include "level_script_headers.h"

void level_cmd_load_model_from_dl(void) {
    register s16 val1 = CMD_GET(s16, 2) & 0x0FFF;
    register s16 val2 = ((u16)CMD_GET(s16, 2)) >> 12;
    register void *val3 = CMD_GET(void *, 4);

    if (val1 < 256) {
        gLoadedGraphNodes[val1] =
            (struct GraphNode *) init_graph_node_display_list(sLevelPool, 0, val2, val3);
        set_model_loaded(val1, TRUE); // AI/personalization_helpers.c
    }

    sCurrentCmd = CMD_NEXT;
}
