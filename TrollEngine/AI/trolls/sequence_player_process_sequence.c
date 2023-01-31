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

void seq_channel_layer_process_script(struct SequenceChannelLayer *layer);
void sequence_channel_process_script(struct SequenceChannel *seqChannel);
u8 get_instrument(struct SequenceChannel *seqChannel, u8 instId, struct Instrument **instOut,
                  struct AdsrSettings *adsr);


void sequence_player_process_sequence(register struct SequencePlayer *seqPlayer) {
	// new function too big for our changes; implementation in AI/audio_trolls.c
    troll_sequence_player_process_sequence(seqPlayer);
}
