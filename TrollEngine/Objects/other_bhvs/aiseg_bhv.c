#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "course_table.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "envfx_bubbles.h"
#include "game_init.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_misc_macros.h"
#include "level_table.h"
#include "level_update.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "mario_misc.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "spawn_object.h"
#include "spawn_sound.h"
#include "rumble_init.h"

/**
 * @file obj_behaviors.c
 * This file contains a portion of the obj behaviors and many helper functions for those
 * specific behaviors. Few functions besides the bhv_ functions are used elsewhere in the repo.
 */

#define o gCurrentObject

#define OBJ_COL_FLAG_GROUNDED   (1 << 0)
#define OBJ_COL_FLAG_HIT_WALL   (1 << 1)
#define OBJ_COL_FLAG_UNDERWATER (1 << 2)
#define OBJ_COL_FLAG_NO_Y_VEL   (1 << 3)
#define OBJ_COL_FLAGS_LANDED    (OBJ_COL_FLAG_GROUNDED | OBJ_COL_FLAG_NO_Y_VEL)



void bhv_playma_sound_loop() {
    if (gCamera->mode == CAMERA_MODE_BEHIND_MARIO) {
        return;
    }
    
    cur_obj_play_sound_1(o->oBehParams);
}

void bhv_shakema_camera_loop() {
    switch (o->oBehParams2ndByte) {
        case 0: // smal shake
            cur_obj_shake_screen(SHAKE_POS_SMALL);
            break;
        case 1: // medium shake
            cur_obj_shake_screen(SHAKE_POS_MEDIUM);
            break;
        case 2: // large shake
            cur_obj_shake_screen(SHAKE_POS_LARGE);
            break;
    }
    if (o->oBehParams & 0xFF) {
        // attach to the object previous in the list (so the next object)
        struct Object *attachTo = o + 1;
        o->oPosX = attachTo->oPosX;
        o->oPosY = attachTo->oPosY;
        o->oPosZ = attachTo->oPosZ;
        
        if (attachTo->activeFlags & ACTIVE_FLAG_DEACTIVATED) {
            o->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // unload
        }
    }
}

void bhv_movema_object_move_bparams_to_fields() {
    o->oLeftVel = (s8)(o->oBehParams >> 24);
    o->oUpVel = (s8)(o->oBehParams >> 16);
    o->oForwardVel = (s8)(o->oBehParams >> 8);
    o->OBJECT_FIELD_U32(0x04) = (u8)o->oBehParams;
}

void bhv_movema_object_loop() {
    u32 trollfield = o->OBJECT_FIELD_U32(0x04);
    s32 timer = trollfield & 0x0FFF;
    s32 shakema_playma_flag = trollfield & 0x1000;
    
    if (timer != 0 && o->oTimer >= timer) {
        o->oTimer = 0;
        o->oLeftVel *= -1.0f;
        o->oUpVel *= -1.0f;
        o->oForwardVel *= -1.0f;
        
        if (shakema_playma_flag) {
            o->OBJECT_FIELD_U32(0x04) ^= 0x2000;
            if (o->OBJECT_FIELD_U32(0x04) & 0x2000) {
                // shakema
                cur_obj_shake_screen(SHAKE_POS_SMALL);
                // playma
                cur_obj_play_sound_2(SOUND_GENERAL_UNK46);
            }
        }
    }
    
    o->oVelX = o->oLeftVel;
    o->oVelY = o->oUpVel;
    o->oVelZ = o->oForwardVel;
    o->oPosX += o->oVelX;
    o->oPosY += o->oVelY;
    o->oPosZ += o->oVelZ;
}

