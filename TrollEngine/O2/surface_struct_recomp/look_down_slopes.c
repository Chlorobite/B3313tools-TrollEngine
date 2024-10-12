#include "camera_headers.h"

/**
 * Pitch the camera down when the camera is facing down a slope
 */
s16 look_down_slopes(s16 camYaw) {
    struct Surface *floor;
    f32 floorDY;
    // Default pitch
    s16 pitch = 0x05B0;
    // x and z offsets towards the camera
    f32 xOff = sMarioCamState->pos[0] + sins(camYaw) * 40.f;
    f32 zOff = sMarioCamState->pos[2] + coss(camYaw) * 40.f;

    floorDY = find_floor(xOff, sMarioCamState->pos[1], zOff, &floor) - sMarioCamState->pos[1];

    if (floor != NULL) {
        if (floor->type != SURFACE_WALL_MISC && floorDY > 0) {
            if (floor->normal.z == 0.f && floorDY < 100.f) {
                pitch = 0x05B0;
            } else {
                // Add the slope's angle of declination to the pitch
                pitch += atan2s(40.f, floorDY);
            }
        }
    }

    return pitch;
}
