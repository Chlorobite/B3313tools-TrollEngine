#include <PR/ultratypes.h>
#include "sm64.h"
#include "stats_tracking.h"
#include "audio_trolls.h"
#include "personalization_helpers.h"
#include "post_object_load_pass.h"
#include "game/interaction.h"
#include "game/level_update.h"
#include "game/memory.h"
#include "game/paintings.h"
#include "game/moving_texture.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/save_file.h"
#include "game/spawn_object.h"
#include "engine/behavior_script.h"
#include "engine/surface_collision.h"
#include "behavior_data.h"
#include "course_table.h"

#include "actors/common1.h"
#include "make_const_nonconst.h"

u8 lsd_textures = FALSE;
u8 flip_textures = FALSE;
u8 normalize_textures = FALSE;
u8 lava_skeeter = FALSE;
u32 *foundTextures[256];
u32 foundTextureSamples[256];

extern struct AllocOnlyPool *sLevelPool;
//extern const GeoLayout personalized_skeeter_geo[];
extern u16 gRandomSeed16;
extern struct Painting *paintings[];
extern u8 *tex_stars_start;
extern u8 *moto_bhv_start;
extern u8 *moto_model_normal_start;
extern u8 *moto_model_ice_start;
extern const BehaviorScript bhvHiddenSilverStarStar[];
extern void *powermeter_last_romstart;

static void crawlVertices(u8 *ptr, u16 count) {
	register s32 i;
	
	for (i = 0; i < count; i++) {
		// do something in vertex lo l
		ptr += 0x10;
	}
}

static void crawlDisplayList(u32 *ptr) {
	register u32 *trollptr;
	u16 vtxSize;
	register s32 i;
	u8 texType;
	
	//if ((u32)ptr & 3) return;
	
	while (1) {
		switch ((*ptr) >> 24) {
			/*case 0x04: // gsSPVertex
				vtxSize = (*ptr & 0xFFFF) / sizeof(Vtx);
				trollptr = (u32*)segmented_to_virtual((void*)*(ptr + 1));
				crawlVertices((u8*)trollptr, vtxSize);
				break;*/
			case 0x06: // gsSPDisplayList
				trollptr = (u32*)segmented_to_virtual((void*)*(ptr + 1));
				crawlDisplayList(trollptr);
				break;
			case 0xB8: // gsSPEndDisplayList
				return;
				
			case 0xF3: // gsDPLoadBlock (texture size)
				if (trollptr != NULL) {
					if (lsd_textures) {
						switch (texType) {
							case 2:
								invertRGBA16((u16*)trollptr, ((*(ptr + 1) >> 12) & 0xFFF) + 1, lsd_textures);
								break;
							case 3:
								invertRGBA32(trollptr, ((*(ptr + 1) >> 12) & 0xFFF) + 1, lsd_textures);
								break;
						}
					}
					if (flip_textures) {
						switch (texType) {
							case 2:
								rotateRGBA16((u16*)trollptr, ((*(ptr + 1) >> 12) & 0xFFF) + 1);
								break;
							case 3:
								rotateRGBA32(trollptr, ((*(ptr + 1) >> 12) & 0xFFF) + 1);
								break;
						}
					}
					if (normalize_textures) {
						switch (texType) {
							case 2:
								normalizeRGBA16((u16*)trollptr, ((*(ptr + 1) >> 12) & 0xFFF) + 1);
								break;
							case 3:
								normalizeRGBA32(trollptr, ((*(ptr + 1) >> 12) & 0xFFF) + 1);
								break;
						}
					}
					if (nightMode) {
						switch (texType) {
							case 2:
								darkenRGBA16((u16*)trollptr, ((*(ptr + 1) >> 12) & 0xFFF) + 1);
								break;
							case 3:
								darkenRGBA32(trollptr, ((*(ptr + 1) >> 12) & 0xFFF) + 1);
								break;
						}
					}
					
					for (i = 0; i < 256; i++) {
						if (foundTextures[i] == trollptr) {
							foundTextureSamples[i] = *trollptr;
							break;
						}
					}
					trollptr = NULL;
				}
				break;
			case 0xF8: // gsDPSetFogColor
				if (nightMode) {
					*(ptr + 1) = ((*(ptr + 1) >> 2) & 0x3F3F3F00) | (*(ptr + 1) & 0xFF);
					//*(ptr + 1) &= 0x000000FF;
				}
				break;
			case 0xFD: // gsDPSetTextureImage
				texType = (*ptr >> 19) & 0x1F;
				trollptr = (u32*)segmented_to_virtual((void*)*(ptr + 1));
				
				/*if ((u32)trollptr & 3) {
					trollptr = NULL;
					break;
				}*/
				
				for (i = 0; i < 256; i++) {
					if (foundTextures[i] == trollptr) {
						trollptr = NULL;
						break;
					}
				}
				
				if (trollptr != NULL) {
					for (i = 0; i < 256; i++) {
						if (foundTextures[i] == NULL) {
							foundTextures[i] = trollptr;
							foundTextureSamples[i] = *trollptr;
							break;
						}
					}
				}
				break;
		}
		
		ptr += 2;
	}
}

