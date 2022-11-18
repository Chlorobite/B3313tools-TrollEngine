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

void offload_init_mario_after_warp(u32 marioSpawnType, struct Painting *exitPainting) {
	register f32 _tmp, _cos, _sin, _halfsize;

	// Set the position relative to the painting
	_tmp = exitPainting->yaw / 57.29577951308232f;
	_cos = cosf(_tmp);
	_sin = sinf(_tmp);
	_halfsize = exitPainting->size / 2.0f;

	gPlayerSpawnInfos[0].startPos[0] = (s16) ((exitPainting->posX + _cos * _halfsize - _sin * 149.0f));
	gPlayerSpawnInfos[0].startPos[1] = (s16) ((exitPainting->posY + _halfsize));
	gPlayerSpawnInfos[0].startPos[2] = (s16) ((exitPainting->posZ - (_sin * _halfsize) - _cos * 149.0f));

	gPlayerSpawnInfos[0].startAngle[1] = (s16)(exitPainting->yaw * 182.0444444444444f);
}
