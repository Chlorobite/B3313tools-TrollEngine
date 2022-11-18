#include "../camera_headers.h"

s32 is_behind_surface(Vec3f pos, struct Surface *surf) {
    register f32 normX, normY, normZ, dirX, dirY, dirZ;
    
    // Surface normal
    normX = (surf->vertex2[1] - surf->vertex1[1]) * (surf->vertex3[2] - surf->vertex2[2]) -
                (surf->vertex3[1] - surf->vertex2[1]) * (surf->vertex2[2] - surf->vertex1[2]);
    normY = (surf->vertex2[2] - surf->vertex1[2]) * (surf->vertex3[0] - surf->vertex2[0]) -
                (surf->vertex3[2] - surf->vertex2[2]) * (surf->vertex2[0] - surf->vertex1[0]);
    normZ = (surf->vertex2[0] - surf->vertex1[0]) * (surf->vertex3[1] - surf->vertex2[1]) -
                (surf->vertex3[0] - surf->vertex2[0]) * (surf->vertex2[1] - surf->vertex1[1]);
    dirX = surf->vertex1[0] - pos[0];
    dirY = surf->vertex1[1] - pos[1];
    dirZ = surf->vertex1[2] - pos[2];

    if (dirX * normX + dirY * normY + dirZ * normZ < 0) {
        return 1;
    }
    return 0;
}
