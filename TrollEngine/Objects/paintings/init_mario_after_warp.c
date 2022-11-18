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
#include "level_table.h"
#include "course_table.h"
#include "rumble_init.h"
#include "game/paintings.h"

#include "AI/personalization_helpers.h"

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

extern struct Painting *paintings[];

extern void offload_init_mario_after_warp(u32 marioSpawnType, struct Painting *exitPainting);

void trolled_init_mario_after_warp(void) {
	register struct ObjectWarpNode *spawnNode;
	register u32 marioSpawnType;
	
	if (gTrollWarpDestination && gLastArea != 0) {
		marioSpawnType = gTrollWarpDestination == 2 ? MARIO_SPAWN_PAINTING_STAR_COLLECT : MARIO_SPAWN_PAINTING_DEATH;

		if (gMarioState->action != ACT_UNINITIALIZED) {
			offload_init_mario_after_warp(marioSpawnType, paintings[gLastPaintingId]);

			gPlayerSpawnInfos[0].startAngle[0] = 0;
			gPlayerSpawnInfos[0].startAngle[1] += 0x8000;
			gPlayerSpawnInfos[0].startAngle[2] = 0;

			if (sWarpDest.type == WARP_TYPE_CHANGE_LEVEL || sWarpDest.type == WARP_TYPE_CHANGE_AREA) {
				gPlayerSpawnInfos[0].areaIndex = sWarpDest.areaIdx;
				load_mario_area();
			}

			init_mario();
			set_mario_initial_action(gMarioState, marioSpawnType, sWarpDest.arg);
		}
	}
	else {
		spawnNode = area_get_warp_node(sWarpDest.nodeId);
		marioSpawnType = get_mario_spawn_type(spawnNode->object);

		if (gMarioState->action != ACT_UNINITIALIZED) {
			gPlayerSpawnInfos[0].startPos[0] = (s16) (spawnNode->object->oPosX / levelScaleH);
			gPlayerSpawnInfos[0].startPos[1] = (s16) (spawnNode->object->oPosY / levelScaleV);
			gPlayerSpawnInfos[0].startPos[2] = (s16) (spawnNode->object->oPosZ / levelScaleH);

			gPlayerSpawnInfos[0].startAngle[0] = 0;
			gPlayerSpawnInfos[0].startAngle[1] = spawnNode->object->oMoveAngleYaw;
			gPlayerSpawnInfos[0].startAngle[2] = 0;

			if (marioSpawnType == MARIO_SPAWN_DOOR_WARP) {
				init_door_warp(&gPlayerSpawnInfos[0], sWarpDest.arg);
			}

			if (sWarpDest.type == WARP_TYPE_CHANGE_LEVEL || sWarpDest.type == WARP_TYPE_CHANGE_AREA) {
				gPlayerSpawnInfos[0].areaIndex = sWarpDest.areaIdx;
				load_mario_area();
			}

			init_mario();
			set_mario_initial_action(gMarioState, marioSpawnType, sWarpDest.arg);

			gMarioState->interactObj = spawnNode->object;
			gMarioState->usedObj = spawnNode->object;
		}
	}

    reset_camera(gCurrentArea->camera);
    sWarpDest.type = WARP_TYPE_NOT_WARPING;
    sDelayedWarpOp = WARP_OP_NONE;

    switch (marioSpawnType) {
        case MARIO_SPAWN_UNKNOWN_03:
            play_transition(WARP_TRANSITION_FADE_FROM_STAR, 0x10, 0x00, 0x00, 0x00);
            break;
        case MARIO_SPAWN_DOOR_WARP:
            play_transition(WARP_TRANSITION_FADE_FROM_CIRCLE, 0x10, 0x00, 0x00, 0x00);
            break;
        case MARIO_SPAWN_TELEPORT:
            play_transition(WARP_TRANSITION_FADE_FROM_COLOR, 0x14, 0xFF, 0xFF, 0xFF);
            break;
        case MARIO_SPAWN_SPIN_AIRBORNE:
            play_transition(WARP_TRANSITION_FADE_FROM_COLOR, 0x1A, 0xFF, 0xFF, 0xFF);
            break;
        case MARIO_SPAWN_SPIN_AIRBORNE_CIRCLE:
            play_transition(WARP_TRANSITION_FADE_FROM_CIRCLE, 0x10, 0x00, 0x00, 0x00);
            break;
        case MARIO_SPAWN_UNKNOWN_27:
            play_transition(WARP_TRANSITION_FADE_FROM_COLOR, 0x10, 0x00, 0x00, 0x00);
            break;
        default:
            play_transition(WARP_TRANSITION_FADE_FROM_STAR, 0x10, 0x00, 0x00, 0x00);
            break;
    }

	set_background_music(gCurrentArea->musicParam, gCurrentArea->musicParam2, 0);

	/*if (gMarioState->flags & MARIO_METAL_CAP) {
		play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP));
	}

	if (gMarioState->flags & (MARIO_VANISH_CAP | MARIO_WING_CAP)) {
		play_cap_music(SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP));
	}*/
}
