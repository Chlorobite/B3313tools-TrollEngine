#include "memory_headers.h"

void *alloc_display_list(u32 size) {
    void *ptr = NULL;

    size = ALIGN8(size);
    if (gGfxPoolEnd - size >= (u8 *) gDisplayListHead) {
        gGfxPoolEnd -= size;
        ptr = gGfxPoolEnd;
    }
    return ptr;
}
