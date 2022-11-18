#include "shadow_headers.h"

f32 scale_shadow_with_distance(f32 initial, f32 distFromFloor) {
    if (distFromFloor <= 0.f) {
        return initial;
    }
    if (distFromFloor >= 600.f) {
        return 0.0f;
    }
    
    return initial * (1.f - distFromFloor / 600.f);
}
