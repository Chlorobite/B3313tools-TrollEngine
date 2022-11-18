#include "../surface_collision_headers.h"

s32 find_wall_collisions(struct WallCollisionData *colData) {
    struct SurfaceNode *node;
    register s32 cellX, cellZ;
    register s32 numCollisions = 0;
    register s32 x = colData->x;
    register s32 z = colData->z;

    colData->numWalls = 0;

    if (x <= -LEVEL_BOUNDARY_MAX) x = -LEVEL_BOUNDARY_MAX + 1;
    if (x >= LEVEL_BOUNDARY_MAX) x = LEVEL_BOUNDARY_MAX - 1;
    if (z <= -LEVEL_BOUNDARY_MAX) z = -LEVEL_BOUNDARY_MAX + 1;
    if (z >= LEVEL_BOUNDARY_MAX) z = LEVEL_BOUNDARY_MAX - 1;

    // World (level) consists of a 16x16 grid. Find where the collision is on
    // the grid (round toward -inf)
    cellX = ((x + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & NUM_CELLS_INDEX;
    cellZ = ((z + LEVEL_BOUNDARY_MAX) / CELL_SIZE) & NUM_CELLS_INDEX;

    // Check for surfaces belonging to objects.
    node = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    numCollisions += find_wall_collisions_from_list(node, colData);

    // Check for surfaces that are a part of level geometry.
    node = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    numCollisions += find_wall_collisions_from_list(node, colData);

    // Increment the debug tracker.
    //gNumCalls.wall++;

    return numCollisions;
}
