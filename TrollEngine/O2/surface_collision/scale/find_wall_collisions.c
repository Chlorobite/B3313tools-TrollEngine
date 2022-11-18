#include "../surface_collision_headers.h"
#include "AI/personalization_helpers.h"

s32 to_cell_pos(s32 pos);

s32 find_wall_collisions(struct WallCollisionData *colData) {
    struct SurfaceNode *node;
    register s32 cellX, cellZ;
    register s32 numCollisions = 0;
    
    colData->x /= levelScaleH;
    colData->y /= levelScaleV;
    colData->z /= levelScaleH;
    colData->radius /= levelScaleH;
    colData->offsetY /= levelScaleV;

    colData->numWalls = 0;

    // World (level) consists of a 16x16 grid. Find where the collision is on
    // the grid (round toward -inf)
    cellX = to_cell_pos(colData->x);
    cellZ = to_cell_pos(colData->z);

    // Check for surfaces belonging to objects.
    node = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    numCollisions += find_wall_collisions_from_list(node, colData);

    // Check for surfaces that are a part of level geometry.
    node = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    numCollisions += find_wall_collisions_from_list(node, colData);
    
    colData->x *= levelScaleH;
    colData->y *= levelScaleV;
    colData->z *= levelScaleH;
    colData->radius *= levelScaleH;
    colData->offsetY *= levelScaleV;

    // Increment the debug tracker.
    //gNumCalls.wall++;

    return numCollisions;
}