static void crawlGeoLayout(u32 *ptr) {
	u32 *dl;
	
	while (1) {
		switch ((*ptr) >> 24) {
			case 0x01: // GEO_END
				return;
			case 0x15: // GEO_DISPLAY_LIST
				dl = (u32*)segmented_to_virtual((void*)*(++ptr));
				crawlDisplayList(dl);
				break;
		}
		
		ptr++;
	}
}

static s32 crawlGeoLayout_NightMode(u32 *ptr) {
	u32 *dl;
	
	while (1) {
		switch ((*ptr) >> 24) {
			case 0x01: // GEO_END
				return;
			case 0x15: // GEO_DISPLAY_LIST
				ptr++;
				break;
			case 0x19: // GEO_BACKGROUND
				dl = (u32*)*(++ptr);
				nightMode = nightTime && dl != NULL;
				return;
		}
		
		ptr++;
	}
}



u32 *area_geo_layout_addresses[16];
void area_geo_load_intercept(u8 areaId, u32 *areaGeoLayout) {
	area_geo_layout_addresses[areaId] = areaGeoLayout;
}

extern u8 *gMovtexIdToTexture[8];
extern uintptr_t sSegmentTable[32];
void troll_geo_layout(u32 *areaGeoLayout) {
	register s32 i, j;
	register struct ObjectNode *listHead;
	register struct Object *obj;
	s32 starCount = (save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1) * (get_red_star_count(gCurrSaveFileNum - 1) - 3)) / 13;
	u16 oldSeed;
	u32 *lavamovtex;
	
	// generate the lsd texture type based on rng and level num
	oldSeed = gRandomSeed16;
	gRandomSeed16 = personalizationRandSeed ^ gCurrLevelNum;
	lsd_textures = 1 + (random_u16() & 1);
	gRandomSeed16 = oldSeed; // return the seed
	
	// troll the level
	for (i = 0; i < 256; i++) {
		if (foundTextures[i] != NULL) {
			if (foundTextureSamples[i] != *foundTextures[i]) { // texture overwritten
				foundTextures[i] = NULL;
			}
		}
	}
	
	nightModeSetting = 0;
	for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
		listHead = &gObjectLists[i];

		obj = (struct Object *) listHead->next;

		while (obj != (struct Object *) listHead) {
			if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED)) {
				if (obj->behavior == segmented_to_virtual(bhvTroll)) {
					// painting data, add the texture sample to protect it from being overwritten
					for (j = 0; j < 256; j++) {
						if (foundTextures[j] == NULL) {
							u32 *trollptr = (u32*)paintings[(u8)(obj->oBehParams >> 24)];
							foundTextures[j] = trollptr;
							foundTextureSamples[j] = *trollptr;
							break;
						}
					}
				}
				else if (obj->behavior == segmented_to_virtual(bhvLoadBlueGomba)) {
					// personalizator object, get night mode here
					nightModeSetting = (u8)(obj->oBehParams >> 8);
					personalizationFlags = (u8)(obj->oBehParams & 0xFF);
				}
			}
			obj = (struct Object *) obj->header.next;
		}
	}
	if (random_u16() >= (3313.0f * starCount / 666.0f)) {
		lsd_textures = 0;
	}
	flip_textures = random_u16() < (3313.0f * starCount / 666.0f);
	normalize_textures = random_u16() < (3313.0f * starCount / 666.0f);
	
	updateRTC();
	switch (nightModeSetting) {
		case 0: // RTC, otherwise day
		case 1: // RTC, otherwise night
		case 2: // flip RTC, otherwise day
		case 3: // flip RTC, otherwise night
			nightMode = FALSE;
			crawlGeoLayout_NightMode(segmented_to_virtual(areaGeoLayout));
			break;
		case 4: // always day
			nightMode = FALSE;
			break;
		case 5: // always night
			nightMode = TRUE;
			break;
	}
	
	if (PERSONALIZATION_FLAG_DISABLE_DISPLAY_LIST) return;
	crawlGeoLayout(segmented_to_virtual(areaGeoLayout));
	
	if ((u32*)sSegmentTable[2] != NULL) {
		// troll the lava mov texture
		lavamovtex = (u32*)segmented_to_virtual(gMovtexIdToTexture[4]);
		if (*lavamovtex == 0xB901C901) {
			if (nightMode) {
				hueRotateRGBA5551((u16*)lavamovtex, 32*32, -81);
			}
		}
		else {
			if (!nightMode) {
				//hueRotateRGBA5551((u16*)lavamovtex, 32*32, 81);
				//*lavamovtex == 0xB901C901;
			}
		}
	}
}





