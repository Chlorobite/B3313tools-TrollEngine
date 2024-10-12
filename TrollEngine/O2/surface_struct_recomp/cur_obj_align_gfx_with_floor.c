#include "object_helpers_headers.h"

void cur_obj_align_gfx_with_floor(void) {
    struct Surface *floor;
    Vec3f floorNormal;
    Vec3f position;

    position[0] = o->oPosX;
    position[1] = o->oPosY;
    position[2] = o->oPosZ;

    find_floor(position[0], position[1], position[2], &floor);
    if (floor != NULL) {
        floorNormal[0] = floor->normal.x;
        floorNormal[1] = floor->normal.y;
        floorNormal[2] = floor->normal.z;

        mtxf_align_terrain_normal(o->transform, floorNormal, position, o->oFaceAngleYaw);
        o->header.gfx.throwMatrix = &o->transform;
    }
}
