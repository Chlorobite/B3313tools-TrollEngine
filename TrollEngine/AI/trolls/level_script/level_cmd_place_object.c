#include "level_script_headers.h"

void level_cmd_place_object(void) {
    register s32 currActFlag = 1 << (gCurrActNum - 1);
    register s32 objActFlags = CMD_GET(u8, 2);
    register struct SpawnInfo *spawnInfo;

    if (((objActFlags & 2) != 0) != ((objActFlags & 4) != 0)) {
        // if nightTime != 0, then it is night. In that case, don't spawn if act 2 is set (while act 3 is not).
        // if nightTime = 0, then it is day. In that case, don't spawn if act 2 is not set (therefore act 3 is).
        if ((nightTimeButReal != 0) == ((objActFlags & 2) != 0)) {
            objActFlags = 0;
        }
        else {
            objActFlags |= 6;
        }
    }

    if ((objActFlags & currActFlag) || objActFlags == 0x1F) {
        spawnInfo = alloc_only_pool_alloc(sLevelPool, sizeof(struct SpawnInfo));

        spawnInfo->startPos[0] = CMD_GET(s16, 4);
        spawnInfo->startPos[1] = CMD_GET(s16, 6);
        spawnInfo->startPos[2] = CMD_GET(s16, 8);

        spawnInfo->startAngle[0] = (s16)(CMD_GET(s16, 10) * 182.04444444f);
        spawnInfo->startAngle[1] = (s16)(CMD_GET(s16, 12) * 182.04444444f);
        spawnInfo->startAngle[2] = (s16)(CMD_GET(s16, 14) * 182.04444444f);

        spawnInfo->areaIndex = sCurrAreaIndex;
        spawnInfo->activeAreaIndex = sCurrAreaIndex;

        spawnInfo->behaviorArg = CMD_GET(u32, 16);
        spawnInfo->behaviorScript = CMD_GET(void *, 20);
        spawnInfo->model = gLoadedGraphNodes[CMD_GET(u8, 3)];
        spawnInfo->next = gAreas[sCurrAreaIndex].objectSpawnInfos;

        gAreas[sCurrAreaIndex].objectSpawnInfos = spawnInfo;
    }

    sCurrentCmd = CMD_NEXT;
}
