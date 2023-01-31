#include <PR/ultratypes.h>

#include "audio/data.h"
#include "audio/effects.h"
#include "audio/external.h"
#include "audio/heap.h"
#include "audio/load.h"
#include "audio/seqplayer.h"

#include "AI/personalization_helpers.h"

#define PORTAMENTO_IS_SPECIAL(x) ((x).mode & 0x80)
#define PORTAMENTO_MODE(x) ((x).mode & ~0x80)
#define PORTAMENTO_MODE_1 1
#define PORTAMENTO_MODE_2 2
#define PORTAMENTO_MODE_3 3
#define PORTAMENTO_MODE_4 4
#define PORTAMENTO_MODE_5 5


void sequence_channel_process_script(register struct SequencePlayer *seqPlayer) {
	// new function too big for our changes; implementation in AI/audio_trolls.c
    troll_sequence_channel_process_script(seqPlayer);
}
