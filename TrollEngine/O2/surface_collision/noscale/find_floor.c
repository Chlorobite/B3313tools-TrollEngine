#include "../surface_collision_headers.h"

f32 find_floor(f32 xPos, f32 yPos, f32 zPos, struct Surface **pfloor) {
    register s32 cellZ, cellX;

    struct Surface *floor, *dynamicFloor;
    struct SurfaceNode *surfaceList;

    f32 height = FLOOR_LOWER_LIMIT;
    f32 dynamicHeight = FLOOR_LOWER_LIMIT;
	
	register s32 x, y, z;
	
	//xPos /= 1.0f;
	//zPos /= 1.0f;

    //! (Parallel Universes) Because position is casted to an s16, reaching higher
    //  float locations can return floors despite them not existing there.
    //  (Dynamic floors will unload due to the range.)
    x = (s32) xPos;
    y = (s32) yPos;
    z = (s32) zPos;

    *pfloor = NULL;

    if (x <= -LEVEL_BOUNDARY_MAX) x = -LEVEL_BOUNDARY_MAX + 1;
    if (x >= LEVEL_BOUNDARY_MAX) x = LEVEL_BOUNDARY_MAX - 1;
    if (z <= -LEVEL_BOUNDARY_MAX) z = -LEVEL_BOUNDARY_MAX + 1;
    if (z >= LEVEL_BOUNDARY_MAX) z = LEVEL_BOUNDARY_MAX - 1;

    // Each level is split into cells to limit load, find the appropriate cell.
    cellX = ((x + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & NUM_CELLS_INDEX;
    cellZ = ((z + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & NUM_CELLS_INDEX;

    // Check for surfaces belonging to objects.
    surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_FLOORS].next;
    dynamicFloor = find_floor_from_list(surfaceList, xPos, yPos, zPos, &dynamicHeight);

    // Check for surfaces that are a part of level geometry.
    surfaceList = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_FLOORS].next;
    floor = find_floor_from_list(surfaceList, xPos, yPos, zPos, &height);

    // To prevent the Merry-Go-Round room from loading when Mario passes above the hole that leads
    // there, SURFACE_INTANGIBLE is used. This prevent the wrong room from loading, but can also allow
    // Mario to pass through.
    if (!gFindFloorIncludeSurfaceIntangible) {
        //! (BBH Crash) Most NULL checking is done by checking the height of the floor returned
        //  instead of checking directly for a NULL floor. If this check returns a NULL floor
        //  (happens when there is no floor under the SURFACE_INTANGIBLE floor) but returns the height
        //  of the SURFACE_INTANGIBLE floor instead of the typical -11000 returned for a NULL floor.
        if (floor != NULL && floor->type == SURFACE_INTANGIBLE) {
            floor = find_floor_from_list(surfaceList, x, (s32)(height - 200.0f), z, &height);
        }
    } else {
        // To prevent accidentally leaving the floor tangible, stop checking for it.
        gFindFloorIncludeSurfaceIntangible = FALSE;
    }

    // If a floor was missed, increment the debug counter.
    //if (floor == NULL) {
    //    gNumFindFloorMisses++;
    //}

    if (dynamicHeight > height) {
        floor = dynamicFloor;
        height = dynamicHeight;
    }

    *pfloor = floor;

    // Increment the debug tracker.
    //gNumCalls.floor++;

    return height;
}
