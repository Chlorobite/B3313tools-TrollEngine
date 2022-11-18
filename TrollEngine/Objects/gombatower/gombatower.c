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

extern s32 obj_update_standard_actions(f32 scale);
extern s32 obj_forward_vel_approach(f32 target, f32 delta);
extern s32 cur_obj_play_sound_at_anim_range(s8 arg0, s8 arg1, u32 sound);
extern BehaviorScript bhvGomba[];

/**
 * Behavior for bhvPokey and bhvPokeyBodyPart.
 * bhvPokey is responsible for the behavior of the pokey itself, as well as
 * spawning the body parts.
 * Pokey comes before its body parts in processing order, and the body parts
 * are processed top to bottom.
 */

#define GOMBA_HEIGHT 116.0f

/**
 * Enter the jump action and set initial y velocity.
 */
static void goomba_begin_jump(void) {
    cur_obj_play_sound_2(SOUND_OBJ_GOOMBA_ALERT);

    o->oAction = GOOMBA_ACT_JUMP;
    o->oForwardVel = 0.0f;
    o->oVelY = 50.0f / 3.0f * 1.5f;
}

/**
 * Move until landing, and rotate toward target yaw.
 */
static void goomba_act_jump(void) {
    obj_resolve_object_collisions(NULL);

    //! If we move outside the goomba's drawing radius the frame it enters the
    //  jump action, then it will keep its velY, but it will still be counted
    //  as being on the ground.
    //  Next frame, the jump action will think it has already ended because it is
    //  still on the ground.
    //  This puts the goomba back in the walk action, but the positive velY will
    //  make it hop into the air. We can then trigger another jump.
    if (o->oMoveFlags & OBJ_MOVE_MASK_ON_GROUND) {
        o->oAction = GOOMBA_ACT_WALK;
    } else {
        cur_obj_rotate_yaw_toward(o->oGoombaTargetYaw, 0x800);
    }
}

/**
 * Hitbox for a single pokey body part.
 */
static struct ObjectHitbox sGombaHitbox = {
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 10,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 72,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 42,
    /* hurtboxHeight:     */ 40,
};

/**
 * Attack handlers for pokey body part.
 */
static u8 sGombaAttackHandlers[] = {
    /* ATTACK_PUNCH:                 */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_KICK_OR_TRIP:          */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_FROM_ABOVE:            */ ATTACK_HANDLER_SQUISHED,
    /* ATTACK_GROUND_POUND_OR_TWIRL: */ ATTACK_HANDLER_SQUISHED,
    /* ATTACK_FAST_ATTACK:           */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_FROM_BELOW:            */ ATTACK_HANDLER_KNOCKBACK,
};

s32 approach_f32_ptr(f32 *px, f32 target, f32 delta);
/**
 * Update function for pokey body part.
 * The behavior parameter is the body part's index from 0 to 4, with 0 at the
 * top.
 */
