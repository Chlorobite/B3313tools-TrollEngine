#include "level_script_headers.h"

void level_cmd_load_model_from_geo(void) {
    register s16 arg0 = CMD_GET(s16, 2);
    register void *arg1 = CMD_GET(void *, 4);

    if (arg0 < 256) {
        gLoadedGraphNodes[arg0] = process_geo_layout(sLevelPool, arg1);
        set_model_loaded(arg0, TRUE); // AI/personalization_helpers.c
    }

    sCurrentCmd = CMD_NEXT;
}
