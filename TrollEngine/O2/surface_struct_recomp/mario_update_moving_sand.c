#include "mario_step_headers.h"

u32 mario_update_moving_sand(struct MarioState *m) {
    struct Surface *floor = m->floor;
    s32 floorType = floor->type;

    if (floorType == SURFACE_DEEP_MOVING_QUICKSAND || floorType == SURFACE_SHALLOW_MOVING_QUICKSAND
        || floorType == SURFACE_MOVING_QUICKSAND || floorType == SURFACE_INSTANT_MOVING_QUICKSAND) {
        s16 pushAngle = floor->force << 8;
        f32 pushSpeed = sMovingSandSpeeds[floor->force >> 8];

        m->vel[0] += pushSpeed * sins(pushAngle);
        m->vel[2] += pushSpeed * coss(pushAngle);

        return 1;
    }

    return 0;
}