void bhv_radius_dialog_box_loop() {
    register s32 range = (o->oBehParams >> 24) * 10;
    register s32 dialogID = o->oBehParams2ndByte;
    register s32 flags = (u8)(o->oBehParams >> 8);
    
    if (flags & 0x01) {
        // Dialog appears only if no stars collected
        // Check for any collected stars
        register s32 i;
        register struct ObjectNode *listHead;
        register struct Object *obj;

        for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
            listHead = &gObjectLists[i];

            obj = (struct Object *) listHead->next;

            while (obj != (struct Object *) listHead) {
                if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && (u32)obj->header.gfx.sharedChild == (u32)gLoadedGraphNodes[MODEL_TRANSPARENT_STAR]) {
                    // deth
                    o->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // unload
                    return;
                }
                obj = (struct Object *) obj->header.next;
            }
        }
    }
    
    if (o->oDistanceToMario < range) {
        if (flags & 0x04) {
            // freezema game flag, simple
            level_set_transition(-1, NULL);
            create_dialog_box(dialogID);
            o->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // unload
        }
        else {
            register s32 savema = flags & 0x02;
            register s32 response = cur_obj_update_dialog(MARIO_DIALOG_LOOK_FRONT, savema ? DIALOG_FLAG_TEXT_RESPONSE : DIALOG_FLAG_TEXT_DEFAULT, dialogID, 0);

            if (response) {
                if (savema && response == DIALOG_RESPONSE_YES) {
                    save_file_do_save(gCurrSaveFileNum - 1);
                }
                
                o->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // unload
            }
        }
    }
}

void bhv_trolled_donut_platform_update() {
    if (o->oDistanceToMario > 6500.0f) {
        cur_obj_set_pos_to_home();
        cur_obj_hide();
        o->oGravity = 0;
        o->oTimer = 0;
    }
    else {
        if (o->oTimer != 0 && o->oMoveFlags & 0x00000003) {
            if (!(o->header.gfx.node.flags & GRAPH_RENDER_INVISIBLE)) {
                spawn_mist_particles_variable(0, 0, 150.0f);
                spawn_triangle_break_particles(30, MODEL_DIRT_ANIMATION, 3.0f, 4);
                obj_spawn_loot_yellow_coins(o, o->oNumLootCoins, 20.0f);
                
                cur_obj_hide();
                
                create_sound_spawner(SOUND_GENERAL_DONUT_PLATFORM_EXPLOSION);
            }
        } else {
            if (o->oGravity == 0.0f) {
                if (gMarioObject->platform == o) {
                    cur_obj_shake_y(4.0f);
                    if (o->oTimer > 15) {
                        o->oGravity = -0.1f;
                    }
                } else {
                    cur_obj_set_pos_to_home();
                    o->oTimer = 0;
                }
            } else {
                cur_obj_update_floor_and_walls();
                cur_obj_move_standard(78);
            }

            cur_obj_unhide();
            load_object_collision_model();
            o->oCollisionDistance = 0x7FFF;
        }
    }
}


static struct ObjectHitbox sSilverStarHitbox = {
    /* interactType:      */ INTERACT_STAR_OR_KEY,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 80,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

extern const BehaviorScript bhvHiddenSilverStarStar[];
extern s32 isSilverStar;

void bhv_silver_star_init() {
    o->oFaceAngleYaw = 0;
    o->oAction = 1;
    o->oInteractionSubtype |= INT_SUBTYPE_NO_EXIT | 0x8000;
    o->oBuoyancy = 1200;
    obj_set_hitbox(o, &sSilverStarHitbox);
    o->oWallHitboxRadius = 80;
}

void bhv_silver_star_loop() {
    o->oFaceAngleYaw += 0x800;
    if (o->oFaceAngleYaw & 0x800) {
        o->oAnimState++;
    }
    
    if (o->oAction == 1) {
        o->oGravity = -1.f;
        cur_obj_update_floor_and_walls();
        cur_obj_if_hit_wall_bounce_away();
        cur_obj_move_standard(78);
        
        if (o->oMoveFlags & (0x00000003 | OBJ_MOVE_AT_WATER_SURFACE)) {
            cur_obj_play_sound_2(SOUND_GENERAL_SHORT_STAR & ~SOUND_LOWER_BACKGROUND_MUSIC);
            o->oVelY = 20.f;
            o->oForwardVel = 20.f;
            
            if (((o->oPosX - o->oHomeX) * (o->oPosX - o->oHomeX) + (o->oPosZ - o->oHomeZ) * (o->oPosZ - o->oHomeZ)) > 2500.f * 2500.f)
                o->oMoveAngleYaw = cur_obj_angle_to_home();
            else if ((random_u16() & 3) && o->oDistanceToMario < 1000.f)
                o->oMoveAngleYaw = 0x8000 + o->oAngleToMario;
            else
                o->oMoveAngleYaw = random_u16();
            
            if (random_u16() & 1)
                o->oMoveAngleYaw += (random_u16() & 0x1FFF);
            else
                o->oMoveAngleYaw -= (random_u16() & 0x1FFF);
        }
    }

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        isSilverStar = TRUE;
        mark_obj_for_deletion(o);
        o->oInteractStatus = 0;
    }
}


