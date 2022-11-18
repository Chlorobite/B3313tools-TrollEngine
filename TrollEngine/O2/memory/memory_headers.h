#include <PR/ultratypes.h>

#include "sm64.h"

#define INCLUDED_FROM_MEMORY_C

#include "buffers/buffers.h"
#include "decompress.h"
#include "game_init.h"
#include "main.h"
#include "memory.h"
#include "segment_symbols.h"
#include "segments.h"

// round up to the next multiple
#define ALIGN4(val) (((val) + 0x3) & ~0x3)
#define ALIGN8(val) (((val) + 0x7) & ~0x7)
#define ALIGN16(val) (((val) + 0xF) & ~0xF)
