#include "shadow_headers.h"

f32 disable_shadow_with_distance(f32 shadowScale, f32 distFromFloor) {
    if (distFromFloor >= 600.f) {
        return 0.f;
    }

    return shadowScale;
}
