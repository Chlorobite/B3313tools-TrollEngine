#include <PR/ultratypes.h>

#include "sm64.h"
#include "game/ingame_menu.h"
#include "engine/graph_node.h"
#include "engine/behavior_script.h"
#include "behavior_data.h"
#include "game/memory.h"
#include "game/object_helpers.h"
#include "game/macro_special_objects.h"
#include "engine/surface_collision.h"
#include "game/mario.h"
#include "game/object_list_processor.h"
#include "engine/surface_load.h"

struct Surface *alloc_surface(void);
struct SurfaceNode *alloc_surface_node(void);
struct Surface *read_surface_data(s16 *vertexData, s16 **vertexIndices);

extern s32 dynamicSurfaceTris;

#define SURFACE_DEBUG
