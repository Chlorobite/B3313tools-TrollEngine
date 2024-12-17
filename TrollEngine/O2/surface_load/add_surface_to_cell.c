#include "surface_load_headers.h"

f32 absf(f32 a);

void add_surface_to_cell(s32 dynamic, s16 cellX, s16 cellZ, struct Surface *surface) {
    register struct SurfaceNode *cell;
    register f32 normalY = surface->normal.y;

    if (dynamic) {
#ifdef TROLLDEBUG
        dynamicSurfaceTris++;
#endif
        cell = gDynamicSurfacePartition[cellZ][cellX];
    } else {
        cell = gStaticSurfacePartition[cellZ][cellX];
    }

    if (normalY < -0.05f) {
        register struct SurfaceNode *newNode = alloc_surface_node();
        register struct SurfaceNode *list = &cell[SPATIAL_PARTITION_CEILS];
        newNode->surface = surface;
        newNode->next = list->next;
        list->next = newNode;
    }
    
    if (normalY > 0.f) {
        register struct SurfaceNode *newNode = alloc_surface_node();
        register struct SurfaceNode *list = &cell[SPATIAL_PARTITION_FLOORS];
        newNode->surface = surface;
        newNode->next = list->next;
        list->next = newNode;
    }
    
    if (absf(normalY) <= 0.05f) {
        register struct SurfaceNode *newNode = alloc_surface_node();
        register struct SurfaceNode *list = &cell[SPATIAL_PARTITION_WALLS];
        newNode->surface = surface;
        newNode->next = list->next;
        list->next = newNode;
        
        if (absf(surface->normal.x) > 0.707f)
            surface->flags |= SURFACE_FLAG_X_PROJECTION;
    }
}
