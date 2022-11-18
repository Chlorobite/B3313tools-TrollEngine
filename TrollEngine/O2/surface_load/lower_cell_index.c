#include "surface_load_headers.h"

s16 lower_cell_index(register s32 coord) {
    // Move from range [-0x2000, 0x2000) to [0, 0x4000)
    coord += LEVEL_BOUNDARY_MAX - 100;
    if (coord < 0) {
        coord = 0;
    }
    if (coord > LEVEL_BOUNDARY_MAX * 2 - 1) {
        coord = LEVEL_BOUNDARY_MAX * 2 - 1;
    }

    return (s16)(coord / CELL_SIZE);
}