s32 hud_type_param = 0;
static void update_rtc_and_level_info() {
	register s32 i;
	register struct ObjectNode *listHead;
	register struct Object *obj;
	
	updateRTC();
	
	levelType = 0;
	
	for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
		listHead = &gObjectLists[i];

		obj = (struct Object *) listHead->next;

		while (obj != (struct Object *) listHead) {
			if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && (u32)obj->behavior >= 0x80000000) {
				if (obj->behavior == segmented_to_virtual(bhvLoadBlueGomba)) {
					// this is a special object that defines lava/cave type levels, and terrain type otherwise
					switch ((obj->oBehParams >> 16) & 0xFF) {
						case 0:
							levelType = 4; // cave
							break;
						case 1:
							levelType = 6; // lava
							break;
						default:
							gCurrentArea->terrainType = ((obj->oBehParams >> 16) & 0xFF) - 2;
							break;
					}
					hud_type_param = obj->oBehParams >> 24;
					break; // found the object
				}
			}
			obj = (struct Object *) obj->header.next;
		}
	}
	
	if (levelType == 0) {
		switch (gCurrentArea->terrainType) {
			case TERRAIN_STONE:
				if (!skyboxLoaded) {
					// castles, castles and more castles
					levelType = 0;
					break;
				}
			case TERRAIN_SLIDE:
				levelType = 7; // secret course
				break;
			case TERRAIN_GRASS:
				levelType = 1;
				break;
			case TERRAIN_SNOW:
				levelType = 2;
				break;
			case TERRAIN_SAND:
				levelType = 3;
				break;
			case TERRAIN_WATER:
				levelType = 5;
				break;
		}
	}
}





static f32 lateral_dist_between_object_and_pos(struct Object *obj1, f32 x, f32 z) {
    f32 dx = obj1->oPosX - x;
    f32 dz = obj1->oPosZ - z;

    return sqrtf(dx * dx + dz * dz);
}

static struct Object *_find_nearest_object_with_behavior(f32 x, f32 z, const BehaviorScript *behavior, f32 *dist) {
    uintptr_t *behaviorAddr = segmented_to_virtual(behavior);
    struct Object *closestObj = NULL;
    struct Object *obj;
	// iterate through the list respective to the behavior, much faster than checking all objects
	struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(behavior)];
    f32 minDist = 0x20000;
	
	// Enumerate the objects to find out what trollage we can do
	if ((u32)listHead >= 0x80000000) {
		obj = (struct Object *) listHead->next;
		
		while (obj != (struct Object *) listHead) {
			if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && obj->behavior == behaviorAddr) {
				f32 objDist = lateral_dist_between_object_and_pos(obj, x, z);
				if (objDist < minDist) {
					closestObj = obj;
					minDist = objDist;
				}
			}
			
			obj = (struct Object *) obj->header.next;
		}
	}

	*dist = minDist;
	return closestObj;
}


void clear_objects();
void clear_areas();

f32 absf(f32 h);
f32 roundf(f32 h);

void level_init_intercept() {
	register s32 diff;
	
	// do everything here because otherwise no space
    clear_objects();
    clear_areas();
    main_pool_push_state();
	
	// reset state
	skyboxLoaded = FALSE;
	nightMode = FALSE; // becomes TRUE if a skybox loads
	
	// set level scale
	levelScaleH = roundf(TRACKER_level_scale_modifier_h * 10.0f);
	levelScaleV = roundf(TRACKER_level_scale_modifier_v * 10.0f);
	if (levelScaleH > 14.0f) levelScaleH = 14.0f;
	if (levelScaleH < 7.0f) levelScaleH = 7.0f;
	if (levelScaleV > 14.0f) levelScaleV = 14.0f;
	if (levelScaleV < 7.0f) levelScaleV = 7.0f;
	// slightly "fix" non-uniform scales
	diff = (s32)(roundf((absf(levelScaleH - levelScaleV) - 1.0f) / 2.0f) * 2.0f) / 2;
	if (diff > 0) {
		if (levelScaleH < levelScaleV) {
			levelScaleH += diff - (diff / 2);
			levelScaleV -= diff / 2;
		}
		else {
			levelScaleH -= diff - (diff / 2);
			levelScaleV += diff / 2;
		}
	}
	// scale down after rounding to .1
	levelScaleH /= 10.0f;
	levelScaleV /= 10.0f;
}


