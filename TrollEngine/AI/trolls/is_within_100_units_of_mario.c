#include <ultra64.h>

#define INCLUDED_FROM_CAMERA_C

#include "prevent_bss_reordering.h"
#include "sm64.h"
#include "camera.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "audio/external.h"
#include "mario_misc.h"
#include "game_init.h"
#include "hud.h"
#include "engine/math_util.h"
#include "area.h"
#include "engine/surface_collision.h"
#include "engine/behavior_script.h"
#include "level_update.h"
#include "ingame_menu.h"
#include "mario_actions_cutscene.h"
#include "save_file.h"
#include "object_helpers.h"
#include "print.h"
#include "spawn_sound.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "object_list_processor.h"
#include "paintings.h"
#include "engine/graph_node.h"
#include "level_table.h"

#include "AI/personalization_helpers.h"

#define CBUTTON_MASK (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS)

extern struct PlayerCameraState *sMarioCamState;
f32 absf(f32 val);

s32 is_within_100_units_of_mario(register f32 posX, register f32 posY, register f32 posZ) {
	// EDIT: level scale messes with hardcoded positions, so scale the position tested
	// also optimize a bit
    register Vec3f pos;

    vec3f_scaled_set(pos, posX, posY, posZ);
    return calc_abs_dist(sMarioCamState->pos, pos) < 100.f; // should we scale the 100 unit margin too, just to be safe?
	// END EDIT
}
