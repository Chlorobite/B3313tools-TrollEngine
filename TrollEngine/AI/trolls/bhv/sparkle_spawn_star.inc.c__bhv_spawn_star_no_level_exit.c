#include "bhv_headers.h"


void bhv_spawn_star_no_level_exit(u32 sp20) {
    struct Object *sp1C = spawn_object(o, MODEL_STAR, bhvSpawnedStarNoLevelExit);
    sp1C->oBehParams = ((sp20 & 0xFF) << 24) | (sp20 & 0x0100);
    sp1C->oInteractionSubtype = INT_SUBTYPE_NO_EXIT;
}

