#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "game_init.h"
#include "interaction.h"
#include "level_table.h"
#include "level_update.h"
#include "main.h"
#include "mario.h"
#include "mario_actions_airborne.h"
#include "mario_actions_automatic.h"
#include "mario_actions_cutscene.h"
#include "mario_actions_moving.h"
#include "mario_actions_object.h"
#include "mario_actions_stationary.h"
#include "mario_actions_submerged.h"
#include "mario_misc.h"
#include "mario_step.h"
#include "memory.h"
#include "object_fields.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "print.h"
#include "save_file.h"
#include "sound_init.h"
#include "rumble_init.h"

#include "AI/stats_tracking.h"
#include "AI/personalization_helpers.h"

void TRACKER_handle_mario_state_change(struct MarioState *m);

u32 set_mario_action(struct MarioState *m, u32 action, u32 actionArg) {
    switch (action & ACT_GROUP_MASK) {
        case ACT_GROUP_MOVING:
            action = set_mario_action_moving(m, action, actionArg);
            break;

        case ACT_GROUP_AIRBORNE:
            action = set_mario_action_airborne(m, action, actionArg);
            break;

        case ACT_GROUP_SUBMERGED:
            action = set_mario_action_submerged(m, action, actionArg);
            break;

        case ACT_GROUP_CUTSCENE:
            action = set_mario_action_cutscene(m, action, actionArg);
            break;
    }

    // Resets the sound played flags, meaning Mario can play those sound types again.
    m->flags &= ~(MARIO_ACTION_SOUND_PLAYED | MARIO_MARIO_SOUND_PLAYED);

    if (!(m->action & ACT_FLAG_AIR)) {
        m->flags &= ~MARIO_UNKNOWN_18;
    }

    // Initialize the action information.
    m->prevAction = m->action;
    m->action = action;
    m->actionArg = actionArg;
    m->actionState = 0;
    m->actionTimer = 0;

	// ADD: personalization AI, see AI/stats_tracking.c
    TRACKER_handle_mario_state_change(m);
	// END ADD

    return TRUE;
}
