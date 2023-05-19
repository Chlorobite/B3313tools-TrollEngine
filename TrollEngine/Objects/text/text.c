#include <PR/ultratypes.h>

#include "sm64.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "game_init.h"
#include "geo_misc.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_table.h"
#include "level_update.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "obj_behaviors_2.h"
#include "object_constants.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "platform_displacement.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "seq_ids.h"
#include "spawn_sound.h"

#include "AI/personalization_helpers.h"
#include "AI/stats_tracking.h"

#define POS_OP_SAVE_POSITION 0
#define POS_OP_COMPUTE_VELOCITY 1
#define POS_OP_RESTORE_POSITION 2

#define o gCurrentObject



char *strings[] = {
    "YOU ARE NOT WORTHY",
    "YOU ARE WORTHY",
    "GET ME OUT",
    "TURN IT OFF",
    "IT HURTS",
    "GET OUT",
    "FIND THE CORE",
    "DESTROY IT",
    "ENHANCER OFFLINE",
    "ENHANCER LIVE"
};


void bhv_text_on_screen_loop(void) {
    if (o->oBehParams & 0x0200 && random_u16() & 1)
        return;

    if (o->oDistanceToMario > ((o->oBehParams >> 24) & 0xFF) * 10.f) return;

    print_text_centered(160, (o->oBehParams & 0x0100) ? 120 : 40, strings[(o->oBehParams >> 16) & 0xFF]);
}

