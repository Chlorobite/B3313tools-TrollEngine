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


void cur_obj_scale_over_time(s32 a0, s32 a1, register f32 from, register f32 to) {
    register f32 diff = to - from;
    register f32 lerp = (f32)o->oTimer / a1;
    register f32 scale = lerp * diff + from;

	// EDIT: level scale, since this function is used by level scaled objects (cap and purple switches),
	// scale the output of cur_obj_scale_over_time as well
	// (POSSIBLE BUG: this makes the assumption that the object is supposed to be affected by the level scale!)
    if (a0 & 0x01) o->header.gfx.scale[0] = levelScaleH * scale;
    if (a0 & 0x02) o->header.gfx.scale[1] = levelScaleV * scale;
    if (a0 & 0x04) o->header.gfx.scale[2] = levelScaleH * scale;
	// END EDIT
}