void trollWarps() {
	register s32 i;
	register struct ObjectNode *listHead;
	register struct Object *obj;
	register struct Object *otherDoor;
	
	u16 oldSeed = gRandomSeed16;
	gRandomSeed16 = personalizationRandSeed;
	
	for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
		listHead = &gObjectLists[i];

		obj = (struct Object *) listHead->next;

		while (obj != (struct Object *) listHead) {
			u16 rng = random_u16();
			if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && rng < 1986) { // 1/33 chance
				// swap 2 random doors' warps
				if (obj->behavior == segmented_to_virtual(bhvDoorWarp)) {
					// find door 2
					otherDoor = obj;
					otherDoor--;
					if (otherDoor->behavior != obj->behavior
						// TODO: somehow figure out whether we should flip the warp destination or not if the doors aren't next to each other?
						// if it's not a double door, otherDoor could be anything, including a single door rotated either way
						// maybe we could just assign a floor type for behind the door (quicksand? :trol:) that would silently flip the warp
						// or door's bparam3
						// either way, pain
						|| obj->oPosY != otherDoor->oPosY || absf(absf(obj->oMoveAngleYaw - otherDoor->oMoveAngleYaw) - 0x8000) > 0x1000) {
						otherDoor = NULL;
					};

					if (otherDoor != NULL) {
						// :trol:
						u32 swap = obj->oBehParams & 0x00FF00FF;
						obj->oBehParams = (obj->oBehParams & 0xFF00FF00) | (otherDoor->oBehParams & 0x00FF00FF);
						otherDoor->oBehParams = (otherDoor->oBehParams & 0xFF00FF00) | swap;

						obj->oBehParams2ndByte = ((obj->oBehParams) >> 16) & 0xFF;
						otherDoor->oBehParams2ndByte = ((otherDoor->oBehParams) >> 16) & 0xFF;

						// double door, set bparam3 so the warp destination is flipped (otherwise mario ends up in gbj)
						// doesn't ensure that mario doesn't end up behind a door, but it covers the main use case (we may put this condition above if it happens too often)
						//if (obj->oPosY == otherDoor->oPosY) {
							obj->oBehParams ^= 0x0100;
							otherDoor->oBehParams ^= 0x0100;
						//}
						continue;
					}
				}

				// swap normal and cursed warp
				if ((obj->oBehParams & 0x00FF) && (obj->oInteractType == INTERACT_WARP || obj->behavior == segmented_to_virtual(bhvDoorWarp) || obj->behavior == segmented_to_virtual(bhvTroll))) {
					// :trol:
					obj->oBehParams = (obj->oBehParams & 0xFF00FF00) | ((obj->oBehParams & 0x00FF0000) >> 16) | ((obj->oBehParams & 0x000000FF) << 16);
					obj->oBehParams2ndByte = ((obj->oBehParams) >> 16) & 0xFF;
				}
			}
			obj = (struct Object *) obj->header.next;
		}
	}
	
	gRandomSeed16 = oldSeed; // return the seed
}

