#include <ultra64.h>
#ifdef NO_SEGMENTED_MEMORY
#include <string.h>
#endif

#include "sm64.h"
#include "audio/external.h"
#include "buffers/framebuffers.h"
#include "buffers/zbuffer.h"
#include "game/area.h"
#include "game/game_init.h"
#include "game/mario.h"
#include "game/memory.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/profiler.h"
#include "game/save_file.h"
#include "game/sound_init.h"
#include "goddard/renderer.h"
#include "engine/geo_layout.h"
#include "engine/graph_node.h"
#include "engine/level_script.h"
#include "level_misc_macros.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"

#include "AI/personalization_helpers.h"
#include "AI/post_object_load_pass.h"

#define CMD_GET(type, offset) (*(type *) (CMD_PROCESS_OFFSET(offset) + (u8 *) sCurrentCmd))

// These are equal
#define CMD_NEXT ((struct LevelCommand *) ((u8 *) sCurrentCmd + (sCurrentCmd->size << CMD_SIZE_SHIFT)))
#define NEXT_CMD ((struct LevelCommand *) ((sCurrentCmd->size << CMD_SIZE_SHIFT) + (u8 *) sCurrentCmd))

struct LevelCommand {
    /*00*/ u8 type;
    /*01*/ u8 size;
    /*02*/ // variable sized argument data
};

enum ScriptStatus { SCRIPT_RUNNING = 1, SCRIPT_PAUSED = 0, SCRIPT_PAUSED2 = -1 };

extern uintptr_t sStack[32];

extern struct AllocOnlyPool *sLevelPool;

extern u16 sDelayFrames;
extern u16 sDelayFrames2;

extern s16 sCurrAreaIndex;

extern uintptr_t *sStackTop;
extern uintptr_t *sStackBase;

extern s16 sScriptStatus;
extern s32 sRegister;
extern struct LevelCommand *sCurrentCmd;
