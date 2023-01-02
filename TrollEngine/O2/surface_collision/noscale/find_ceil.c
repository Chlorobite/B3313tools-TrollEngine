#include "../surface_collision_headers.h"

s32 to_cell_pos(s32 pos);

f32 find_ceil(register f32 xPos, register f32 yPos, register f32 zPos, struct Surface **pceil) {
    register s32 cellZ, cellX;

    register struct Surface *ceil, *dynamicCeil;
    register struct SurfaceNode *surfaceList;

    f32 height = CELL_HEIGHT_LIMIT;
    f32 dynamicHeight = CELL_HEIGHT_LIMIT;

    *pceil = NULL;

    // Each level is split into cells to limit load, find the appropriate cell.
    cellX = to_cell_pos((s32) xPos);
    cellZ = to_cell_pos((s32) zPos);

    // Check for surfaces belonging to objects.
    surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_CEILS].next;
    dynamicCeil = find_ceil_from_list(surfaceList, xPos, yPos, zPos, &dynamicHeight);
    
    surfaceList = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_CEILS].next;
    ceil = find_ceil_from_list(surfaceList, xPos, yPos, zPos, &height);

    if (dynamicHeight < height) {
        ceil = dynamicCeil;
        height = dynamicHeight;
    }
    
    if (height == CELL_HEIGHT_LIMIT) {
        height = CELL_HEIGHT_LIMIT;
    }

    *pceil = ceil;

    // Increment the debug tracker.
    //gNumCalls.ceil++;

    return height;
}

s32 to_cell_pos(s32 pos) {
    if (pos <= -LEVEL_BOUNDARY_MAX) pos = -LEVEL_BOUNDARY_MAX + 1;
    if (pos >= LEVEL_BOUNDARY_MAX) pos = LEVEL_BOUNDARY_MAX - 1;
    return ((pos + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & NUM_CELLS_INDEX;
}
