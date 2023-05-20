#include "../surface_collision_headers.h"
#include "AI/personalization_helpers.h"

f32 find_poison_gas_level(register f32 x, register f32 z) {
    register s32 i;
    register s32 numRegions;
    s16 val;
    register f32 loX, hiX, loZ, hiZ;
    f32 gasLevel = FLOOR_LOWER_LIMIT;
    register s16 *p = gEnvironmentRegions;

    if (p != NULL && PERSONALIZATION_FLAG_ENABLE_TOXIC_GAS) {
        numRegions = *p++;

        for (i = 0; i < numRegions; i++) {
            val = *p;

            if (val >= 50 && val % 10 == 0) {
                loX = p[1] * levelScaleH;
                loZ = p[2] * levelScaleH;
                hiX = p[3] * levelScaleH;
                hiZ = p[4] * levelScaleH;

                // If the location is within a gas's box and it is a gas box.
                // Gas has a value of 50, 60, etc.
                if (loX < x && x < hiX && loZ < z && z < hiZ) {
                    // Set the gas height. Since this breaks, only return the first height.
                    gasLevel = p[5];
                    break;
                }
            }

            p += 6;
        }
    }

    return gasLevel * levelScaleV;
}
