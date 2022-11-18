#include "../camera_headers.h"

s32 collide_with_walls(Vec3f pos, f32 offsetY, f32 radius) {
    struct WallCollisionData collisionData;
    register struct Surface *wall = NULL;
    register f32 normX, normY, normZ;
    register f32 originOffset;
    register f32 offset, offsetAbsolute;
    Vec3f newPos[4];
    register s32 i;
    s32 numCollisions = 0;

    collisionData.x = pos[0];
    collisionData.y = pos[1];
    collisionData.z = pos[2];
    collisionData.radius = radius;
    collisionData.offsetY = offsetY;
    numCollisions = find_wall_collisions(&collisionData);
    if (numCollisions != 0) {
        for (i = 0; i < collisionData.numWalls; i++) {
            wall = collisionData.walls[collisionData.numWalls - 1];
            vec3f_copy(newPos[i], pos);
            normX = wall->normal.x;
            normY = wall->normal.y;
            normZ = wall->normal.z;
            originOffset = wall->originOffset;
            offset = normX * newPos[i][0] + normY * newPos[i][1] + normZ * newPos[i][2] + originOffset;
            offsetAbsolute = ABS2(offset);
            if (offsetAbsolute < radius) {
                newPos[i][0] += normX * (radius - offset);
                newPos[i][2] += normZ * (radius - offset);
                vec3f_copy(pos, newPos[i]);
            }
        }
    }
    return numCollisions;
}
