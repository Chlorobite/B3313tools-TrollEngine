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

#include "normal/header.h"
#include "bhvdefs.h"

extern void bhv_motos_hand_loop(void);
extern void bhv_motos_loop(void);

const BehaviorScript bhvMotos[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW | OBJ_FLAG_HOLDABLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO)),
    LOAD_ANIMATIONS(oAnimations, motos_anime),
    SET_INT(oInteractType, INTERACT_GRABBABLE),
    SET_HITBOX(/*Radius*/ 100, /*Height*/ 100),
    SET_OBJ_PHYSICS(/*Wall hitbox radius*/ 30, /*Gravity*/ -400, /*Bounciness*/ -50, /*Drag strength*/ 1000, /*Friction*/ 1000, /*Buoyancy*/ 200, /*Unused*/ 0, 0),
    SPAWN_OBJ(/*Model*/ MODEL_NONE, /*Behavior*/ bhvMotosHand),
    SET_INT(oIntangibleTimer, 0),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_motos_loop),
    END_LOOP(),
};

const BehaviorScript bhvBlueCoinMotos[] = {
    BEGIN(OBJ_LIST_LEVEL),
    CALL_NATIVE(bhv_moving_blue_coin_init),
    // duping because I'm not touching yellow coin bhv
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    BILLBOARD(),
    SET_HITBOX(/*Radius*/ 100, /*Height*/ 64),
    SET_INT(oInteractType, INTERACT_COIN),
    SET_INT(oIntangibleTimer, 0),
    SET_INT(oAnimState, -1),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_moving_yellow_coin_loop),
        ADD_INT(oAnimState, 1),
    END_LOOP(),
}; 


const BehaviorScript bhvMotosHand[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW)),
    BILLBOARD(),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_motos_hand_loop),
    END_LOOP(),
};