void addMoreObjects() {
	register s32 i;
	register struct ObjectNode *listHead;
	register struct Object *obj;

	register s32 redStarCount = get_red_star_count(gCurrSaveFileNum - 1);

	if (redStarCount > 0) {
		// check for personalization flag 0x02 (PERSONALIZATION_FLAG_DISABLE_OBJECTS)
		for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
			listHead = &gObjectLists[i];

			obj = (struct Object *) listHead->next;

			while (obj != (struct Object *) listHead) {
				if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && obj->behavior == segmented_to_virtual(bhvLoadBlueGomba)) {
					if (obj->oBehParams & 0x02)
					{
						// yeah we do not add objects here
						return;
					}
				}
				obj = (struct Object *) obj->header.next;
			}
		}

		if (redStarCount >= 5) {
			register s32 totali;
			register f32 prevLevelScaleH, prevLevelScaleV;
			register s32 object_count_goal = (s32)((TRACKER_difficulty_modifier_sqrt_half - 1.0f) * 13.3f);
			f32 distGoomba, distBobomb;
			struct SpawnInfo spawnInfo;
			struct Surface *floor = NULL;
			register u32 *spawnBhv;
			u8 spawnModel;
			u8 water;

			u16 oldSeed = gRandomSeed16;
			gRandomSeed16 = personalizationRandSeed;

			update_rtc_and_level_info();

			prevLevelScaleH = levelScaleH;
			prevLevelScaleV = levelScaleV;
			levelScaleH = 1.0f;
			levelScaleV = 1.0f;

			if (object_count_goal > 10) {
				object_count_goal = (s32)(10 + sqrtf(object_count_goal - 10));
			}

			if (gMarioObject != NULL) {
				if (TRACKER_difficulty_modifier > 1.0f) {
					// spawn some more enemies in :trol:

					// i will be increased only if an object was successfully spawned
					// totali is used to limit the amount of iterations to avoid a long loading screen / potentially infinite loop
					for (i = 0, totali = 0; i < object_count_goal && totali < object_count_goal * object_count_goal; totali++) {
						spawnInfo.startPos[0] = (random_u16() - 32768) / 32768.0f * 8192.0f;
						spawnInfo.startPos[2] = (random_u16() - 32768) / 32768.0f * 8192.0f;
						spawnInfo.startPos[1] = find_floor(spawnInfo.startPos[0], 3313.0f, spawnInfo.startPos[2], &floor);
						// use as a temp variable
						distGoomba = find_water_level(spawnInfo.startPos[0], spawnInfo.startPos[2]);
						water = FALSE;

						if (distGoomba > spawnInfo.startPos[1] + 100.0f) {
							water = TRUE;
							spawnInfo.startPos[1] = distGoomba;
						}

						spawnBhv = NULL;

						if (floor != NULL && spawnInfo.startPos[1] > FLOOR_LOWER_LIMIT) {
							if (water || floor->type == SURFACE_BURNING) {
								// prefer to spawn objects on land
								if (random_u16() < 3313) {
									if (levelType == 6 || !lava_skeeter) {
										spawnBhv = segmented_to_virtual(bhvSkeeter);
										spawnModel = MODEL_SKEETER;
									}
								}
							}
							else {
								// decide to spawn goombas or bobombs
								_find_nearest_object_with_behavior(spawnInfo.startPos[0], spawnInfo.startPos[2], segmented_to_virtual(bhvGoomba), &distGoomba);
								// use distbobomb temporarily
								_find_nearest_object_with_behavior(spawnInfo.startPos[0], spawnInfo.startPos[2], segmented_to_virtual(bhvGoombaTripletSpawner), &distBobomb);
								if (distGoomba > distBobomb)
									distGoomba = distBobomb;

								// use distbobomb correctly now
								_find_nearest_object_with_behavior(spawnInfo.startPos[0], spawnInfo.startPos[2], segmented_to_virtual(bhvBobomb), &distBobomb);

								if (distGoomba < distBobomb || (distGoomba == distBobomb && (random_u16() & 1))) {
									spawnBhv = segmented_to_virtual(bhvGoomba);
									spawnModel = MODEL_GOOMBA;
								}
								else {
									spawnBhv = segmented_to_virtual(bhvBobomb);
									spawnModel = MODEL_BLACK_BOBOMB;
								}
							}

							// try not to spawn on ledges challenge
							if (floor->normal.y < 0.9f) {
								spawnBhv = NULL;
							}
						}

						if (spawnBhv != NULL && get_model_loaded(spawnModel)) {
							// spawn skeet
							obj = create_object(spawnBhv);

							// Behavior parameters are often treated as four separate bytes, but
							// are stored as an s32.
							obj->oBehParams = 0x00000000;
							// The second byte of the behavior parameters is copied over to a special field
							// as it is the most frequently used by objects.
							obj->oBehParams2ndByte = ((obj->oBehParams) >> 16) & 0xFF;

							obj->behavior = spawnBhv;
							obj->unused1 = 0;

							// Record death/collection in the SpawnInfo
							obj->respawnInfoType = RESPAWN_INFO_TYPE_32;
							obj->respawnInfo = &obj->oBehParams;

							spawnInfo.startAngle[0] = 0.0f;
							spawnInfo.startAngle[1] = 0.0f;
							spawnInfo.startAngle[2] = 0.0f;

							spawnInfo.areaIndex = gCurrentArea->index;
							spawnInfo.activeAreaIndex = gCurrentArea->index;
							spawnInfo.model = gLoadedGraphNodes[spawnModel];

							geo_obj_init_spawninfo(&obj->header.gfx, &spawnInfo);

							obj->oPosX = spawnInfo.startPos[0];
							obj->oPosY = spawnInfo.startPos[1];
							obj->oPosZ = spawnInfo.startPos[2];

							i++;
						}
					}
				}
				else if (TRACKER_difficulty_modifier < 1.0f) {
					// delete some enemies
					// use the now useless object_count_goal for the RNG ceiling
					object_count_goal = 65536 * (sqrtf(TRACKER_difficulty_modifier) / 2.0f + 0.5f);

					for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
						listHead = &gObjectLists[i];

						obj = (struct Object *) listHead->next;

						while (obj != (struct Object *) listHead) {
							if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && (u32)obj->behavior >= 0x80000000) {
								// use totali, also unused
								totali = get_object_list_from_behavior(obj->behavior);
								if (obj->oInteractType == INTERACT_DAMAGE || totali == OBJ_LIST_DESTRUCTIVE || totali == OBJ_LIST_PUSHABLE) {
									// despawn time
									if (random_u16() > object_count_goal) {
										obj->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // unload
									}
								}

								if (nightMode && (obj->behavior == segmented_to_virtual(bhvBobombBuddy) || obj->behavior == segmented_to_virtual(bhvToadMessage))) {
									// despawn random NPCs at night
									if (random_u16() > 33130) {
										obj->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // unload
									}
								}
							}
							obj = (struct Object *) obj->header.next;
						}
					}
				}
			}

			// restore old state
			levelScaleH = prevLevelScaleH;
			levelScaleV = prevLevelScaleV;
			gRandomSeed16 = oldSeed; // return the seed
		}
	}
	else {
		// All warp holes in castle turn to bowser, otherwise despawn
		for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
			listHead = &gObjectLists[i];

			obj = (struct Object *) listHead->next;

			while (obj != (struct Object *) listHead) {
				if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED)) {
					// Warp Holes
					if (obj->behavior == segmented_to_virtual((void*)0x13000780)) {
						if (gCurrLevelNum == 0x10) { // Castle Grounds
							struct ObjectWarpNode *warpNode = area_get_warp_node(obj->oBehParams2ndByte);

							// bye rng warp
							obj->oBehParams &= 0xFFFFFF00;

							// set the only warp left to go to bowser
							if (warpNode != NULL) {
								warpNode->node.destLevel = 0x1E;
								warpNode->node.destArea = 1;
								warpNode->node.destNode = 10;
							}
						}
						else {
							obj->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // unload
						}
					}
					if (/*Warp doors*/ obj->behavior == segmented_to_virtual((void*)0x13000afc) ||
						/*Paintings*/  obj->behavior == segmented_to_virtual(bhvTroll)
					) {
						// bye rng warp
						obj->oBehParams &= 0xFFFFFF00;
					}
				}
				obj = (struct Object *) obj->header.next;
			}
		}
	}
}