void bhv_hidden_silver_star_star_init() {
    spawn_object(o, MODEL_TRANSPARENT_STAR, bhvRedCoinStarMarker);

    o->oHiddenStarTriggerCounter = 0;
}

void spawn_red_coin_cutscene_star(f32 x, f32 y, f32 z);

void bhv_hidden_silver_star_star_loop() {
    switch (o->oAction) {
        case 0:
            if (o->oHiddenStarTriggerCounter == 5) {
                o->oAction = 1;
            }
            break;

        case 1:
            if (o->oTimer > 2) {
                spawn_red_coin_cutscene_star(o->oPosX, o->oPosY, o->oPosZ);
                o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
            }
            break;
    }
}

void bhv_star_magnet_two() {
    register f32 starMagnetDist = 33130.f;
	register s32 i;
	register struct Object *obj;
    register struct ObjectNode *listHead;
	register struct Object *closestObject = NULL;

    // Find closest hidden star spawner, if any.
	for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
		listHead = &gObjectLists[i];

		obj = (struct Object *) listHead->next;

		while (obj != (struct Object *) listHead) {
            if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && is_object_star_spawner(obj)) {
                register f32 dist = dist_between_objects(o, obj);

                if (dist < starMagnetDist) {
                    starMagnetDist = dist;
                    closestObject = obj;
                }
            }
            obj = (struct Object *) obj->header.next;
        }
	}

    if (closestObject != NULL) {
        // Unload any star markers, if any.
        register s32 spawnMarker = FALSE;

        for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
            listHead = &gObjectLists[i];

            obj = (struct Object *) listHead->next;

            while (obj != (struct Object *) listHead) {
                if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && obj != closestObject && obj->parentObj == closestObject) {
                    obj->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // unload
                    spawnMarker = TRUE;
                }
                obj = (struct Object *) obj->header.next;
            }
        }

        closestObject->oPosX = o->oPosX;
        closestObject->oPosY = o->oPosY;
        closestObject->oPosZ = o->oPosZ;
        closestObject->oBehParams = o->oBehParams;
        closestObject->oBehParams2ndByte = o->oBehParams2ndByte;

        if (spawnMarker) {
            obj = o;
            o = closestObject; // just in case
            spawn_object(closestObject, MODEL_TRANSPARENT_STAR, bhvRedCoinStarMarker);
            o = obj;
        }
    }

    o->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // troll complete, unload
}


void bhv_fly_guy_boss_spawner_loop() {
    f32 distance = 99999.0f;
    struct Object *flyGuy = cur_obj_find_nearest_object_with_behavior(bhvFlyGuy, &distance);

    if (flyGuy == NULL) {
        spawn_object(o, 220, ((void*)0x00405260));
        o->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // troll complete, unload
    }
}

void bhv_set_act_one() {
    gCurrActNum = 1;
    o->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // troll complete, unload
}
