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

void bhv_playma_sound_loop();
void bhv_shakema_camera_loop();
void bhv_movema_object_move_bparams_to_fields();
void bhv_movema_object_loop();
void bhv_radius_dialog_box_loop();
void bhv_trolled_donut_platform_update();
void bhv_silver_star_init();
void bhv_silver_star_loop();
void bhv_hidden_silver_star_star_init();
void bhv_hidden_silver_star_star_loop();


const BehaviorScript bhvPlaymaSoundLoop[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_playma_sound_loop),
    END_LOOP(),
};

const BehaviorScript bhvShakemaCameraLoop[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_shakema_camera_loop),
    END_LOOP(),
};

const BehaviorScript bhvMovemaObjectCopyBparamsFunc[] = {
    CALL_NATIVE(bhv_movema_object_move_bparams_to_fields),
    RETURN(),
};

const BehaviorScript bhvMovemaObjectLoopFunc[] = {
    CALL_NATIVE(bhv_movema_object_loop),
    RETURN(),
};

const BehaviorScript bhvRadiusDialogBox[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_radius_dialog_box_loop),
    END_LOOP(),
};

const BehaviorScript bhvMovemaTest[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    OR_INT(oFlags, (OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    CALL(bhvMovemaObjectCopyBparamsFunc),
    BEGIN_LOOP(),
        CALL(bhvMovemaObjectLoopFunc),
    END_LOOP(),
};

const BehaviorScript bhvStarMagnet[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BEGIN_LOOP(),
    END_LOOP(),
};

const BehaviorScript bhvMusicModifier[] = {
    BEGIN(OBJ_LIST_DEFAULT),
    BEGIN_LOOP(),
    END_LOOP(),
};

const BehaviorScript bhvTrolledDonutPlatformFunc[] = {
    CALL_NATIVE(bhv_trolled_donut_platform_update),
    RETURN(),
};

const BehaviorScript bhvSilverStar[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE | OBJ_FLAG_COMPUTE_DIST_TO_MARIO | OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO),
    BILLBOARD(),
    CALL_NATIVE(bhv_init_room),
    CALL_NATIVE(bhv_silver_star_init),
    SET_HOME(),
    BEGIN_LOOP(),
        //0x0F1A0001, // animation
        CALL_NATIVE(bhv_silver_star_loop),
    END_LOOP(),
};

const BehaviorScript bhvHiddenSilverStarStar[] = {
    BEGIN(OBJ_LIST_LEVEL),
    OR_INT(oFlags, (OBJ_FLAG_PERSISTENT_RESPAWN | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    CALL_NATIVE(bhv_hidden_silver_star_star_init),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_hidden_silver_star_star_loop),
    END_LOOP(),
};
