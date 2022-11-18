#include "../surface_load_headers.h"

void load_environmental_regions(s16 **data) {
    register s32 numRegions;
    register s32 i;

    gEnvironmentRegions = *data;
    numRegions = *(*data)++;

    for (i = 0; i < numRegions; i++) {
        s16 height;

        *data += 5;
        height = *(*data)++;

        gEnvironmentLevels[i] = height;
    }
}
