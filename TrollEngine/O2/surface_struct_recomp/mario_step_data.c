#include "mario_step_headers.h"

s16 sMovingSandSpeeds[] = { 12, 8, 4, 0 };

struct Surface gWaterSurfacePseudoFloor = {
    SURFACE_VERY_SLIPPERY, 0,    0,    0, {0, 0, 0}, NULL,
    { 0.0f, 1.0f, 0.0f },  0.0f,
    { 0, 0, 0 }, { 0, 0, 0 }, 0, 0
};
