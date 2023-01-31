#include <ultra64.h>

#include "sm64.h"
#include "behavior_data.h"
#include "engine/behavior_script.h"
#include "game/area.h"
#include "game/behavior_actions.h"
#include "game/game_init.h"
#include "game/mario.h"
#include "game/memory.h"
#include "game/obj_behaviors_2.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "engine/graph_node.h"
#include "engine/surface_collision.h"

#include "AI/personalization_helpers.h"

// Macros for retrieving arguments from behavior scripts.
#define BHV_CMD_GET_1ST_U8(index)  (u8)((gCurBhvCommand[index] >> 24) & 0xFF) // unused
#define BHV_CMD_GET_2ND_U8(index)  (u8)((gCurBhvCommand[index] >> 16) & 0xFF)
#define BHV_CMD_GET_3RD_U8(index)  (u8)((gCurBhvCommand[index] >> 8) & 0xFF)
#define BHV_CMD_GET_4TH_U8(index)  (u8)((gCurBhvCommand[index]) & 0xFF)

#define BHV_CMD_GET_1ST_S16(index) (s16)(gCurBhvCommand[index] >> 16)
#define BHV_CMD_GET_2ND_S16(index) (s16)(gCurBhvCommand[index] & 0xFFFF)

#define BHV_CMD_GET_U32(index)     (u32)(gCurBhvCommand[index])
#define BHV_CMD_GET_VPTR(index)    (void *)(gCurBhvCommand[index])

#define BHV_CMD_GET_ADDR_OF_CMD(index) (uintptr_t)(&gCurBhvCommand[index])


s32 bhv_cmd_begin(void) {
	// EDIT: reduce code size of haunted chair and mad piano check
	register u32 *bhv = virtual_to_segmented(0x13, gCurrentObject->behavior);
    // These objects were likely very early objects, which is why this code is here
    // instead of in the respective behavior scripts.

    // Initiate the room if the object is a haunted chair or the mad piano.
    if (bhv == bhvHauntedChair || bhv == bhvMadPiano) {
        bhv_init_room();
    }
	// END EDIT: reduce code size of haunted chair and mad piano check
    // REMOVE: Set collision distance if the object is a message panel. (message panels are unused; area overflow otherwise)
    /*if (bhv == bhvMessagePanel) {
        gCurrentObject->oCollisionDistance = 150.0f;
    }*/
	// END REMOVE
    
	// ADD: level scale, scale surface type objects when they load
    if (BHV_CMD_GET_2ND_U8(0) == OBJ_LIST_SURFACE) {
        obj_scale_personalized(gCurrentObject);
    }
	// END ADD
    gCurBhvCommand++;
    return BHV_PROC_CONTINUE;
}
