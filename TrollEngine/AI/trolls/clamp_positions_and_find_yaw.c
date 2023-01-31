#include <ultra64.h>

#define INCLUDED_FROM_CAMERA_C

#include "prevent_bss_reordering.h"
#include "sm64.h"
#include "game/camera.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "audio/external.h"
#include "game/mario_misc.h"
#include "game/game_init.h"
#include "game/hud.h"
#include "engine/math_util.h"
#include "game/area.h"
#include "engine/surface_collision.h"
#include "engine/behavior_script.h"
#include "game/level_update.h"
#include "game/ingame_menu.h"
#include "game/mario_actions_cutscene.h"
#include "game/spawn_sound.h"
#include "game/behavior_actions.h"
#include "behavior_data.h"
#include "game/object_list_processor.h"
#include "game/paintings.h"
#include "engine/graph_node.h"
#include "level_table.h"

#include "AI/personalization_helpers.h"

#define CBUTTON_MASK (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS)
extern struct Camera *gCamera;

s32 clamp_positions_and_find_yaw(Vec3f pos, Vec3f origin, register f32 xMax, register f32 xMin, register f32 zMax, register f32 zMin) {
    s16 yaw = gCamera->nextYaw;

	// ADD: level scale messes with hardcoded positions, so scale the bounds
    xMax *= levelScaleH;
    xMin *= levelScaleH;
    zMax *= levelScaleH;
    zMin *= levelScaleH;
	// END ADD
    if (pos[0] >= xMax) {
        pos[0] = xMax;
    }
    if (pos[0] <= xMin) {
        pos[0] = xMin;
    }
    if (pos[2] >= zMax) {
        pos[2] = zMax;
    }
    if (pos[2] <= zMin) {
        pos[2] = zMin;
    }
    yaw = calculate_yaw(origin, pos);
    return yaw;
}
