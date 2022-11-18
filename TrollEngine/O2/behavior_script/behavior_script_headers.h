#include <ultra64.h>

#include "sm64.h"
#include "behavior_data.h"
#include "engine/behavior_script.h"
#include "game/area.h"
#include "game/behavior_actions.h"
#include "game/game_init.h"
#include "game/mario.h"
#include "game/memory.h"
#include "game/obj_behaviors_2.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "engine/graph_node.h"
#include "engine/surface_collision.h"

typedef s32 (*BhvCommandProc)(void);
extern BhvCommandProc BehaviorCmdTable[];