void bhv_pokey_body_part_update(void) {
    // PARTIAL_UPDATE
    register struct Object *_o = o;
    register struct Object *_o_parentObj = _o->parentObj;
    const register u8 _goombaType = goombaType;
    const register s32 is_fast = _goombaType == 2 || _goombaType == 3;

    s16 offsetAngle;
    register f32 baseHeight;
	register f32 animSpeed;

    if (obj_update_standard_actions(1.5f)) {
        if (_o_parentObj->oAction == POKEY_ACT_UNLOAD_PARTS) {
            obj_mark_for_deletion(_o);
        } else {
            cur_obj_update_floor_and_walls();
            obj_update_blinking(&_o->oPokeyBodyPartBlinkTimer, 30, 50, 5);

            // If the body part above us is dead, then decrease body part index
            // by one, since new parts are spawned from the bottom.
            //! It is possible to briefly get two body parts to have the same
            //  index by killing two body parts on the frame before a new part
            //  spawns, but one of the body parts shifts upward immediately,
            //  so not very interesting
            if (_o->oBehParams2ndByte > 0
                && !(_o_parentObj->oPokeyAliveBodyPartFlags & (1 << (_o->oBehParams2ndByte - 1)))) {
                _o_parentObj->oPokeyAliveBodyPartFlags =
                    _o_parentObj->oPokeyAliveBodyPartFlags | 1 << (_o->oBehParams2ndByte - 1);

                _o_parentObj->oPokeyAliveBodyPartFlags =
                    _o_parentObj->oPokeyAliveBodyPartFlags & ((1 << _o->oBehParams2ndByte) ^ ~0);

                _o->oBehParams2ndByte--;
            }

            // The bottom goomba does the movement for the whole tower.
            else if (_o->oBehParams2ndByte + 1 == _o_parentObj->oPokeyNumAliveBodyParts) {
				_o->oDistanceToMario = lateral_dist_between_objects(_o, gMarioObject);
				//treat_far_home_as_mario(1000.0f);
                
                if (_o_parentObj->oPokeyBottomBodyPartSize < 1.0f) {
                    approach_f32_ptr(&_o_parentObj->oPokeyBottomBodyPartSize, 1.0f, 0.1f);
                    cur_obj_scale(_o_parentObj->oPokeyBottomBodyPartSize * 1.5f);
                }
				
				if ((animSpeed = _o->oForwardVel / 1.5f * 0.4f) < 1.0f) {
					animSpeed = 1.0f;
				}

				cur_obj_init_animation_with_accel_and_sound(0, animSpeed);
				
				if (_o->oAction == GOOMBA_ACT_JUMP) {
					goomba_act_jump();
				}
				else {
					cur_obj_update_floor_and_walls();
					obj_forward_vel_approach(_o->oGoombaRelativeSpeed * 1.5f, 0.4f);
                    
                    if (is_fast || _goombaType == 6) {
                        _o->oDamageOrCoinValue = 3;
                    }
                    
					// If walking fast enough, play footstep sounds
					if (_o->oGoombaRelativeSpeed > 2.0f) {
						cur_obj_play_sound_at_anim_range(2, 17, SOUND_OBJ_GOOMBA_WALK);
					}
					
					//! By strategically hitting a wall, steep slope, or another goomba, we can
					//  prevent the goomba from turning back toward home for a while (goomba
					//  chase extension)
					//! It seems theoretically possible to get 2-3 goombas to repeatedly touch
					//  each other and move arbitrarily far from their home, but it's
					//  extremely precise and chaotic in practice, so probably can't be performed
					//  for nontrivial distances
					if (_o->oGoombaTurningAwayFromWall) {
						_o->oGoombaTurningAwayFromWall = obj_resolve_collisions_and_turn(_o->oGoombaTargetYaw, 0x200);
					} else {
						// If far from home, walk toward home.
						if (_o->oDistanceToMario >= 25000.0f) {
							_o->oGoombaTargetYaw = _o->oAngleToMario;
							_o->oGoombaWalkTimer = random_linear_offset(20, 30);
						}

						if (!(_o->oGoombaTurningAwayFromWall =
								obj_bounce_off_walls_edges_objects(&_o->oGoombaTargetYaw))) {
							if (_o->oDistanceToMario < 1000.0f && _goombaType != 4) {
								// If close to mario, begin chasing him. If not already chasing
								// him, jump first

								if (_o->oGoombaRelativeSpeed <= 2.0f) {
									goomba_begin_jump();
								}

								_o->oGoombaTargetYaw = _o->oAngleToMario;
								if (_goombaType == 5) {
                                    _o->oGoombaTargetYaw += 0x8000; // face away
                                }
                                _o->oGoombaRelativeSpeed = (is_fast || _goombaType == 5) ? 40.0f : 20.0f;
							} else {
								// If mario is far away, walk at a normal pace, turning randomly
								// and occasionally jumping

								_o->oGoombaRelativeSpeed = is_fast ? 2.0f : (4.0f / 3.0f);

								if (_o->oGoombaWalkTimer != 0) {
									_o->oGoombaWalkTimer--;
								} else {
									if (random_u16() & 3) {
										_o->oGoombaTargetYaw = obj_random_fixed_turn(0x2000);
										_o->oGoombaWalkTimer = random_linear_offset(100, 100);
									} else {
										goomba_begin_jump();
										_o->oGoombaTargetYaw = obj_random_fixed_turn(0x6000);
									}
								}
							}
						}

						cur_obj_rotate_yaw_toward(_o->oGoombaTargetYaw, (is_fast ? 0x600 : 0x200));
					}
				}

				cur_obj_move_standard(-78);
				
				// update the tower
				_o_parentObj->oPosX = _o->oPosX;
				_o_parentObj->oPosY = _o->oPosY;
				_o_parentObj->oPosZ = _o->oPosZ;
				_o_parentObj->oVelY = _o->oVelY;
				_o_parentObj->oFaceAngleYaw = _o->oFaceAngleYaw;
				_o_parentObj->oMoveAngleYaw = _o->oMoveAngleYaw;
            }
            // The parts above are just offset like Pokey's parts
            else {
				//! Pausing causes jumps in offset angle
				offsetAngle = _o->oBehParams2ndByte * 0x4000 + gGlobalTimer * 0x800;
				_o->oPosX = _o_parentObj->oPosX + coss(offsetAngle) * 6.0f;
				_o->oPosZ = _o_parentObj->oPosZ + sins(offsetAngle) * 6.0f;
				
				// This is the height of the tower beneath the body part
				baseHeight = _o_parentObj->oPosY
							+ (GOMBA_HEIGHT * (_o_parentObj->oPokeyNumAliveBodyParts - _o->oBehParams2ndByte - 2))
							+ GOMBA_HEIGHT * _o_parentObj->oPokeyBottomBodyPartSize;

				// We treat the base height as a minimum height, allowing the body
				// part to briefly stay in the air after a part below it dies
				if (_o->oPosY < baseHeight) {
					_o->oPosY = baseHeight;
					_o->oVelY = 0.0f;
				}
			}

            _o->oNumLootCoins = _goombaType == 5 ? -1 : 1;

            // If the body part was attacked, then die. If the head was killed,
            // then die after a delay.

            if (obj_handle_attacks(&sGombaHitbox, _o->oAction, sGombaAttackHandlers)) {
                if (_o_parentObj->oPokeyAliveBodyPartFlags | (1 << _o->oBehParams2ndByte))
                    _o_parentObj->oPokeyNumAliveBodyParts--;

                _o_parentObj->oPokeyAliveBodyPartFlags &= ((1 << _o->oBehParams2ndByte) ^ ~0);
            } else {
                // Die in order from top to bottom
                // If a new body part spawns after the head has been killed, its
                // death delay will be 0
                _o->oPokeyBodyPartDeathDelayAfterHeadKilled = (_o->oBehParams2ndByte << 2) + 20;
            }

            cur_obj_move_standard(-78);
        }
    } else {
        _o->oAnimState = 1;
    }

    //_o->oGraphYOffset = _o->header.gfx.scale[1] * 22.0f;
    _o->oFaceAngleYaw = _o_parentObj->oMoveAngleYaw;
    _o->oDeathSound = SOUND_OBJ_ENEMY_DEATH_HIGH;
}

