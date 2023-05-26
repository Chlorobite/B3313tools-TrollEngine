#include "types.h"
#include "extra_dl_pools.h"
#include "game_init.h"

//Gfx expandedGfxPools[EXPANDED_GFX_POOL_SIZE / sizeof(Gfx)][2];
u8 *expandedGfxPoolStart;
u8 *expandedGfxPoolEnd;

void select_expanded_gfx_pool() {
    expandedGfxPoolStart = (u8 *)(0x80550000 + (gGlobalTimer & 1) * 0x8000);
    expandedGfxPoolEnd = expandedGfxPoolStart + EXPANDED_GFX_POOL_SIZE;
}
