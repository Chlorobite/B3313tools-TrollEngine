#include <PR/ultratypes.h>

#include "prevent_bss_reordering.h"
#include "sm64.h"
#include "area.h"
#include "audio/external.h"
#include "behavior_data.h"
#include "camera.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "game_init.h"
#include "gfx_dimensions.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_table.h"
#include "level_update.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "mario_actions_moving.h"
#include "mario_step.h"
#include "moving_texture.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "save_file.h"
#include "seq_ids.h"
#include "sound_init.h"
#include "rumble_init.h"


s32 check_for_instant_quicksand(struct MarioState *m) {
    if (m->floor->type == SURFACE_INSTANT_QUICKSAND && m->action & ACT_FLAG_INVULNERABLE
        && m->action != ACT_QUICKSAND_DEATH) {
        update_mario_sound_and_camera(m);
        return drop_and_set_mario_action(m, ACT_QUICKSAND_DEATH, 0);
    }
    return FALSE;
}
