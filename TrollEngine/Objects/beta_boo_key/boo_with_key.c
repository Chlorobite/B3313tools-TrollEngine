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



void boo_chase_mario(f32 a0, s16 a1, f32 a2);

extern const BehaviorScript bhvBooWithKey[];
extern const BehaviorScript bhvBigBooWithKey[];



//! Key-boo
static void boo_with_key_act_0(void) {
    o->oBooParentBigBoo = NULL;
    if (o->oBehParams2ndByte == 1)
        o->oBooParentBigBoo = cur_obj_nearest_object_with_behavior(bhvBigBooWithKey);
    o->oBooTargetOpacity = 0xFF;
    o->oBooBaseScale = 1.2f;

	// Fix longtime bug.
	//When the boo is stopped, ideally you want to reset
	//its velocity so it doesn't go to space and beyond.
	o->oVelY = 0;

    cur_obj_scale(1.2f);
    cur_obj_become_tangible();

    if (boo_should_be_active()) {
        o->oAction = 1;
    }
}

static void boo_with_key_act_1(void) {
    s32 attackStatus;

    boo_chase_mario(100.0f, 512, 0.5f);

    attackStatus = boo_get_attack_status();

    if (boo_should_be_stopped()) {
        o->oAction = 0;
    }

    if (attackStatus == BOO_BOUNCED_ON) {
        o->oAction = 2;
    }

    if (attackStatus == BOO_ATTACKED) {
        o->oAction = 3;
    }
}

static void boo_with_key_act_2(void) {
    if (boo_update_after_bounced_on(20.0f)) {
        o->oAction = 1;
    }
}

static void boo_with_key_act_3(void) {
    if (boo_update_during_death()) {
        if (o->oBooParentBigBoo != NULL)
            o->oAction = 4;
        else
            obj_mark_for_deletion(o);
    }
}

static void boo_with_key_act_4(void) {
    s32 dialogID;

    // If there are no remaining "minion" boos, show the dialog of the Big Boo
    if (cur_obj_nearest_object_with_behavior(bhvBooWithKey) == NULL) {
        dialogID = DIALOG_108;
    } else {
        dialogID = DIALOG_107;
    }

    if (cur_obj_update_dialog(MARIO_DIALOG_LOOK_UP, DIALOG_FLAG_TEXT_DEFAULT, dialogID, 0)) {
        create_sound_spawner(SOUND_OBJ_DYING_ENEMY1);
        obj_mark_for_deletion(o);

        if (dialogID == DIALOG_108) { // If the Big Boo should spawn, play the jingle
            play_puzzle_jingle();
        }
    }
}

void bhv_boo_with_key_init(void) {
    struct Object* key;

	// check the key ID
	int keyID = o->oBehParams >> 24;
	int modelID = MODEL_BETA_BOO_KEY;
	if (save_file_get_betakey_flags(gCurrSaveFileNum - 1) & (1 << keyID))
		modelID = MODEL_CUSTOM_BETA_BOO_KEY_B;

    key = spawn_object(o, modelID, bhvBetaBooKey);
    obj_scale_xyz(key, 0.75f, 0.75f, 0.75f);
    key->oBehParams = o->oBehParams;
}

void bhv_big_boo_with_key_init(void) {
    struct Object* key;

	// check the key ID
	int keyID = o->oBehParams >> 24;
	int modelID = MODEL_BETA_BOO_KEY;
	if (save_file_get_betakey_flags(gCurrSaveFileNum - 1) & (1 << keyID))
		modelID = MODEL_CUSTOM_BETA_BOO_KEY_B;

    key = spawn_object(o, modelID, bhvBetaBooKey);
    //obj_scale_xyz(key, 2.f, 2.f, 2.f);
    key->oBehParams = o->oBehParams;

    o->oBigBooNumMinionBoosKilled = 0; // this is set by the bhv, but to prevent shifting the small boos' behavior address, fix it here instead. Honestly should have done it here since the beginning
}


static void (*sBooWithKeyActions[])(void) = {
    boo_with_key_act_0,
    boo_with_key_act_1,
    boo_with_key_act_2,
    boo_with_key_act_3,
    boo_with_key_act_4
};

void bhv_boo_with_key_loop(void)
{
    //PARTIAL_UPDATE

    cur_obj_update_floor_and_walls();
    cur_obj_call_action_function(sBooWithKeyActions);
    cur_obj_move_standard(78);

    boo_approach_target_opacity_and_update_scale();
    o->oInteractStatus = 0;
}

