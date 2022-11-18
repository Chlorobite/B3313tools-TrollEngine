#include <ultra64.h>
#include "sm64.h"
#include "audio/heap.h"
#include "audio/load.h"
#include "audio/data.h"
#include "audio/seqplayer.h"
#include "audio/external.h"
#include "audio/playback.h"
#include "audio/synthesis.h"
#include "game/level_update.h"
#include "game/object_list_processor.h"
#include "game/camera.h"
#include "seq_ids.h"
#include "dialog_ids.h"

#if defined(VERSION_EU) || defined(VERSION_SH)
#define EU_FLOAT(x) x##f
#else
#define EU_FLOAT(x) x
#endif

// N.B. sound banks are different from the audio banks referred to in other
// files. We should really fix our naming to be less ambiguous...
#define MAX_BACKGROUND_MUSIC_QUEUE_SIZE 6
#define MAX_CHANNELS_PER_SOUND_BANK 1

#define SEQUENCE_NONE 0xFF

#define SAMPLES_TO_OVERPRODUCE 0x10
#define EXTRA_BUFFERED_AI_SAMPLES_TARGET 0x40

#define MARIO_X_GE 0
#define MARIO_Y_GE 1
#define MARIO_Z_GE 2
#define MARIO_X_LT 3
#define MARIO_Y_LT 4
#define MARIO_Z_LT 5
#define MARIO_IS_IN_AREA 6
#define MARIO_IS_IN_ROOM 7

extern s16 *sLevelDynamics[];

struct MusicDynamic {
    /*0x0*/ s16 bits1;
    /*0x2*/ u16 volScale1;
    /*0x4*/ s16 dur1;
    /*0x6*/ s16 bits2;
    /*0x8*/ u16 volScale2;
    /*0xA*/ s16 dur2;
}; // size = 0xC

extern u8 sCurrentMusicDynamic;
extern u8 sBackgroundMusicForDynamics;
extern struct MusicDynamic sMusicDynamics[];
extern u8 sCurrentBackgroundMusicSeqId;
extern u8 sMusicDynamicDelay;
