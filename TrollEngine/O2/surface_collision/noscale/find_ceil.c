#include "../surface_collision_headers.h"

f32 find_ceil(f32 xPos, f32 yPos, f32 zPos, struct Surface **pceil) {
    register s32 cellZ, cellX;

    struct Surface *ceil, *dynamicCeil;
    struct SurfaceNode *surfaceList;

    f32 height = CELL_HEIGHT_LIMIT;
    f32 dynamicHeight = CELL_HEIGHT_LIMIT;
	
	register s32 x, y, z;
	
	//xPos /= 1.0f;
	//zPos /= 1.0f;

    //! (Parallel Universes) Because position is casted to an s16, reaching higher
    //  float locations can return ceilings despite them not existing there.
    //  (Dynamic ceilings will unload due to the range.)
    x = (s32) xPos;
    y = (s32) yPos;
    z = (s32) zPos;

    *pceil = NULL;

    if (x <= -LEVEL_BOUNDARY_MAX) x = -LEVEL_BOUNDARY_MAX + 1;
    if (x >= LEVEL_BOUNDARY_MAX) x = LEVEL_BOUNDARY_MAX - 1;
    if (z <= -LEVEL_BOUNDARY_MAX) z = -LEVEL_BOUNDARY_MAX + 1;
    if (z >= LEVEL_BOUNDARY_MAX) z = LEVEL_BOUNDARY_MAX - 1;

    // Each level is split into cells to limit load, find the appropriate cell.
    cellX = ((x + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & NUM_CELLS_INDEX;
    cellZ = ((z + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & NUM_CELLS_INDEX;

    // Check for surfaces belonging to objects.
    surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_CEILS].next;
    dynamicCeil = find_ceil_from_list(surfaceList, xPos, yPos, zPos, &dynamicHeight);

    // Check for surfaces that are a part of level geometry.
    surfaceList = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_CEILS].next;
    ceil = find_ceil_from_list(surfaceList, xPos, yPos, zPos, &height);

    if (dynamicHeight < height) {
        ceil = dynamicCeil;
        height = dynamicHeight;
    }

    *pceil = ceil;

    // Increment the debug tracker.
    //gNumCalls.ceil++;

    return height;
}
