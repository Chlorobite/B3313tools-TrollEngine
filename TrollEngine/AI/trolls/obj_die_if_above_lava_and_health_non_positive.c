#include <PR/ultratypes.h>

#include "sm64.h"
#include "actors/common0.h"
#include "actors/group11.h"
#include "actors/group17.h"
#include "audio/external.h"
#include "game/behavior_actions.h"
#include "behavior_data.h"
#include "game/camera.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "game/game_init.h"
#include "game/geo_misc.h"
#include "game/ingame_menu.h"
#include "game/interaction.h"
#include "level_table.h"
#include "game/level_update.h"
#include "game/mario.h"
#include "game/mario_actions_cutscene.h"
#include "game/memory.h"
#include "game/obj_behaviors.h"
#include "game/obj_behaviors_2.h"
#include "object_constants.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/platform_displacement.h"
#include "game/rendering_graph_node.h"
#include "game/save_file.h"
#include "seq_ids.h"
#include "game/spawn_sound.h"

// bhvs exist so AI exists too
#include "AI/stats_tracking.h"
#include "AI/personalization_helpers.h"

#define POS_OP_SAVE_POSITION 0
#define POS_OP_COMPUTE_VELOCITY 1
#define POS_OP_RESTORE_POSITION 2

#define o gCurrentObject

/* BSS (declared to force order) */
extern s32 sNumActiveFirePiranhaPlants;
extern s32 sNumKilledFirePiranhaPlants;
extern f32 sObjSavedPosX;
extern f32 sObjSavedPosY;
extern f32 sObjSavedPosZ;
extern struct Object *sMontyMoleHoleList;
extern s32 sMontyMoleKillStreak;
extern f32 sMontyMoleLastKilledPosX;
extern f32 sMontyMoleLastKilledPosY;
extern f32 sMontyMoleLastKilledPosZ;
extern struct Object *sMasterTreadmill;


s32 obj_die_if_above_lava_and_health_non_positive(void) {
    if (o->oMoveFlags & OBJ_MOVE_UNDERWATER_ON_GROUND) {
        if (o->oGravity + o->oBuoyancy > 0.0f
            || find_water_level(o->oPosX, o->oPosZ) - o->oPosY < 150.0f) {
            return FALSE;
        }
    } else if (!(o->oMoveFlags & OBJ_MOVE_ABOVE_LAVA)) {
        if (o->oMoveFlags & OBJ_MOVE_ENTERED_WATER) {
            if (o->oWallHitboxRadius < 200.0f) {
                cur_obj_play_sound_2(SOUND_OBJ_DIVING_INTO_WATER);
            } else {
                cur_obj_play_sound_2(SOUND_OBJ_DIVING_IN_WATER);
            }
        }
        return FALSE;
    }

	// ADD: new object flag made for lava skeeters (0x8000)
    if (!(o->oFlags & OBJ_FLAG_RESIST_LAVA)) {
        obj_die_if_health_non_positive();
        return TRUE;
    }
	// END ADD
    
    return FALSE;
}
