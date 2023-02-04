#include <ultra64.h>
#ifdef NO_SEGMENTED_MEMORY
#include <string.h>
#endif

#include "prevent_bss_reordering.h"
#include "trolle.h"
#include "audio/external.h"
#include "buffers/framebuffers.h"
#include "buffers/zbuffer.h"
#include "game/mario.h"
#include "game/memory.h"
#include "game/object_helpers.h"
#include "game/profiler.h"
#include "game/save_file.h"
#include "game/sound_init.h"
#include "goddard/renderer.h"
#include "engine/graph_node.h"
#include "engine/level_script.h"
#include "level_misc_macros.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"

struct LevelCommand {
    /*00*/ u8 type;
    /*01*/ u8 size;
    /*02*/ // variable sized argument data
};

extern struct AllocOnlyPool *sLevelPool;
extern struct Area gAreaDataNew[];
extern struct LevelCommand *sCurrentCmd;

#define CMD_GET(type, offset) (*(type *) (CMD_PROCESS_OFFSET(offset) + (u8 *) sCurrentCmd))

// These are equal
#define CMD_NEXT ((struct LevelCommand *) ((u8 *) sCurrentCmd + (sCurrentCmd->size << CMD_SIZE_SHIFT)))
#define NEXT_CMD ((struct LevelCommand *) ((sCurrentCmd->size << CMD_SIZE_SHIFT) + (u8 *) sCurrentCmd))


void level_cmd_free_level_pool(void) {
    register s32 i;

    load_stuff_in_level_pool();

    alloc_only_pool_resize(sLevelPool, sLevelPool->usedSpace);
    sLevelPool = NULL;

    for (i = 0; i < AREA_COUNT; i++) {
        if (gAreaDataNew[i].terrainData != NULL) {
            alloc_surface_pools();
            break;
        }
    }

    sCurrentCmd = CMD_NEXT;
}
