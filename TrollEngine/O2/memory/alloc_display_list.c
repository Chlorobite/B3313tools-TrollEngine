#include "memory_headers.h"
#include "AI/extra_dl_pools.h"

void *alloc_display_list(u32 size) {
    void *ptr = NULL;

    size = ALIGN8(size);
    if (expandedGfxPoolEnd - size >= (u8 *) expandedGfxPoolStart) {
        expandedGfxPoolEnd -= size;
        ptr = expandedGfxPoolEnd;
    }
    else if (gGfxPoolEnd - size >= (u8 *) gDisplayListHead) {
        gGfxPoolEnd -= size;
        ptr = gGfxPoolEnd;
    }
    return ptr;
}
