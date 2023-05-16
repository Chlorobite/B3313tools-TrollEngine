#include <ultra64.h>

#include "sm64.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "audio/external.h"
#include "level_update.h"
#include "game_init.h"
#include "level_update.h"
#include "main.h"
#include "engine/math_util.h"
#include "engine/graph_node.h"
#include "area.h"
#include "save_file.h"
#include "sound_init.h"
#include "mario.h"
#include "camera.h"
#include "object_list_processor.h"
#include "ingame_menu.h"
#include "obj_behaviors.h"
#include "save_file.h"
#include "debug_course.h"
#ifdef VERSION_EU
#include "memory.h"
#include "eu_translation.h"
#include "segment_symbols.h"
#endif
#include "level_table.h"
#include "course_table.h"
#include "rumble_init.h"

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

extern u8 gLastPaintingId;
extern u16 gLastArea;
extern u8 gTrollWarpDestination;
extern struct CreditsEntry sCreditsSequence[];

void trolled_initiate_delayed_warp(void) {
    struct ObjectWarpNode *warpNode;
    s32 destWarpNode;

    if (sDelayedWarpOp != WARP_OP_NONE && --sDelayedWarpTimer == 0) {
        reset_dialog_render_state();

		switch (sDelayedWarpOp) {
			case WARP_OP_GAME_OVER:
				save_file_reload();
				warp_special(-3);
				break;

			case WARP_OP_CREDITS_END:
				warp_special(-1);
				sound_banks_enable(SEQ_PLAYER_SFX,
									SOUND_BANKS_ALL & ~SOUND_BANKS_DISABLED_AFTER_CREDITS);
				break;

			case WARP_OP_DEMO_NEXT:
				warp_special(-2);
				break;

			case WARP_OP_CREDITS_START:
				gCurrCreditsEntry = &sCreditsSequence[0];
				initiate_warp(gCurrCreditsEntry->levelNum, gCurrCreditsEntry->areaIndex,
								WARP_NODE_CREDITS_START, 0);
				break;

			case WARP_OP_CREDITS_NEXT:
				sound_banks_disable(SEQ_PLAYER_SFX, SOUND_BANKS_ALL);

				gCurrCreditsEntry++;
				gCurrActNum = gCurrCreditsEntry->unk02 & 0x07;
				if ((gCurrCreditsEntry + 1)->levelNum == LEVEL_NONE) {
					destWarpNode = WARP_NODE_CREDITS_END;
				} else {
					destWarpNode = WARP_NODE_CREDITS_NEXT;
				}

				initiate_warp(gCurrCreditsEntry->levelNum, gCurrCreditsEntry->areaIndex,
								destWarpNode, 0);
				break;

			default:
				if (gLastArea == 0) {
					gTrollWarpDestination = 0;
				}
				
				if (gTrollWarpDestination) {
					initiate_warp(gLastArea >> 4, gLastArea & 0x000F, 0, sDelayedWarpArg);
				}
				else {
					warpNode = area_get_warp_node(sSourceWarpNodeId);

					initiate_warp(warpNode->node.destLevel & 0x7F, warpNode->node.destArea,
								warpNode->node.destNode, sDelayedWarpArg);
				}

				if (sWarpDest.type != WARP_TYPE_CHANGE_LEVEL) {
					level_set_transition(2, NULL);
				}
				break;
		}
    }
}
