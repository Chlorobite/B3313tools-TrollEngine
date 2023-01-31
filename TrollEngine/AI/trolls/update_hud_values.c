#include <ultra64.h>

#include "sm64.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "audio/external.h"
#include "game/level_update.h"
#include "game/game_init.h"
#include "game/level_update.h"
#include "game/main.h"
#include "engine/math_util.h"
#include "engine/graph_node.h"
#include "game/area.h"
#include "game/save_file.h"
#include "game/sound_init.h"
#include "game/mario.h"
#include "game/camera.h"
#include "game/object_list_processor.h"
#include "game/ingame_menu.h"
#include "game/obj_behaviors.h"
#include "game/save_file.h"
#include "level_table.h"
#include "course_table.h"

#include "AI/stats_tracking.h"
#include "AI/post_object_load_pass.h"

#define PLAY_MODE_NORMAL 0
#define PLAY_MODE_PAUSED 2
#define PLAY_MODE_CHANGE_AREA 3
#define PLAY_MODE_CHANGE_LEVEL 4
#define PLAY_MODE_FRAME_ADVANCE 5

#define WARP_TYPE_NOT_WARPING 0
#define WARP_TYPE_CHANGE_LEVEL 1
#define WARP_TYPE_CHANGE_AREA 2
#define WARP_TYPE_SAME_AREA 3

#define WARP_NODE_F0 0xF0
#define WARP_NODE_DEATH 0xF1
#define WARP_NODE_F2 0xF2
#define WARP_NODE_WARP_FLOOR 0xF3
#define WARP_NODE_CREDITS_START 0xF8
#define WARP_NODE_CREDITS_NEXT 0xF9
#define WARP_NODE_CREDITS_END 0xFA

#define WARP_NODE_CREDITS_MIN 0xF8


void update_hud_values(void) {
    if (gCurrCreditsEntry == NULL) {
        s16 numHealthWedges = gMarioState->health > 0 ? gMarioState->health >> 8 : 0;

		// EDIT: always display coin count
        gHudDisplay.flags |= HUD_DISPLAY_FLAG_COIN_COUNT;
		// END EDIT

        if (gHudDisplay.coins < gMarioState->numCoins) {
            if (gGlobalTimer & 1) {
				// REMOVE: separate water coin sound
                gHudDisplay.coins++;
                play_sound(0x50218081, gMarioState->marioObj->header.gfx.cameraToObject);
				// END REMOVE
            }
        }

		// ADD: handle losing coins (purple coins)
        if (gHudDisplay.coins > gMarioState->numCoins) {
            if (gGlobalTimer & 1) {
                gHudDisplay.coins--;
                play_sound(SOUND_OBJ_BOO_LAUGH_SHORT, gMarioState->marioObj->header.gfx.cameraToObject);
            }
        }
		// END ADD

        if (gMarioState->numLives > 100) {
            gMarioState->numLives = 100;
        }

		// EDIT: real beta, instead of capping at 999, add 1ups at 100 coins
        if (gHudDisplay.coins >= 100) {
            gHudDisplay.coins -= 100;
            gMarioState->numCoins -= 100;
            gMarioState->numLives++;
            play_sound(SOUND_GENERAL_COLLECT_1UP, gMarioState->marioObj->header.gfx.cameraToObject);
        }
		// also handle going into the negatives
        else if (gHudDisplay.coins < 0) {
            gHudDisplay.coins += 100;
            gMarioState->numCoins += 100;
            gMarioState->numLives--;
            play_sound(0x701FFF81, gMarioState->marioObj->header.gfx.cameraToObject);
        }
		// END EDIT

        gHudDisplay.stars = gMarioState->numStars;
        gHudDisplay.lives = gMarioState->numLives;
        gHudDisplay.keys = gMarioState->numKeys;

        if (numHealthWedges > gHudDisplay.wedges) {
            play_sound(SOUND_MENU_POWER_METER, gGlobalSoundSource);
        }
        gHudDisplay.wedges = numHealthWedges;

        if (gMarioState->hurtCounter > 0) {
            gHudDisplay.flags |= HUD_DISPLAY_FLAG_EMPHASIZE_POWER;
        } else {
            gHudDisplay.flags &= ~HUD_DISPLAY_FLAG_EMPHASIZE_POWER;
        }
    }
}
