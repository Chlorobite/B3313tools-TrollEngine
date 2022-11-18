#include "../shadow_headers.h"
#include "AI/personalization_helpers.h"

s8 init_shadow(struct Shadow *s, register f32 xPos, register f32 yPos, register f32 zPos, s16 shadowScale, u8 overwriteSolidity) {
    register f32 waterLevel;
    register f32 floorSteepness;
    struct Surface *flor;
    register struct Surface *floor;

    s->parentX = xPos;
    s->parentY = yPos;
    s->parentZ = zPos;

    s->floorHeight = find_floor(s->parentX, s->parentY, s->parentZ, &flor);
    floor = flor;

    gShadowAboveWaterOrLava = FALSE;
    gMarioOnIceOrCarpet = 0;
    sMarioOnFlyingCarpet = 0;
    if (floor != NULL) {
        if (floor->type == SURFACE_ICE) {
            gMarioOnIceOrCarpet = 1;
        }
        sSurfaceTypeBelowShadow = floor->type;
    }

    waterLevel = get_water_level_below_shadow(s);
    if (gShadowAboveWaterOrLava) {
        s->floorHeight = waterLevel;

        // Assume that the water is flat.
        s->floorNormalX = 0.f;
        s->floorNormalY = 1.f;
        s->floorNormalZ = 0.f;
        s->floorOriginOffset = -waterLevel;
    } else {
        // Don't draw a shadow if the floor is lower than expected possible,
        // or if the y-normal is negative (an unexpected result).
        if (s->floorHeight < FLOOR_LOWER_LIMIT_SHADOW || floor->normal.y <= 0.f) {
            return 1;
        }

        s->floorNormalX = floor->normal.x / levelScaleH;
        s->floorNormalY = floor->normal.y / levelScaleV;
        s->floorNormalZ = floor->normal.z / levelScaleH;
        s->floorOriginOffset = floor->originOffset;
    }

    if (overwriteSolidity) {
        s->solidity = dim_shadow_with_distance(overwriteSolidity, yPos - s->floorHeight);
    }

    s->shadowScale = scale_shadow_with_distance(shadowScale, yPos - s->floorHeight);

    s->floorDownwardAngle = atan2_deg(s->floorNormalZ, s->floorNormalX);

    floorSteepness = sqrtf(s->floorNormalX * s->floorNormalX + s->floorNormalZ * s->floorNormalZ);

    // This if-statement avoids dividing by 0.
    if (floorSteepness == 0.f) {
        s->floorTilt = 0.f;
    } else {
        s->floorTilt = 90.f - atan2_deg(floorSteepness, s->floorNormalY);
    }
    return 0;
}
