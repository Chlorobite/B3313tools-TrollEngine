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


void create_camera(register struct GraphNodeCamera *gc, struct AllocOnlyPool *pool) {
    register s16 mode = gc->config.mode;
    register struct Camera *c = alloc_only_pool_alloc(pool, sizeof(struct Camera));

    gc->config.camera = c;
    c->mode = mode;
    c->defMode = mode;
    c->cutscene = 0;
    c->doorStatus = DOOR_DEFAULT;
	// EDIT: level scale messes with hardcoded positions, so scale the 'area center' position
    c->areaCenX = gc->focus[0] * levelScaleH;
    c->areaCenY = gc->focus[1] * levelScaleV;
    c->areaCenZ = gc->focus[2] * levelScaleH;
	// END EDIT
    c->yaw = 0;
    vec3f_copy(c->pos, gc->pos);
    vec3f_copy(c->focus, gc->focus);
}
