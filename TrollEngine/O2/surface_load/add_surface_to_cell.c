#include "surface_load_headers.h"

f32 absf(f32 a);

void add_surface_to_cell(s32 dynamic, s16 cellX, s16 cellZ, struct Surface *surface) {
    register struct SurfaceNode *newNode = alloc_surface_node();
    register struct SurfaceNode *list;
    register s32 priority;
    register s32 sortDir;
    register s32 listIndex = SPATIAL_PARTITION_FLOORS;

    if (surface->normal.y <= 0.01f) {
        if (surface->normal.y < -0.01f) {
            listIndex = SPATIAL_PARTITION_CEILS;
        } else {
            listIndex = SPATIAL_PARTITION_WALLS;

            if (absf(surface->normal.x) > 0.707f)
                surface->flags |= SURFACE_FLAG_X_PROJECTION;
        }
    }

    newNode->surface = surface;

    if (dynamic) {
#ifdef SURFACE_DEBUG
        dynamicSurfaceTris++;
#endif
        list = &gDynamicSurfacePartition[cellZ][cellX][listIndex];
    } else {
        list = &gStaticSurfacePartition[cellZ][cellX][listIndex];
    }

    newNode->next = list->next;
    list->next = newNode;
}
