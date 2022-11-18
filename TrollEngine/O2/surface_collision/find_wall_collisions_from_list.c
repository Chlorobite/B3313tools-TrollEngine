#include "surface_collision_headers.h"

extern void handle_nmi_request(void);

s32 find_wall_collisions_from_list(register struct SurfaceNode *surfaceNode,
                                   register struct WallCollisionData *data) {
    register struct Surface *surf;
    register f32 offset;
    register f32 radius = data->radius;
    register f32 x_f = data->x;
    register f32 y_f = data->y + data->offsetY;
    register f32 z_f = data->z;
    register s32 x_i = x_f;
    register s32 y_i = y_f;
    register s32 z_i = z_f;
    register s32 w1, w2, w3;
    register s32 y1, y2, y3;
    register s32 numCols = 0;

    // Max collision radius = 200
    if (radius > 200.0f) radius = 200.0f;

    // Stay in this loop until out of walls.
    while (surfaceNode != NULL) {
        surf = surfaceNode->surface;
        surfaceNode = surfaceNode->next;

        // Exclude a large number of walls immediately to optimize.
        if (y_i < surf->lowerY || y_i > surf->upperY) continue;

        offset = surf->normal.x * x_f + surf->normal.y * y_f + surf->normal.z * z_f + surf->originOffset;

        if (offset < -radius || offset > radius) continue;

        //! (Quantum Tunneling) Due to issues with the vertices walls choose and
        //  the fact they are floating point, certain floating point positions
        //  along the seam of two walls may collide with neither wall or both walls.
        y1 = surf->vertex1[1];
        y2 = surf->vertex2[1];
        y3 = surf->vertex3[1];
        if (surf->flags & SURFACE_FLAG_X_PROJECTION) {
            w1 = -surf->vertex1[2];            w2 = -surf->vertex2[2];            w3 = -surf->vertex3[2];

            if (surf->normal.x > 0.0f) {
                if ((y1 - y_i) * (w2 - w1) > (w1 + z_i) * (y2 - y1)) continue;
                if ((y2 - y_i) * (w3 - w2) > (w2 + z_i) * (y3 - y2)) continue;
                if ((y3 - y_i) * (w1 - w3) > (w3 + z_i) * (y1 - y3)) continue;
            } else {
                if ((y1 - y_i) * (w2 - w1) < (w1 + z_i) * (y2 - y1)) continue;
                if ((y2 - y_i) * (w3 - w2) < (w2 + z_i) * (y3 - y2)) continue;
                if ((y3 - y_i) * (w1 - w3) < (w3 + z_i) * (y1 - y3)) continue;
            }
        } else {
            w1 = surf->vertex1[0];            w2 = surf->vertex2[0];            w3 = surf->vertex3[0];

            if (surf->normal.z > 0.0f) {
                if ((y1 - y_i) * (w2 - w1) > (w1 - x_i) * (y2 - y1)) continue;
                if ((y2 - y_i) * (w3 - w2) > (w2 - x_i) * (y3 - y2)) continue;
                if ((y3 - y_i) * (w1 - w3) > (w3 - x_i) * (y1 - y3)) continue;
            } else {
                if ((y1 - y_i) * (w2 - w1) < (w1 - x_i) * (y2 - y1)) continue;
                if ((y2 - y_i) * (w3 - w2) < (w2 - x_i) * (y3 - y2)) continue;
                if ((y3 - y_i) * (w1 - w3) < (w3 - x_i) * (y1 - y3)) continue;
            }
        }

        // Determine if we are checking for the camera or not.
        if (!gCheckingSurfaceCollisionsForCamera) {
            // Ignore camera only surfaces.
            if (surf->type == SURFACE_CAMERA_BOUNDARY) continue;

            // If an object can pass through a vanish cap wall, pass through.
            if (surf->type == SURFACE_VANISH_CAP_WALLS && gCurrentObject != NULL) {
                // If an object can pass through a vanish cap wall, pass through.
                if (gCurrentObject->activeFlags & ACTIVE_FLAG_MOVE_THROUGH_GRATE) continue;

                // If Mario has a vanish cap, pass through the vanish cap wall.
                if (gCurrentObject == gMarioObject && (gMarioState->flags & MARIO_VANISH_CAP)) continue;
            }
        }
        else if (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION) continue;

        //! (Wall Overlaps) Because this doesn't update the x and z local variables,
        //  multiple walls can push mario more than is required.
        data->x += surf->normal.x * (radius - offset);
        data->z += surf->normal.z * (radius - offset);

        //! (Unreferenced Walls) Since this only returns the first four walls,
        //  this can lead to wall interaction being missed. Typically unreferenced walls
        //  come from only using one wall, however.
        if (data->numWalls < 4) {
            data->walls[data->numWalls++] = surf;
        }

        numCols++;
    }

    return numCols;
}
