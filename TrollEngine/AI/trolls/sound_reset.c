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

#include "AI/audio_trolls.h"

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


extern s32 sGameLoopTicked;
extern u8 D_80332108;
extern u8 sHasStartedFadeOut;


void sound_reset(u8 presetId) {
    sGameLoopTicked = 0;
    disable_all_sequence_players();
    sound_init();
    audio_reset_session(&gAudioSessionPresets[presetId]);
    osWritebackDCacheAll();
    if (presetId != 7) {
        preload_sequence(SEQ_EVENT_SOLVE_PUZZLE, PRELOAD_BANKS | PRELOAD_SEQUENCE);
        preload_sequence(SEQ_EVENT_PEACH_MESSAGE, PRELOAD_BANKS | PRELOAD_SEQUENCE);
        preload_sequence(SEQ_EVENT_CUTSCENE_STAR_SPAWN, PRELOAD_BANKS | PRELOAD_SEQUENCE);
    }
    seq_player_play_sequence(SEQ_PLAYER_SFX, SEQ_SOUND_PLAYER, 0);
    D_80332108 = (D_80332108 & 0xf0) + presetId;
    gSoundMode = D_80332108 >> 4;
    sHasStartedFadeOut = FALSE;
}
