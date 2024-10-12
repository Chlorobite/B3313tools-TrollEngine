#include "mario_actions_submerged_headers.h"

void apply_water_current(struct MarioState *m, Vec3f step) {
    s32 i;
    f32 whirlpoolRadius = 2000.0f;

    if (m->floor->type == SURFACE_FLOWING_WATER) {
        s16 currentAngle = m->floor->force << 8;
        f32 currentSpeed = m->floor->force >> 8;

        step[0] += currentSpeed * sins(currentAngle);
        step[2] += currentSpeed * coss(currentAngle);
    }

    for (i = 0; i < 2; i++) {
        struct Whirlpool *whirlpool = gCurrentArea->whirlpools[i];
        if (whirlpool != NULL) {
            f32 strength = 0.0f;

            f32 dx = whirlpool->pos[0] - m->pos[0];
            f32 dy = whirlpool->pos[1] - m->pos[1];
            f32 dz = whirlpool->pos[2] - m->pos[2];

            f32 lateralDist = sqrtf(dx * dx + dz * dz);
            f32 distance = sqrtf(lateralDist * lateralDist + dy * dy);

            s16 pitchToWhirlpool = atan2s(lateralDist, dy);
            s16 yawToWhirlpool = atan2s(dz, dx);

            yawToWhirlpool -= (s16)(0x2000 * 1000.0f / (distance + 1000.0f));

            if (whirlpool->strength >= 0) {
                if (gCurrLevelNum == LEVEL_DDD && gCurrAreaIndex == 2) {
                    whirlpoolRadius = 4000.0f;
                }

                if (distance >= 26.0f && distance < whirlpoolRadius) {
                    strength = whirlpool->strength * (1.0f - distance / whirlpoolRadius);
                }
            } else if (distance < 2000.0f) {
                strength = whirlpool->strength * (1.0f - distance / 2000.0f);
            }

            step[0] += strength * coss(pitchToWhirlpool) * sins(yawToWhirlpool);
            step[1] += strength * sins(pitchToWhirlpool);
            step[2] += strength * coss(pitchToWhirlpool) * coss(yawToWhirlpool);
        }
    }
}
