#ifndef AI_EXTRA_DL_POOLS_H
#define AI_EXTRA_DL_POOLS_H

#include "types.h"

#define EXPANDED_GFX_POOL_SIZE 0x8000

extern u8 *expandedGfxPoolStart;
extern u8 *expandedGfxPoolEnd;
void select_expanded_gfx_pool();

#endif
