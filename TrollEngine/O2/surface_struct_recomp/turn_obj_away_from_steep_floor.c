#include "obj_behaviors_headers.h"

/**
 * Turns an object away from steep floors, similarly to walls.
 */
s8 turn_obj_away_from_steep_floor(struct Surface *objFloor, f32 floorY, f32 objVelX, f32 objVelZ) {
    f32 floor_nX, floor_nY, floor_nZ, objVelXCopy, objVelZCopy, objYawX, objYawZ;

    if (objFloor == NULL) {
        //! (OOB Object Crash) TRUNC overflow exception after 36 minutes
        o->oMoveAngleYaw += 32768.f;
        return FALSE;
    }

    floor_nX = objFloor->normal.x;
    floor_nY = objFloor->normal.y;
    floor_nZ = objFloor->normal.z;

    // If the floor is steep and we are below it (i.e. walking into it), turn away from the floor.
    if (floor_nY < 0.5 && floorY > o->oPosY) {
        objVelXCopy = objVelX;
        objVelZCopy = objVelZ;
        turn_obj_away_from_surface(objVelXCopy, objVelZCopy, floor_nX, floor_nY, floor_nZ, &objYawX,
                               &objYawZ);
        o->oMoveAngleYaw = atan2s(objYawZ, objYawX);
        return FALSE;
    }

    return TRUE;
}
