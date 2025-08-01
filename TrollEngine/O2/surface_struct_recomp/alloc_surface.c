#include <PR/ultratypes.h>

#include "sm64.h"
#include "game/ingame_menu.h"
#include "engine/behavior_script.h"
#include "behavior_data.h"
#include "game/memory.h"
#include "game/object_helpers.h"
#include "game/macro_special_objects.h"
#include "engine/surface_collision.h"
#include "game/mario.h"
#include "game/object_list_processor.h"
#include "engine/surface_load.h"

/**
 * Pools of data to contain either surface nodes or surfaces.
 */
extern struct SurfaceNode *sSurfaceNodePool;
extern struct Surface *sSurfacePool;

struct Surface *alloc_surface(void) {
    struct Surface *surface = &sSurfacePool[gSurfacesAllocated];
    gSurfacesAllocated++;

    surface->type = 0;
    surface->force = 0;
    surface->flags = 0;
    surface->room = 0;
    surface->object = NULL;

    return surface;
}
