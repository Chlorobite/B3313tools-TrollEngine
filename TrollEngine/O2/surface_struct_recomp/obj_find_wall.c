#include "obj_behaviors_headers.h"

/**
 * Finds any wall collisions, applies them, and turns away from the surface.
 */
s8 obj_find_wall(f32 objNewX, f32 objY, f32 objNewZ, f32 objVelX, f32 objVelZ) {
    struct WallCollisionData hitbox;
    f32 wall_nX, wall_nY, wall_nZ, objVelXCopy, objVelZCopy, objYawX, objYawZ;

    hitbox.x = objNewX;
    hitbox.y = objY;
    hitbox.z = objNewZ;
    hitbox.offsetY = o->hitboxHeight / 2;
    hitbox.radius = o->hitboxRadius;

    if (find_wall_collisions(&hitbox) != 0) {
        o->oPosX = hitbox.x;
        o->oPosY = hitbox.y;
        o->oPosZ = hitbox.z;

        wall_nX = hitbox.walls[0]->normal.x;
        wall_nY = hitbox.walls[0]->normal.y;
        wall_nZ = hitbox.walls[0]->normal.z;

        objVelXCopy = objVelX;
        objVelZCopy = objVelZ;

        // Turns away from the first wall only.
        turn_obj_away_from_surface(objVelXCopy, objVelZCopy, wall_nX, wall_nY, wall_nZ, &objYawX, &objYawZ);

        o->oMoveAngleYaw = atan2s(objYawZ, objYawX);
        return FALSE;
    }

    return TRUE;
}
