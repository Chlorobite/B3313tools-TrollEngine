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


s32 execute_mario_action(UNUSED struct Object *o) {
    register s32 inLoop = TRUE;

    if (gMarioState->action) {
        gMarioState->marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
        mario_reset_bodystate(gMarioState);
        update_mario_inputs(gMarioState);
        mario_handle_special_floors(gMarioState);
        mario_process_interactions(gMarioState);

        // If Mario is OOB, stop executing actions.
        if (gMarioState->floor == NULL) {
            return 0;
        }

        // The function can loop through many action shifts in one frame,
        // which can lead to unexpected sub-frame behavior. Could potentially hang
        // if a loop of actions were found, but there has not been a situation found.
        while (inLoop) {
            switch (gMarioState->action & ACT_GROUP_MASK) {
                case ACT_GROUP_STATIONARY:
                    inLoop = mario_execute_stationary_action(gMarioState);
                    break;

                case ACT_GROUP_MOVING:
                    inLoop = mario_execute_moving_action(gMarioState);
                    break;

                case ACT_GROUP_AIRBORNE:
                    inLoop = mario_execute_airborne_action(gMarioState);
                    break;

                case ACT_GROUP_SUBMERGED:
                    inLoop = mario_execute_submerged_action(gMarioState);
                    break;

                case ACT_GROUP_CUTSCENE:
                    inLoop = mario_execute_cutscene_action(gMarioState);
                    break;

                case ACT_GROUP_AUTOMATIC:
                    inLoop = mario_execute_automatic_action(gMarioState);
                    break;

                case ACT_GROUP_OBJECT:
                    inLoop = mario_execute_object_action(gMarioState);
                    break;
            }
        }

        sink_mario_in_quicksand(gMarioState);
        squish_mario_model(gMarioState);
        set_submerged_cam_preset_and_spawn_bubbles(gMarioState);
        update_mario_health(gMarioState);
        update_mario_info_for_cam(gMarioState);
        mario_update_hitbox_and_cap_model(gMarioState);

        // Both of the wind handling portions play wind audio only in
        // non-Japanese releases.
        if (gMarioState->floor->type == SURFACE_HORIZONTAL_WIND) {
            spawn_wind_particles(0, (gMarioState->floor->force << 8));
            play_sound(SOUND_ENV_WIND2, gMarioState->marioObj->header.gfx.cameraToObject);
        }

        if (gMarioState->floor->type == SURFACE_VERTICAL_WIND) {
            spawn_wind_particles(1, 0);
            play_sound(SOUND_ENV_WIND2, gMarioState->marioObj->header.gfx.cameraToObject);
        }

        play_infinite_stairs_music();
        gMarioState->marioObj->oInteractStatus = 0;

        TRACKER_record_mario_state(gMarioState);

        return gMarioState->particleFlags;
    }

    return 0;
}
