#include "camera_headers.h"

void find_mario_floor_and_ceil(struct PlayerGeometry *pg) {
    s16 tempCheckingSurfaceCollisionsForCamera = gCheckingSurfaceCollisionsForCamera;
    //gCheckingSurfaceCollisionsForCamera = TRUE;

    if ((pg->currFloorHeight = find_floor(sMarioCamState->pos[0], sMarioCamState->pos[1] + 10.f,
                   sMarioCamState->pos[2], &pg->currFloor)) != FLOOR_LOWER_LIMIT) {
        pg->currFloorType = pg->currFloor->type;
    } else {
        pg->currFloorType = 0;
    }

    if ((pg->currCeilHeight = find_ceil(sMarioCamState->pos[0], sMarioCamState->pos[1] - 10.f,
                  sMarioCamState->pos[2], &pg->currCeil)) != CELL_HEIGHT_LIMIT) {
        pg->currCeilType = pg->currCeil->type;
    } else {
        pg->currCeilType = 0;
    }

    //gCheckingSurfaceCollisionsForCamera = FALSE;
    /*pg->currFloorHeight = find_floor(sMarioCamState->pos[0],
                                     sMarioCamState->pos[1] + 10.f,
                                     sMarioCamState->pos[2], &pg->currFloor);
    pg->currCeilHeight = find_ceil(sMarioCamState->pos[0],
                                   sMarioCamState->pos[1] - 10.f,
                                   sMarioCamState->pos[2], &pg->currCeil);*/
    pg->waterHeight = find_water_level(sMarioCamState->pos[0], sMarioCamState->pos[2]);
    gCheckingSurfaceCollisionsForCamera = tempCheckingSurfaceCollisionsForCamera;
}
