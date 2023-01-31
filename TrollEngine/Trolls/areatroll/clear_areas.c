#include "trolle.h"

void clear_areas(void) {
    register s32 i;

    gCurrentArea = NULL;
    gWarpTransition.isActive = FALSE;
    gWarpTransition.pauseRendering = FALSE;
    gMarioSpawnInfo->areaIndex = -1;

    for (i = 0; i < AREA_COUNT; i++) {
        gAreaDataNew[i].index = i;
        gAreaDataNew[i].flags = 0;
        gAreaDataNew[i].terrainType = 0;
        gAreaDataNew[i].unk04 = NULL;
        gAreaDataNew[i].terrainData = NULL;
        gAreaDataNew[i].surfaceRooms = NULL;
        gAreaDataNew[i].macroObjects = NULL;
        gAreaDataNew[i].warpNodes = NULL;
        gAreaDataNew[i].paintingWarpNodes = NULL;
        gAreaDataNew[i].instantWarps = NULL;
        gAreaDataNew[i].objectSpawnInfos = NULL;
        gAreaDataNew[i].camera = NULL;
        gAreaDataNew[i].unused = NULL;
        gAreaDataNew[i].whirlpools[0] = NULL;
        gAreaDataNew[i].whirlpools[1] = NULL;
        gAreaDataNew[i].dialog[0] = DIALOG_NONE;
        gAreaDataNew[i].dialog[1] = DIALOG_NONE;
        gAreaDataNew[i].musicParam = 0;
        gAreaDataNew[i].musicParam2 = 0;
    }
}
