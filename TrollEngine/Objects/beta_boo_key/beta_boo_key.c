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



/**
 * Update the key to be inside its parent boo, and handle the boo dying.
 */
static void beta_boo_key_inside_boo_loop(void) {
    // Update the key to be inside the boo at all times
	struct Object *parent = o->parentObj;
    obj_copy_pos(o, parent);

    o->oPosY += 50.0f * parent->oBooBaseScale;

    // Hide/show depending on parent object
    if (parent->header.gfx.node.flags & GRAPH_RENDER_INVISIBLE) {
        cur_obj_hide();
    }
    else {
        cur_obj_unhide();
    }

    // If the boo is dying/dead, set the action to BETA_BOO_KEY_ACT_DROPPING.
    if (parent->oBooDeathStatus != BOO_DEATH_STATUS_ALIVE) {
        o->oAction = BETA_BOO_KEY_ACT_DROPPING;
    }

    // Rotate the key
    o->oFaceAngleRoll += 0x200;
    o->oFaceAngleYaw += 0x200;
}

/**
 * Drop the key. This function is run once, the frame after the boo dies;
 * It immediately sets the action to BETA_BOO_KEY_ACT_DROPPED.
 */
static void beta_boo_key_drop(void) {
    s16 velocityDirection;
    f32 velocityMagnitude;

    // Update the key to be inside the boo
    struct Object *parent = o->parentObj;
    obj_copy_pos(o, parent);

    // This if statement to only run this code on the first frame
    // is redundant, since it instantly sets the action to BETA_BOO_KEY_ACT_DROPPED
    // which stops this function from running again.
    if (o->oTimer == 0) {
        // Separate from the parent boo
        o->parentObj = parent->parentObj;

        o->oAction = BETA_BOO_KEY_ACT_DROPPED;

        // Make the key move laterally away from Mario at 3 units/frame
        // (as if he transferred kinetic energy to it)
        velocityDirection = gMarioObject->oMoveAngleYaw;
        velocityMagnitude = 3.0f;

        o->oVelX = sins(velocityDirection) * velocityMagnitude;
        o->oVelZ = coss(velocityDirection) * velocityMagnitude;

        // Give it an initial Y velocity of 40 units/frame
        o->oVelY = 40.0f;
    }

    // Rotate the key
    o->oFaceAngleYaw += 0x200;
    o->oFaceAngleRoll += 0x200;
}

/**
 * Continue to make the key fall, and handle collection.
 */
static void beta_boo_key_dropped_loop(void) {
    // Apply standard physics to the key
    cur_obj_update_floor_and_walls();
    cur_obj_move_standard(78);

    // Slowly increase the Y offset to make the model aligned correctly.
    // This is spread out over 13 frames so that it's not noticable.
    if (o->oGraphYOffset < 26.0f) {
        o->oGraphYOffset += 2.0f;
    }

    // Transition from rotating in both the yaw and the roll axes
    // to just in the yaw axis. This is done by truncating the key's roll
    // to the nearest multiple of 0x800, then continuously adding 0x800
    // until it reaches a multiple of 0x10000, at which point &-ing with
    // 0xFFFF returns 0 and the key stops rotating in the roll direction.
    if (o->oFaceAngleRoll & 0xFFFF) {
        o->oFaceAngleRoll &= 0xF800;
        o->oFaceAngleRoll += 0x800;
    }

    // Once the key stops bouncing, stop its horizontal movement on the ground.
    if (o->oMoveFlags & OBJ_MOVE_ON_GROUND) {
        o->oVelX = 0.0f;
        o->oVelZ = 0.0f;
    }

    // Rotate the key
    o->oFaceAngleYaw += 0x800;

    // If the key hits the floor or 90 frames have elapsed since it was dropped,
    // become tangible and handle collision.
    if (o->oTimer > 90 || o->oMoveFlags & OBJ_MOVE_LANDED) {
        cur_obj_become_tangible();

        if (obj_check_if_collided_with_object(o, gMarioObject)) {
			int index;
            // This interaction status is 0x01, the first interaction status flag.
            // It was only used for Hoot in the final game, but it seems it could've
            // done something else or held some special meaning in beta.
            // Earlier, in beta_boo_key_drop (called when the parent boo is killed),
            // o->parentObj is set to the parent boo's parentObj. This means that
            // here, the parentObj is actually the parent of the old parent boo.
            // One theory about this code is that there was a boo spawner, which
            // spawned "false" boos and one "true" boo with the key, and the player
            // was intended to find the one with the key to progress.
            o->parentObj->oInteractStatus = 0x01;

			//gMarioState->numKeys++;
			index = o->oBehParams >> 24;
			save_file_collect_beta_key (index);

            // Delete the object and spawn sparkles
            obj_mark_for_deletion(o);
            spawn_object(o, MODEL_SPARKLES, bhvGoldenCoinSparkles);

			//set_mario_action(gMarioState, ACT_STAR_DANCE_NO_EXIT, 3);
			//gMarioState->actionState = 3;
			play_sound(SOUND_GENERAL_VANISH_SFX, gMarioState->marioObj->header.gfx.cameraToObject);
			//play_sound(SOUND_MENU_STAR_SOUND, gMarioState->marioObj->header.gfx.cameraToObject);

			/*if (save_file_get_total_betakey_count(gCurrSaveFileNum - 1) >= 6 && index < 8)
			{
				bhv_spawn_star_no_level_exit(1);
			}*/
        }
    }
}


static void (*sBetaBooKeyActions[])(void) = { beta_boo_key_inside_boo_loop, beta_boo_key_drop, beta_boo_key_dropped_loop };

/**
 * Update function for bhvBetaBooKey.
 */
void bhv_beta_boo_key_loop(void) {
    cur_obj_call_action_function(sBetaBooKeyActions);
}
