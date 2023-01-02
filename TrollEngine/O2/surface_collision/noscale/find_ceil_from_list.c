#include "../surface_collision_headers.h"

struct Surface *find_ceil_from_list(struct SurfaceNode *surfaceNode, s32 x, s32 y, s32 z, f32 *pheight) {
    register struct Surface *surf;
    register s32 x1, z1, x2, z2, x3, z3;
    register s32 gost = can_pass_through_walls();
    register f32 height;
    register f32 x_f = x;
    register f32 y_f = y;
    register f32 z_f = z;
    register struct Surface *ceil = NULL;

    // Stay in this loop until out of ceilings.
    while (surfaceNode != NULL) {
        surf = surfaceNode->surface;
        surfaceNode = surfaceNode->next;

        x1 = surf->vertex1[0];
        x2 = surf->vertex2[0];
        x3 = surf->vertex3[0];
        
        // Check that the point is within the triangle bounds.
        if ((x1 < x) == (x2 < x) && (x1 < x) == (x3 < x)) continue;
        
        z1 = surf->vertex1[2];
        z2 = surf->vertex2[2];
        z3 = surf->vertex3[2];
        
        if ((z1 < z) == (z2 < z) && (z1 < z) == (z3 < z)) continue;

        // abusing o2: it won't fully calculate height here,
        // it'll just sprinkle in instructions where otherwise there would be NOPs
		height = -(x_f * surf->normal.x + z_f * surf->normal.z + surf->originOffset) / surf->normal.y;
        if ((z1 - z) * (x2 - x1) > (x1 - x) * (z2 - z1)) continue;
        if ((z2 - z) * (x3 - x2) > (x2 - x) * (z3 - z2)) continue;
        if ((z3 - z) * (x1 - x3) > (x3 - x) * (z1 - z3)) continue;
        
        // Determine if we are checking for the camera or not.
        if (!gCheckingSurfaceCollisionsForCamera) {
            // If we are not checking for the camera, ignore camera only floors.
            if (surf->type == SURFACE_CAMERA_BOUNDARY) continue;
            
            // If an object can pass through a vanish cap wall, pass through.
            if (surf->type == SURFACE_VANISH_CAP_WALLS && gost) continue;
        }
        else if (surf->flags & SURFACE_FLAG_NO_CAM_COLLISION) continue;

		// Checks for ceiling interaction with a 78 unit buffer.
		//! (Exposed Ceilings) Because any point above a ceiling counts
		//  as interacting with a ceiling, ceilings far below can cause
		// "invisible walls" that are really just exposed ceilings.
		if (y_f - height > 78.0f) continue;
        if (height > *pheight) continue;

		*pheight = height;
		ceil = surf;
    }

    //! (Surface Cucking) Since only the first ceil is returned and not the lowest,
    //  lower ceilings can be "cucked" by higher ceilings.
    return ceil;
}
