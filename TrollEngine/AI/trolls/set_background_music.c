#include <ultra64.h>

#include "area.h"
#include "audio/external.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "level_table.h"
#include "level_update.h"
#include "main.h"
#include "paintings.h"
#include "print.h"
#include "profiler.h"
#include "save_file.h"
#include "seq_ids.h"
#include "sm64.h"
#include "sound_init.h"
#include "rumble_init.h"

#define MUSIC_NONE 0xFFFF

extern u16 sCurrentMusic;

void set_background_music(u16 a, u16 seqArgs, s16 fadeTimer) {
    if (gResetTimer == 0 && seqArgs != sCurrentMusic) {
        if (gCurrCreditsEntry != NULL) {
            sound_reset(7);
        } else {
            sound_reset(a);
        }

		// REMOVE: inside castle mute check (for the bowser textbox)
        play_music(SEQ_PLAYER_LEVEL, seqArgs, fadeTimer);
        sCurrentMusic = seqArgs;
		// END REMOVE
    }
}
