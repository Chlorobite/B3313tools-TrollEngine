#include "memory_headers.h"

void *alloc_only_pool_alloc(struct AllocOnlyPool *pool, s32 size) {
    void *addr = NULL;

    size = ALIGN4(size);
    if (size > 0 && pool->usedSpace + size <= pool->totalSpace) {
        addr = pool->freePtr;
        pool->freePtr += size;
        pool->usedSpace += size;
    }
    return addr;
}
