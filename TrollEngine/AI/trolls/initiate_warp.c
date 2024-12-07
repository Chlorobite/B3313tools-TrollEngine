#include <ultra64.h>
#include "types.h"
#include "area.h"
#include "level_update.h"

#define WARP_TYPE_NOT_WARPING 0
#define WARP_TYPE_CHANGE_LEVEL 1
#define WARP_TYPE_CHANGE_AREA 2
#define WARP_TYPE_SAME_AREA 3

void initiate_warp(s16 destLevel, s16 destArea, s16 destWarpNode, s32 arg3) {
    sWarpDest.type =
        (destLevel == gCurrLevelNum && destArea == gCurrentArea->index) ?
            WARP_TYPE_SAME_AREA : WARP_TYPE_CHANGE_LEVEL;
    sWarpDest.levelNum = destLevel;
    sWarpDest.areaIdx = destArea;
    sWarpDest.nodeId = destWarpNode;
    sWarpDest.arg = arg3;

    // set up magic value that external program wouldnt be aware of but troll_play_mode_change_level is
    //whatscheatma = 0xBEEE;
}
