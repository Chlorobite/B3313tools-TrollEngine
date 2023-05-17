#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "envfx_snow.h"
#include "game_init.h"
#include "goddard/renderer.h"
#include "interaction.h"
#include "level_update.h"
#include "mario_actions_cutscene.h"
#include "mario_misc.h"
#include "memory.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "skybox.h"
#include "sound_init.h"

#include "AI/stats_tracking.h"
extern u8 gTrollWarpDestination;


Gfx *geo_draw_mario_head_goddard(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c) {
    Gfx *gfx = NULL;
    s16 sfx = 0;
    struct GraphNodeGenerated *asGenerated = (struct GraphNodeGenerated *) node;

    if (callContext == GEO_CONTEXT_RENDER) {
        // ADD: reset painting variables and level scale to fix issues
        TRACKER_level_scale_modifier_h = 1.0f;
        TRACKER_level_scale_modifier_v = 1.0f;
        gTrollWarpDestination = 0;
        // END ADD

        if (gPlayer1Controller->controllerData != NULL && !gWarpTransition.isActive) {
            gd_copy_p1_contpad(gPlayer1Controller->controllerData);
        }
        gfx = (Gfx *) PHYSICAL_TO_VIRTUAL(gdm_gettestdl(asGenerated->parameter));
        gGoddardVblankCallback = gd_vblank;
        sfx = gd_sfx_to_play();
        play_menu_sounds(sfx);
    }
    return gfx;
}
