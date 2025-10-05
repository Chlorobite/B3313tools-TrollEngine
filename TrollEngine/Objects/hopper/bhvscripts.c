#define OBJECT_FIELDS_INDEX_DIRECTLY

#include "sm64.h"

#include "object_constants.h"
#include "game/object_list_processor.h"
#include "game/interaction.h"
#include "game/behavior_actions.h"
#include "game/mario_actions_cutscene.h"
#include "game/mario_misc.h"
#include "game/object_helpers.h"
#include "game/debug.h"
#include "menu/file_select.h"
#include "engine/surface_load.h"

#include "make_const_nonconst.h"
#include "behavior_data.h"

#include "header.h"
#include "bhvdefs.h"

extern void bhv_hopper_loop(void);
extern struct Animation *const hopper_anime[];

const BehaviorScript bhvHopper[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_HOLDABLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO  | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW)),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ 0, /*Drag strength*/ 2500, /*Friction*/ 1000, /*Buoyancy*/ 600, /*Unused*/ 0, 0),
    LOAD_ANIMATIONS(oAnimations, hopper_anime),
    SET_HOME(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_hopper_loop),
    END_LOOP(),
};
