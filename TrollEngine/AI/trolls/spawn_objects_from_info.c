#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "behavior_data.h"
#include "camera.h"
#include "debug.h"
#include "engine/behavior_script.h"
#include "engine/graph_node.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "interaction.h"
#include "level_update.h"
#include "mario.h"
#include "memory.h"
#include "object_collision.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "platform_displacement.h"
#include "profiler.h"
#include "spawn_object.h"

#include "AI/post_object_load_pass.h"


void spawn_objects_from_info(UNUSED s32 unused, register struct SpawnInfo *spawnInfo) {
    gObjectLists = gObjectListArray;
    gTimeStopState = 0;

    gWDWWaterLevelChanging = FALSE;
    gMarioOnMerryGoRound = FALSE;

    if (gCurrAreaIndex == 2) {
        gCCMEnteredSlide |= 1;
    }

    while (spawnInfo != NULL) {
        struct Object *object;
        const BehaviorScript *script;

        script = segmented_to_virtual(spawnInfo->behaviorScript);

        // If the object was previously killed/collected, don't respawn it
        if ((spawnInfo->behaviorArg & (RESPAWN_INFO_DONT_RESPAWN << 8))
            != (RESPAWN_INFO_DONT_RESPAWN << 8)) {
            object = create_object(script);

            // Behavior parameters are often treated as four separate bytes, but
            // are stored as an s32.
            object->oBehParams = spawnInfo->behaviorArg;
            // The second byte of the behavior parameters is copied over to a special field
            // as it is the most frequently used by objects.
            object->oBehParams2ndByte = ((spawnInfo->behaviorArg) >> 16) & 0xFF;

            object->behavior = script;
            object->unused1 = 0;

            // Record death/collection in the SpawnInfo
            object->respawnInfoType = RESPAWN_INFO_TYPE_32;
            object->respawnInfo = &spawnInfo->behaviorArg;

            if (spawnInfo->behaviorArg & 0x01) {
                gMarioObject = object;
                geo_make_first_child(&object->header.gfx.node);
            }

            geo_obj_init_spawninfo(&object->header.gfx, spawnInfo);

            object->oPosX = spawnInfo->startPos[0];
            object->oPosY = spawnInfo->startPos[1];
            object->oPosZ = spawnInfo->startPos[2];

            object->oFaceAnglePitch = spawnInfo->startAngle[0];
            object->oFaceAngleYaw = spawnInfo->startAngle[1];
            object->oFaceAngleRoll = spawnInfo->startAngle[2];

            object->oMoveAnglePitch = spawnInfo->startAngle[0];
            object->oMoveAngleYaw = spawnInfo->startAngle[1];
            object->oMoveAngleRoll = spawnInfo->startAngle[2];
        }

        spawnInfo = spawnInfo->next;
    }
    
	// ADD: personalization AI, see AI/post_object_load_pass.c
    addMoreObjects();
	// END ADD
}