/**
 * When mario gets within range, spawn the 5 body parts and enter the wander
 * action.
 */
static void pokey_act_uninitialized(void) {
    struct Object *bodyPart;
    s32 i, count;

    if (o->oDistanceToMario < 4000.0f) {
		count = o->oPokeyNumAliveBodyParts;
        if (count == 0)
            count = o->oBehParams >> 24;
		if (count == 0)
			count = 5;
		
        for (i = count - 1; i >= 0; i--) {
            // Spawn body parts at y offsets 480, 360, 240, 120, 0
            // behavior param 0 = head, 4 = lowest body part
            bodyPart = spawn_object_relative(i, 0, -i * GOMBA_HEIGHT + (GOMBA_HEIGHT * (count - 1)), 0, o, MODEL_GOOMBA, bhvGomba);

            if (bodyPart != NULL) {
				bodyPart->oHomeX = o->oPosX;
				bodyPart->oHomeY = o->oPosY;
				bodyPart->oHomeZ = o->oPosZ;
                obj_scale(bodyPart, 1.5f);
				o->oPokeyAliveBodyPartFlags |= 1 << i;
            }
        }

        o->oPokeyNumAliveBodyParts = count;
        o->oPokeyBottomBodyPartSize = 1.0f;
        o->oAction = POKEY_ACT_WANDER;
    }
}

/**
 * Wander around, replenishing body parts if they are killed. When mario moves
 * far away, enter the unload parts action.
 * While wandering, if mario is within 2000 units, try to move toward him. But
 * if mario gets too close, then shy away from him.
 */
static void pokey_act_wander(void) {
    s32 targetAngleOffset;

    if (o->oPokeyNumAliveBodyParts == 0) {
        obj_mark_for_deletion(o);
    } else if (o->oDistanceToMario > 4500.0f) {
        o->oAction = POKEY_ACT_UNLOAD_PARTS;
        o->oForwardVel = 0.0f;
    }
    else if (TRACKER_difficulty_modifier > 2.5f) {
        // If a body part is missing, replenish it after 100 frames
        if (o->oPokeyNumAliveBodyParts < (o->oBehParams >> 24 == 0 ? 5 : o->oBehParams >> 24)) {
            if (o->oTimer > 40) {
                // Because the body parts shift index whenever a body part
                // is killed, the new part's index is equal to the number
                // of living body parts

                struct Object *bodyPart
                    = spawn_object_relative(o->oPokeyNumAliveBodyParts, 0, 0, 0, o, MODEL_GOOMBA, bhvGomba);

                if (bodyPart != NULL) {
                    o->oPokeyAliveBodyPartFlags =
                        o->oPokeyAliveBodyPartFlags | (1 << o->oPokeyNumAliveBodyParts);
                    o->oPokeyNumAliveBodyParts++;
                    o->oPokeyBottomBodyPartSize = 0.0f;

                    obj_scale(bodyPart, 0.0f);
                }

                o->oTimer = 0;
            }
        } else {
            o->oTimer = 0;
        }
    }
}

/**
 * Move back to home and enter the uninitialized action.
 * The pokey body parts check to see if pokey is in this action, and if so,
 * unload themselves.
 */
static void pokey_act_unload_parts(void) {
    o->oAction = POKEY_ACT_UNINITIALIZED;
    cur_obj_set_pos_to_home();
}

/**
 * Update function for pokey.
 */
void bhv_pokey_update(void) {
    // PARTIAL_UPDATE

    o->oDeathSound = SOUND_OBJ_POKEY_DEATH;

    switch (o->oAction) {
        case POKEY_ACT_UNINITIALIZED:
            pokey_act_uninitialized();
            break;
        case POKEY_ACT_WANDER:
            pokey_act_wander();
            break;
        case POKEY_ACT_UNLOAD_PARTS:
            pokey_act_unload_parts();
            break;
    }
}
