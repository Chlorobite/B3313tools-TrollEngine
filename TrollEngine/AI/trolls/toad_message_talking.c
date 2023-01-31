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
#include "ingame_menu.h"
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

#include "AI/personalization_helpers.h"

enum ToadMessageStates {
    TOAD_MESSAGE_FADED,
    TOAD_MESSAGE_OPAQUE,
    TOAD_MESSAGE_OPACIFYING,
    TOAD_MESSAGE_FADING,
    TOAD_MESSAGE_TALKING
};


void toad_message_talking() {
	// new function too big for our changes; implementation in AI/personalization_helpers.c
    troll_toad_message_talking();
}
