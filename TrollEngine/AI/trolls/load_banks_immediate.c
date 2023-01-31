#include <ultra64.h>
#include <PR/os.h>

#include "audio/data.h"
#include "audio/external.h"
#include "audio/heap.h"
#include "audio/load.h"
#include "audio/seqplayer.h"

#include "AI/audio_trolls.h"

#define ALIGN16(val) (((val) + 0xF) & ~0xF)

struct SharedDma {
    /*0x0*/ u8 *buffer;       // target, points to pre-allocated buffer
    /*0x4*/ uintptr_t source; // device address
    /*0x8*/ u16 sizeUnused;   // set to bufSize, never read
    /*0xA*/ u16 bufSize;      // size of buffer
    /*0xC*/ u8 unused2;       // set to 0, never read
    /*0xD*/ u8 reuseIndex;    // position in sSampleDmaReuseQueue1/2, if ttl == 0
    /*0xE*/ u8 ttl;           // duration after which the DMA can be discarded
};                            // size = 0x10

extern u8 *gAlBankSets;


struct AudioBank *bank_load_immediate(s32 bankId, s32 arg1);

struct AudioBank *load_banks_immediate(s32 seqId, u8 *outDefaultBank) {
	// new function too big for our changes; implementation in AI/audio_trolls.c
    return troll_load_banks_immediate(seqId, outDefaultBank);
}
