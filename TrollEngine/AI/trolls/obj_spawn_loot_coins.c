#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "debug.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/geo_layout.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "game_init.h"
#include "helper_macros.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_table.h"
#include "level_update.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include "spawn_object.h"
#include "spawn_sound.h"

#include "AI/stats_tracking.h"
#include "AI/personalization_helpers.h"


#define o gCurrentObject


void obj_spawn_loot_coins(register struct Object *obj, s32 numCoins, f32 sp30,
                                    const BehaviorScript *coinBehavior,
                                    s16 posJitter, s16 model) {
    register s32 i;
    register struct Object *coin;

	// REMOVE: coin spawn height override
	// (they snapped to the ground if the coins spawned less than 100 units above, kinda useless)

    spawningLootCoins = TRUE;
    for (i = 0; i < numCoins; i++) {
        if (obj->oNumLootCoins <= 0) {
            break;
        }

        obj->oNumLootCoins--;

        coin = spawn_object(obj, model, coinBehavior);
        obj_translate_xz_random(coin, posJitter);
        coin->oCoinUnk110 = sp30;
    }
    
    // ADD: personalization AI, increase murder count
    if (numCoins != 0 && get_object_list_from_behavior(obj->behavior) != OBJ_LIST_SURFACE)
        TRACKER_accum_murder += 0.1f;
	// END ADD
}
