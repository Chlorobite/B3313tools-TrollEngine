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

void load_static_surfaces(s16 **data, s16 *vertexData, s16 surfaceType, s8 **surfaceRooms) {
    s32 i;
    s32 numSurfaces;
    struct Surface *surface;
    s8 room = 0;
    s16 hasForce = surface_has_force(surfaceType);
    s16 flags = surf_has_no_cam_collision(surfaceType);

    numSurfaces = *(*data);
    (*data)++;

    for (i = 0; i < numSurfaces; i++) {
        if (*surfaceRooms != NULL) {
            room = *(*surfaceRooms);
            (*surfaceRooms)++;
        }

        surface = read_surface_data(vertexData, data);
        if (surface != NULL) {
            surface->room = room;
            surface->type = surfaceType;
            surface->flags = (s8) flags;

            if (hasForce) {
                surface->force = *(*data + 3);
            } else {
                surface->force = 0;
            }

            add_surface(surface, FALSE);
        }

        *data += 3;
        if (hasForce) {
            (*data)++;
        }
    }
}
