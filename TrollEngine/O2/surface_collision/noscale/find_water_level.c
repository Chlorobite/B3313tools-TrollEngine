#include "../surface_collision_headers.h"

f32 find_water_level(f32 x, f32 z) {
    s32 i;
    s32 numRegions;
    s16 val;
    f32 loX, hiX, loZ, hiZ;
    f32 waterLevel = FLOOR_LOWER_LIMIT;
    s16 *p = gEnvironmentRegions;

    if (p != NULL) {
        numRegions = *p++;

        for (i = 0; i < numRegions; i++) {
            val = *p;

            if (val < 50) {
                loX = p[1];
                loZ = p[2];
                hiX = p[3];
                hiZ = p[4];

				// If the location is within a water box and it is a water box.
				// Water is less than 50 val only, while above is gas and such.
                if (loX < x && x < hiX && loZ < z && z < hiZ) {
                    // Set the water height. Since this breaks, only return the first height.
                    waterLevel = p[5];
                    break;
                }
            }

            p += 6;
        }
    }

    return waterLevel;
}
