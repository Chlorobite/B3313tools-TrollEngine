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

s16 min_3(s16 a0, s16 a1, s16 a2);
s16 max_3(s16 a0, s16 a1, s16 a2);
s16 lower_cell_index(s16 coord);
s16 upper_cell_index(s16 coord);

void add_surface(register struct Surface *surface, register s32 dynamic) {
    register s32 minCellX, minCellZ, maxCellX, maxCellZ;

    register s32 cellZ, cellX;

    minCellX = lower_cell_index(min_3(surface->vertexX[0], surface->vertexX[1], surface->vertexX[2]));
    maxCellX = upper_cell_index(max_3(surface->vertexX[0], surface->vertexX[1], surface->vertexX[2]));
    minCellZ = lower_cell_index(min_3(surface->vertexZ[0], surface->vertexZ[1], surface->vertexZ[2]));
    maxCellZ = upper_cell_index(max_3(surface->vertexZ[0], surface->vertexZ[1], surface->vertexZ[2]));

    for (cellZ = minCellZ; cellZ <= maxCellZ; cellZ++) {
        for (cellX = minCellX; cellX <= maxCellX; cellX++) {
            add_surface_to_cell(dynamic, cellX, cellZ, surface);
        }
    }
}