s32 is_star(struct Object *obj) {
	return obj->behavior == segmented_to_virtual(bhvStar) ||
		obj->behavior == segmented_to_virtual(bhvHiddenRedCoinStar) ||
		obj->behavior == segmented_to_virtual(bhvHiddenStar) || // secret
		obj->behavior == segmented_to_virtual(bhvHiddenSilverStarStar);
}

extern const BehaviorScript bhvMusicModifier[];
void postObjectLoadPass() {
	register s32 i;
	register struct ObjectNode *listHead;
	register struct Object *obj;
	s32 goombaCount;
	f32 minY = 32767.0f, maxY = -32768.0f, avgY;
	u8 wingCapBox = FALSE, metalCapBox = FALSE, vanishCapBox = FALSE;
	void *vaddr;
	
	personalization_beeparams = 0;
	mus_tempooverride = 0;
	mus_pitchmul = 1.0f;
	mus_transposition = 0;
	mus_nlstday = 0;
	mus_nlstnight = 0;

	powermeter_last_romstart = NULL;
	
	update_rtc_and_level_info();
	troll_geo_layout(area_geo_layout_addresses[gCurrentArea->index]);
	
	// Enumerate the objects to find out what trollage we can do
	goombaCount = 0;
	for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
		listHead = &gObjectLists[i];

		obj = (struct Object *) listHead->next;

		while (obj != (struct Object *) listHead) {
			if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && (u32)obj->behavior >= 0x80000000) {
				if (obj->behavior == segmented_to_virtual(bhvGoomba)) {
					goombaCount += 1;
				}
				else if (obj->behavior == segmented_to_virtual(bhvGoombaTripletSpawner)) {
					goombaCount += 3;
				}
				else if (obj->behavior == segmented_to_virtual(bhvGombaTower)) {
					s16 __count = obj->oBehParams >> 24;
					if (__count == 0) {
						__count = 5;
					}
					goombaCount += __count;
				}
				else if (obj->behavior == segmented_to_virtual(bhvStar)) {
					AI_star_set_bparams(obj);
				}
				else if (obj->behavior == segmented_to_virtual(bhvTroll)) {
					// we have to uniformly scale here because paintings are a bit limited
					levelScaleV = levelScaleH;
				}
				else if (obj->behavior == segmented_to_virtual(bhvExclamationBox)) {
					switch (obj->oBehParams2ndByte) {
						case 0:
							wingCapBox = TRUE;
							break;
						case 1:
							metalCapBox = TRUE;
							break;
						case 2:
							vanishCapBox = TRUE;
							break;
					}
				}
				else if (obj->behavior == segmented_to_virtual(bhvLoadBlueGomba)) {
					// personalizator object
					personalization_beeparams = obj->oBehParams;

					// If we're instant warping, changing the level scale suddenly could screw things up.
					if (!isInstantWarping && PERSONALIZATION_FLAG_DISABLE_LEVEL_SCALE) {
						levelScaleH = 1.0f;
						levelScaleV = 1.0f;
					}
				}
				else if (obj->behavior == segmented_to_virtual(bhvMusicModifier)) {
					mus_transposition = (((obj->oBehParams >> 28) & 0xF) - 7) / 8.0f;
					mus_pitchmul = (((obj->oBehParams >> 24) & 0xF) + 1) / 8.0f;
					mus_tempooverride = (obj->oBehParams >> 16) & 0xFF;
					mus_nlstday = (obj->oBehParams >> 8) & 0xFF;
					mus_nlstnight = obj->oBehParams & 0xFF;
				}

				if (obj->oPosY < minY) {
					minY = obj->oPosY;
				}
				if (obj->oPosY > maxY) {
					maxY = obj->oPosY;
				}
			}
			obj = (struct Object *) obj->header.next;
		}
	}
	
	// luigi model troll from level_cmd_init_mario moved here to fix bug
	gMarioSpawnInfo->model = gLoadedGraphNodes[ *((u8*)0x807FFFED) ? 2 : 1 ];
	if (minY < gMarioSpawnInfo->startPos[1]) {
		minY = gMarioSpawnInfo->startPos[1];
	}
	avgY = (minY + maxY) / 2.0f;
	
	
	if (!PERSONALIZATION_FLAG_DISABLE_RETEXTURE && get_red_star_count(gCurrSaveFileNum - 1) >= 6) {
		// Goomba Personalization
		// check if gomba are loaded, and set gomba type, trolling the texture accordingly

		// TODO: Tweak the pixel check and the hues on B3313
		vaddr = (void*)segmented_to_virtual((void*)0x08019D30);
		if (*((u16*)vaddr) == 0xA201) {
			goombaType = 0;
			
			if (levelType == 6) {
				// fire type level
				goombaType = (TRACKER_difficulty_modifier < 1.0f ? 0 : 6);
			}
			else if (levelType == 4) {
				// cave type level
				goombaType = (TRACKER_difficulty_modifier > 2.0f ? 3 : 1);
			}
			else {
				// other levels
				if (TRACKER_difficulty_modifier < 0.5f) {
					// yellow goombas (easier)
					goombaType = 4;
				}
				else if (TRACKER_accum_murder * 0.5f > 1.0f + sqrtf(goombaCount) * TRACKER_difficulty_modifier) {
					// the player doing a little too much killing
					goombaType = 5; // FUCKFUCK RUN
				}
				else if (TRACKER_difficulty_modifier > 1.5f) {
					// red goombas (harder)
					goombaType = 2;
				}
			}
			
			switch (goombaType) {
				case 1: // blue
					hueRotateRGBA5551((u16*)vaddr, 32*32 * 2, 128);
					break;
				case 2: // red
					hueRotateRGBA5551((u16*)vaddr, 32*32 * 2, -17);
					break;
				case 3: // teal
					hueRotateRGBA5551((u16*)vaddr, 32*32 * 2, 106);
					break;
				case 4: // yellow
					hueRotateRGBA5551((u16*)vaddr, 32*32 * 2, 20);
					break;
				case 5: // purple
					hueRotateRGBA5551((u16*)vaddr, 32*32 * 2, -80);
					break;
				case 6: // orange
					hueRotateRGBA5551((u16*)vaddr, 32*32 * 2, 12);
					break;
			}
		}
		
		// Bobomb Personalization
		// check if bom are loaded, and set bom type, trolling the texture accordingly

		// TODO: Tweak the pixel check and the hues on B3313
		vaddr = (void*)segmented_to_virtual((void*)0x0801DA60);
		if (*((u16*)vaddr + 331) == 0x0001) {
			bobombType = 0;
			
			if (levelType == 6) {
				// fire type level
				bobombType = 2;
			}
			else {
				// other levels
				if (TRACKER_difficulty_modifier > 2.0f) {
					// red bobombs (harder)
					bobombType = 1;
				}
			}
			
			switch (bobombType) {
				case 1: // red (insta explode)
					texCopyRGBA16((u16*)vaddr + 64*32 * 2, (u16*)vaddr, 64*32 * 2);
					hueRotateRGBA5551((u16*)vaddr, 64*32 * 2, -36);
					rgbMultiplyRGBA16((u16*)vaddr, 64*32 * 2, 0.7f, 0.7f, 0.7f);
					break;
				case 2: // orange
					texCopyRGBA16((u16*)vaddr + 64*32 * 2, (u16*)vaddr, 64*32 * 2);
					hueRotateRGBA5551((u16*)vaddr, 64*32 * 2, -12);
					rgbMultiplyRGBA16((u16*)vaddr, 64*32 * 2, 0.7f, 0.7f, 0.7f);
					break;
			}
		}
		
		// Amp Personalization
		// check if amp are loaded, and set amp type, trolling the lights (color) accordingly

		// TODO: Tweak the pixel check and the hues on B3313
		vaddr = (void*)segmented_to_virtual((void*)0x08002320);
		if (*((u32*)vaddr) == 0x333F0000) {
			ampType = 0;
			
			if (TRACKER_difficulty_modifier > 2.0f) {
				// blue amps (harder)
				ampType = 1;
			}
			
			switch (ampType) {
				case 1: // blue
					*((u32*)vaddr) = 0x00333F00;
					*((u32*)vaddr + 1) = 0x00333F00;
					*((u32*)vaddr + 2) = 0x00CFFF00;
					*((u32*)vaddr + 3) = 0x00CFFF00;
					break;
			}
		}
	}

	// load motos behavior data
	// TODO: move to memory setup, replacing the current DMA, but too lazy rn lol!
	dma_read(0x80410000, &moto_bhv_start, &moto_bhv_start+0x5000/4);
	// load motos model data
	if (levelType == 2) {
		dma_read(0x80415000, &moto_model_ice_start, &moto_model_ice_start+0x7B00/4);
	}
	else {
		dma_read(0x80415000, &moto_model_normal_start, &moto_model_normal_start+0x7B00/4);
	}
	
	// load star texture
	if (PERSONALIZATION_FLAG_LOAD_RED_STAR) {
		// load red star
		dma_read(0x80408800, &tex_stars_start + 32*32*8*2 / 4, &tex_stars_start + 2 * 32*32*8*2 / 4 - 1);
		// discolor the star door if it's loaded
		vaddr = segmented_to_virtual((void*)0x090000C0);
		if (*((u32*)vaddr) == 0xBA83BA41) {
			hueRotateRGBA5551((u16*)vaddr, 32*64 * 2 + 32*31, -44);
		}
	}
	else {
		// load yellow star
		dma_read(0x80408800, &tex_stars_start, &tex_stars_start + 32*32*8*2/4 - 1);
	}
	if (PERSONALIZATION_FLAG_HUE_SHIFT_STAR) {
		// hue shift star (red->green)
		hueRotateRGBA5551((u16*)0x80408800, 32*32 * 8, 85);
	}
	
	// troll the objects now
	
	// don't do this due to overflows, troll init_mario instead
	//gMarioSpawnInfo->startPos[0] *= levelScaleH;
	//gMarioSpawnInfo->startPos[1] *= levelScaleV;
	//gMarioSpawnInfo->startPos[2] *= levelScaleH;
	
	if (gCurrentArea != NULL && gCurrentArea->camera != NULL) {
		gCurrentArea->camera->areaCenX *= levelScaleH;
		gCurrentArea->camera->areaCenY *= levelScaleV;
		gCurrentArea->camera->areaCenZ *= levelScaleH;
	}
	
	for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
		listHead = &gObjectLists[i];

		obj = (struct Object *) listHead->next;

		while (obj != (struct Object *) listHead) {
			if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && (u32)obj->behavior >= 0x80000000 && (u32)obj->behavior != 0x80401700) {
				// we can't switch case this because C
				/*if (obj->behavior == segmented_to_virtual(bhvGoomba)) {
					if (goombaType == 3) {
						obj->oHealth = 2;
					}
				}
				else*/ if (obj->behavior == segmented_to_virtual(bhvStaticObject)) {
					// scale all objects that have the "bhvStaticObject" behavior (used for what you would think to be level geometry)
					obj_scale_personalized(obj);
				}
				else if (obj->behavior == segmented_to_virtual(bhvStar)) {
					// Performance enhancement: assign the bparam here so it doesn't find the average Y for every single star in the level
					if (obj->oPosY > avgY) {
						// platform
						obj->oBehParams |= 0x08;
					}
				}
				else if (TRACKER_star_preferences_cap[2] > 1.0f && vanishCapBox) {
					if (!PERSONALIZATION_FLAG_DISABLE_OBJECTS && get_red_star_count(gCurrSaveFileNum - 1) >= (5 + ((personalizationRandSeed & 16) ? 1 : 0))) {
						// replace blue coins with clear coins
						if (obj->behavior == segmented_to_virtual(bhvBlueCoinSwitch)) {
							obj->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // unload
						}
						else if (obj->behavior == segmented_to_virtual(bhvHiddenBlueCoin)) {
							// convert to a clear coin
							obj->oBehParams = 0x05000000;
							obj->header.gfx.sharedChild = gLoadedGraphNodes[MODEL_YELLOW_COIN];
							obj->behavior = segmented_to_virtual(bhvYellowCoin);
							obj->curBhvCommand = obj->behavior;
						}
					}
				}

				// position it
				obj->oPosX *= levelScaleH;
				obj->oPosY *= levelScaleV;
				obj->oPosZ *= levelScaleH;
			}
			obj = (struct Object *) obj->header.next;
		}
	}
	
	if (!PERSONALIZATION_FLAG_DISABLE_OBJECTS) {
		trollWarps(); // trol
	}
	set_hud_type(hud_type_param);
}

void load_stuff_in_level_pool() {
}
