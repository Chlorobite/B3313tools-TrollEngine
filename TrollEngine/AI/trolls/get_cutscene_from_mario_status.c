#include <ultra64.h>

#define INCLUDED_FROM_CAMERA_C

#include "prevent_bss_reordering.h"
#include "sm64.h"
#include "camera.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "audio/external.h"
#include "mario_misc.h"
#include "game_init.h"
#include "hud.h"
#include "engine/math_util.h"
#include "area.h"
#include "engine/surface_collision.h"
#include "engine/behavior_script.h"
#include "level_update.h"
#include "ingame_menu.h"
#include "mario_actions_cutscene.h"
#include "save_file.h"
#include "object_helpers.h"
#include "print.h"
#include "spawn_sound.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "object_list_processor.h"
#include "paintings.h"
#include "engine/graph_node.h"
#include "level_table.h"

#define CBUTTON_MASK (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS)


extern u8 sObjectCutscene;
extern struct PlayerCameraState *sMarioCamState;
extern s32 gCurrLevelArea;
extern u32 gPrevLevel;


// BAD NAME, rename to get_cutscene_from_mario_status, and remove from trollfinity.asm if it conflicts
u8 troll_get_cutscene_from_mario_status(struct Camera *c) {
    u8 cutscene = c->cutscene;

    if (cutscene == 0) {
        // A cutscene started by an object, if any, will start if nothing else happened
        cutscene = sObjectCutscene;
        sObjectCutscene = 0;
        if (sMarioCamState->cameraEvent == CAM_EVENT_DOOR) {
            switch (gCurrLevelArea) {
				// REMOVE: case AREA_CASTLE_LOBBY, handled by default case now
                case AREA_BBH:
                    //! Castle Lobby uses 0 to mean 'no special modes', but BBH uses 1...
                    if (c->doorStatus == DOOR_LEAVING_SPECIAL) {
                        cutscene = open_door_cutscene(CUTSCENE_DOOR_PULL, CUTSCENE_DOOR_PUSH);
                    } else {
                        cutscene = open_door_cutscene(CUTSCENE_DOOR_PULL_MODE, CUTSCENE_DOOR_PUSH_MODE);
                    }
                    break;
                default:
					// EDIT: make doors check for camera triggers in areas that were set up for real camera
                    if (c->doorStatus == DOOR_LEAVING_SPECIAL) {
                        cutscene = open_door_cutscene(CUTSCENE_DOOR_PULL_MODE, CUTSCENE_DOOR_PUSH_MODE);
                    } else {
                        cutscene = open_door_cutscene(CUTSCENE_DOOR_PULL, CUTSCENE_DOOR_PUSH);
                    }
					// END EDIT
                    break;
            }
        }
        if (sMarioCamState->cameraEvent == CAM_EVENT_DOOR_WARP) {
            cutscene = CUTSCENE_DOOR_WARP;
        }
        if (sMarioCamState->cameraEvent == CAM_EVENT_CANNON) {
            cutscene = CUTSCENE_ENTER_CANNON;
        }
		// REMOVE: real beta, removed CUTSCENE_ENTER_PAINTING
        switch (sMarioCamState->action) {
			// EDIT: real beta, add actionTimer checks to fix the cutscenes running twice on exiting a painting
            case ACT_DEATH_EXIT:
				if (gMarioState->actionTimer < 10) {
					cutscene = CUTSCENE_DEATH_EXIT;
				}
                break;
            case ACT_EXIT_AIRBORNE:
                if (gMarioState->actionTimer < 10) {
					cutscene = CUTSCENE_EXIT_PAINTING_SUCC;
				}
				break;
			// END EDIT
            case ACT_SPECIAL_EXIT_AIRBORNE:
                if (gPrevLevel == LEVEL_BOWSER_1 || gPrevLevel == LEVEL_BOWSER_2
                    || gPrevLevel == LEVEL_BOWSER_3) {
                    cutscene = CUTSCENE_EXIT_BOWSER_SUCC;
                } else {
                    cutscene = CUTSCENE_EXIT_SPECIAL_SUCC;
                }
                break;
            case ACT_SPECIAL_DEATH_EXIT:
                if (gPrevLevel == LEVEL_BOWSER_1 || gPrevLevel == LEVEL_BOWSER_2
                    || gPrevLevel == LEVEL_BOWSER_3) {
                    cutscene = CUTSCENE_EXIT_BOWSER_DEATH;
                } else {
                    cutscene = CUTSCENE_NONPAINTING_DEATH;
                }
                break;
            case ACT_ENTERING_STAR_DOOR:
                if (c->doorStatus == DOOR_DEFAULT) {
                    cutscene = CUTSCENE_SLIDING_DOORS_OPEN;
                } else {
                    cutscene = CUTSCENE_DOOR_PULL_MODE;
                }
                break;
            case ACT_UNLOCKING_KEY_DOOR:
                cutscene = CUTSCENE_UNLOCK_KEY_DOOR;
                break;
            case ACT_WATER_DEATH:
                cutscene = CUTSCENE_WATER_DEATH;
                break;
            case ACT_DEATH_ON_BACK:
                cutscene = CUTSCENE_DEATH_ON_BACK;
                break;
            case ACT_DEATH_ON_STOMACH:
                cutscene = CUTSCENE_DEATH_ON_STOMACH;
                break;
            case ACT_STANDING_DEATH:
                cutscene = CUTSCENE_STANDING_DEATH;
                break;
            case ACT_SUFFOCATION:
                cutscene = CUTSCENE_SUFFOCATION_DEATH;
                break;
            case ACT_QUICKSAND_DEATH:
                cutscene = CUTSCENE_QUICKSAND_DEATH;
                break;
            case ACT_ELECTROCUTION:
                cutscene = CUTSCENE_STANDING_DEATH;
                break;
			// EDIT: real beta, normal star dances just always use CUTSCENE_DANCE_DEFAULT
            case ACT_STAR_DANCE_EXIT:
            case ACT_STAR_DANCE_WATER:
            case ACT_STAR_DANCE_NO_EXIT:
                cutscene = CUTSCENE_DANCE_DEFAULT;
                break;
			// END EDIT
        }
        switch (sMarioCamState->cameraEvent) {
            case CAM_EVENT_START_INTRO:
                cutscene = CUTSCENE_INTRO_PEACH;
                break;
            case CAM_EVENT_START_GRAND_STAR:
                cutscene = CUTSCENE_GRAND_STAR;
                break;
            case CAM_EVENT_START_ENDING:
                cutscene = CUTSCENE_ENDING;
                break;
            case CAM_EVENT_START_END_WAVING:
                cutscene = CUTSCENE_END_WAVING;
                break;
            case CAM_EVENT_START_CREDITS:
                cutscene = CUTSCENE_CREDITS;
                break;
        }
    }
    //! doorStatus is reset every frame. CameraTriggers need to constantly set doorStatus
    c->doorStatus = DOOR_DEFAULT;

    return cutscene;
}
