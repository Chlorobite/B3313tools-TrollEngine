#include "../camera_headers.h"
#include "AI/personalization_helpers.h"

s32 is_behind_surface(Vec3f pos, struct Surface *surf) {
    register f32 normX, normY, normZ, dirX, dirY, dirZ, scaleH, scaleV;

	scaleH = levelScaleH;
	scaleV = levelScaleV;
    
    // Surface normal
    normX = (surf->vertexY[1] - surf->vertexY[0]) * (surf->vertexZ[2] - surf->vertexZ[1]) -
                (surf->vertexY[2] - surf->vertexY[1]) * (surf->vertexZ[1] - surf->vertexZ[0]);
    normY = (surf->vertexZ[1] - surf->vertexZ[0]) * (surf->vertexX[2] - surf->vertexX[1]) -
                (surf->vertexZ[2] - surf->vertexZ[1]) * (surf->vertexX[1] - surf->vertexX[0]);
    normZ = (surf->vertexX[1] - surf->vertexX[0]) * (surf->vertexY[2] - surf->vertexY[1]) -
                (surf->vertexX[2] - surf->vertexX[1]) * (surf->vertexY[1] - surf->vertexY[0]);
    dirX = surf->vertexX[0] - pos[0] / scaleH;
    dirY = surf->vertexY[0] - pos[1] / scaleV;
    dirZ = surf->vertexZ[0] - pos[2] / scaleH;

    if (dirX * normX + dirY * normY + dirZ * normZ < 0) {
        return 1;
    }
    return 0;
}
