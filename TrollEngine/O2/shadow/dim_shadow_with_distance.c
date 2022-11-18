#include "shadow_headers.h"

u8 dim_shadow_with_distance(u8 solidity, f32 distFromFloor) {
    if (solidity < 121) {
        return solidity;
    }
    if (distFromFloor <= 0.f) {
        return solidity;
    }
    if (distFromFloor >= 600.f) {
        return 120;
    }

    return ((120 - solidity) * distFromFloor) / 600.f + (f32) solidity;
}
