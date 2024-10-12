#include "mario_actions_automatic_headers.h"

s32 perform_hanging_step(struct MarioState *m, Vec3f nextPos) {
    struct Surface *ceil;
    struct Surface *floor;
    f32 ceilHeight;
    f32 floorHeight;
    f32 ceilOffset;

    m->wall = resolve_and_return_wall_collisions(nextPos, 50.0f, 50.0f);
    floorHeight = find_floor(nextPos[0], nextPos[1], nextPos[2], &floor);
    ceilHeight = vec3f_find_ceil(nextPos, floorHeight, &ceil);

    if (floor == NULL) {
        return HANG_HIT_CEIL_OR_OOB;
    }
    if (ceil == NULL) {
        return HANG_LEFT_CEIL;
    }
    if (ceilHeight - floorHeight <= 160.0f) {
        return HANG_HIT_CEIL_OR_OOB;
    }
    if (ceil->type != SURFACE_HANGABLE) {
        return HANG_LEFT_CEIL;
    }

    ceilOffset = ceilHeight - (nextPos[1] + 160.0f);
    if (ceilOffset < -30.0f) {
        return HANG_HIT_CEIL_OR_OOB;
    }
    if (ceilOffset > 30.0f) {
        return HANG_LEFT_CEIL;
    }

    nextPos[1] = m->ceilHeight - 160.0f;
    vec3f_copy(m->pos, nextPos);

    m->floor = floor;
    m->floorHeight = floorHeight;
    m->ceil = ceil;
    m->ceilHeight = ceilHeight;

    return HANG_NONE;
}
