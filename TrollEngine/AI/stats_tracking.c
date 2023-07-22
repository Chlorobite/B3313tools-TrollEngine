#include <PR/ultratypes.h>
#include "sm64.h"
#include "stats_tracking.h"
#include "personalization_helpers.h"

#include "game/object_list_processor.h"
#include "game/object_helpers.h"
#include "game/game_init.h"
#include "game/interaction.h"
#include "game/level_update.h"
#include "game/save_file.h"
#include "engine/behavior_script.h"
#include "behavior_data.h"
#include "actors/common1.h"

extern struct SaveBuffer gSaveBuffer;


f32 __min(f32 a, f32 b) {
	if (a < b)
		return a;
	return b;
}

f32 __max(f32 a, f32 b) {
	if (a > b)
		return a;
	return b;
}

f32 moveTo(f32 a, f32 b, f32 c) {
	if (a == b) return a;
	
	if (a > b) {
		return __max(a - c, b);
	}
	
	return __min(a + c, b);
}

#define DECAY(a, b) a = __max(a - b, 0.0)
#define LERP(a, b, c) a = a + c * (b - a);

// definition of variables, information and constants can be found in stats_tracking.h
f32 TRACKER_speed = 24.0f;
f32 TRACKER_air_time = 0.25f;
f32 TRACKER_water = 0.1f;

f32 TRACKER_hp = 4.0f;
f32 TRACKER_boss_performance = 0.0f;
f32 TRACKER_accum_deaths = 0.0f;

f32 TRACKER_accum_deaths_enemy_knockback = 0.0f;
f32 TRACKER_accum_deaths_skill_issue = 0.0f; // fall damage, drowning, quicksand, basically things that literally can't kill you unless you're bad lol
f32 TRACKER_accum_deaths_fire_lava = 0.0f;
f32 TRACKER_accum_deaths_death_barrier = 0.0f;
f32 TRACKER_accum_deaths_trolled = 0.0f; // traps as a result of personalization
// these are the values that should actually be used; they're normalized to an average of 1.0
f32 TRACKER_death_reasons[5];

f32 TRACKER_accum_social = 1.0f;
f32 TRACKER_accum_nerd = 1.0f;
f32 TRACKER_accum_murder = 0.0f;//22.5f;


f32 TRACKER_accum_stars = 0.0f;//12.0f;

f32 TRACKER_accum_stars_prefer_boss = 0.0f;
f32 TRACKER_accum_stars_prefer_cannon = 0.0f;
f32 TRACKER_accum_stars_prefer_collect = 0.0f; // reds/smol stars, secrets
f32 TRACKER_accum_stars_prefer_free = 0.0f; // e.g. toad stars
f32 TRACKER_accum_stars_prefer_murder = 0.0f; // the funny stars in b3313 where you commit mass genocide
f32 TRACKER_accum_stars_prefer_platform = 0.0f;
f32 TRACKER_accum_stars_prefer_slide = 0.0f;
f32 TRACKER_accum_stars_prefer_timed = 0.0f;
f32 TRACKER_star_preferences_gameplay[8];

f32 TRACKER_accum_stars_prefer_buildings = 0.0f;
f32 TRACKER_accum_stars_prefer_overworld_courses = 0.0f;
f32 TRACKER_accum_stars_prefer_snow_courses = 0.0f;
f32 TRACKER_accum_stars_prefer_desert_courses = 0.0f;
f32 TRACKER_accum_stars_prefer_cave_courses = 0.0f;
f32 TRACKER_accum_stars_prefer_water_courses = 0.0f;
f32 TRACKER_accum_stars_prefer_fire_courses = 0.0f;
f32 TRACKER_accum_stars_prefer_secret_courses = 0.0f;
f32 TRACKER_star_preferences_level_type[8];

f32 TRACKER_accum_stars_prefer_wing_cap = 0.0f;
f32 TRACKER_accum_stars_prefer_metal_cap = 0.0f;
f32 TRACKER_accum_stars_prefer_vanish_cap = 0.0f;
f32 TRACKER_star_preferences_cap[3];

f32 TRACKER_prefer_collect = 0.0f;
f32 TRACKER_prefer_exploration = 0.0f;
f32 TRACKER_prefer_lore = 0.0f;
f32 TRACKER_prefer_murder = 0.0f;
f32 TRACKER_prefer_parkour = 0.0f;
f32 TRACKER_prefer_swimming = 0.0f;
f32 TRACKER_difficulty_modifier = 1.0f;
f32 TRACKER_level_scale_modifier_h = 1.0f;
f32 TRACKER_level_scale_modifier_v = 1.0f;
f32 TRACKER_difficulty_modifier_half = 1.0f;
f32 TRACKER_difficulty_modifier_sqrt_half = 1.0f;

u16 TRACKER_flags = 0;


u8 hasSocObject = FALSE;
u8 hasNerdObject = FALSE;


void decay_stats() {
	// All constants can be found in stats_tracking.h
	DECAY(TRACKER_accum_deaths, TRACKER_accum_deaths_DECAY);
	if (hasSocObject) {
		DECAY(TRACKER_accum_social, TRACKER_accum_social_DECAY);
	}
	if (hasNerdObject) {
		DECAY(TRACKER_accum_nerd, TRACKER_accum_nerd_DECAY);
	}
	DECAY(TRACKER_accum_murder, TRACKER_accum_murder_DECAY);
	DECAY(TRACKER_accum_stars, TRACKER_accum_stars_DECAY);
	
	DECAY(TRACKER_accum_deaths_enemy_knockback, TRACKER_accum_death_reasons_DECAY);
	DECAY(TRACKER_accum_deaths_skill_issue, TRACKER_accum_death_reasons_DECAY);
	DECAY(TRACKER_accum_deaths_fire_lava, TRACKER_accum_death_reasons_DECAY);
	DECAY(TRACKER_accum_deaths_death_barrier, TRACKER_accum_death_reasons_DECAY);
	DECAY(TRACKER_accum_deaths_trolled, TRACKER_accum_death_reasons_DECAY);
	
	DECAY(TRACKER_accum_stars_prefer_boss, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_cannon, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_collect, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_free, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_murder, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_platform, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_slide, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_timed, TRACKER_accum_stars_prefer_DECAY);
	
	DECAY(TRACKER_accum_stars_prefer_buildings, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_overworld_courses, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_snow_courses, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_desert_courses, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_cave_courses, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_water_courses, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_fire_courses, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_secret_courses, TRACKER_accum_stars_prefer_DECAY);
	
	DECAY(TRACKER_accum_stars_prefer_wing_cap, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_metal_cap, TRACKER_accum_stars_prefer_DECAY);
	DECAY(TRACKER_accum_stars_prefer_vanish_cap, TRACKER_accum_stars_prefer_DECAY);
}

void avg_stats(f32 *from, f32 *to, int length) {
	f32 sum = 0.0f;
	f32 lerpValue;
	int i;
	
	// get sum, and reset the target array to One
	for (i = 0; i < length; i++) {
		sum += from[i];
		to[i] = 1.0f;
	}
	
	if (sum == 0.0f) {
		// special case, would be a division by zero so just leave the ones
		return;
	}
	
	// <5 stars/deaths is not concrete data, so reduce the effect of the new array
	lerpValue = sum / 5.0f;
	if (lerpValue > 1.0f) lerpValue = 1.0f;
	
	for (i = 0; i < length; i++) {
		LERP(to[i], from[i] / sum * length, lerpValue);
	}
}


void object_scan() {
	s32 i;
	struct ObjectNode *listHead;
	struct Object *obj;
	
	hasSocObject = FALSE;
	hasNerdObject = FALSE;
	
	// Enumerate the objects to find out what trollage we can do
	loadedObjectCount = 0;
	for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
		listHead = &gObjectLists[i];
		
		obj = (struct Object *) listHead->next;
		
		while (obj != (struct Object *) listHead) {
			loadedObjectCount++;

			// we can't switch case this because C
			if (obj->oInteractionSubtype & INT_SUBTYPE_NPC) {
				if ((u32)obj->header.gfx.sharedChild == (u32)gLoadedGraphNodes[MODEL_WOODEN_SIGNPOST]) {
					hasNerdObject = TRUE;
				}
				else {
					hasSocObject = TRUE;
				}
			}
			else if (obj->oInteractionSubtype & INT_SUBTYPE_SIGN) {
				hasNerdObject = TRUE;
			}

			if (obj->behavior == segmented_to_virtual(bhvDoor) && obj->oBehParams & 0x0600) {
				gHudDisplay.flags |= HUD_DISPLAY_FLAG_KEYS;
			}
			
			if (nightMode) {
				// disable water bomb cannons
				if (obj->behavior == segmented_to_virtual(bhvWaterBombCannon)) {
					obj->oWaterCannonUnkF4 = 0x33;
				}
				if (obj->behavior == segmented_to_virtual(bhvWaterBombSpawner)) {
					obj->oWaterBombSpawnerTimeToSpawn = 0x13;
				}
				// disable bowling ball spawners
				if ((u32)obj->behavior >= (u32)segmented_to_virtual(bhvTtmBowlingBallSpawner) && (u32)obj->behavior <= (u32)segmented_to_virtual(bhvThiBowlingBallSpawner)) {
					obj->oTimer = obj->oBBallSpawnerPeriodMinus1 >> 1;
				}
				// change red fire to blue
				if ((u32)obj->header.gfx.sharedChild == (u32)gLoadedGraphNodes[MODEL_RED_FLAME]) {
					obj->header.gfx.sharedChild = gLoadedGraphNodes[MODEL_BLUE_FLAME];
				}
			}
			
			obj = (struct Object *) obj->header.next;
		}
	}
}


u32 afkTimer = 0;
f32 lastStickMag = 0.0f;

void TRACKER_record_mario_state(struct MarioState *m) {
	f32 air_time_target = 0.0f;
	f32 water_target = 0.0f;
	f32 min_star_prefs_gameplay = 1.0f;
	f32 min_star_prefs_level_type = 1.0f;
	int i;
	
	// display the stats lol
	//stats_tracking_debug_display();

	gMarioState->numKeys = save_file_get_total_betakey_count(gCurrSaveFileNum - 1);
	gHudDisplay.flags &= ~HUD_DISPLAY_FLAG_KEYS;
	object_scan();
	
	if (get_red_star_count(gCurrSaveFileNum - 1) >= 1) {
		register s32 unlockDynamicDifficulty = get_red_star_count(gCurrSaveFileNum - 1) >= 4;
		register s32 unlockLevelScale = get_red_star_count(gCurrSaveFileNum - 1) >= 5;
		
		afkTimer++;
		if (gPlayer1Controller->buttonPressed || gPlayer1Controller->stickMag > 32.0f || absf(lastStickMag - gPlayer1Controller->stickMag) > 1.0f) {
			afkTimer = 0;
		}
		lastStickMag = gPlayer1Controller->stickMag;
		
		// afk for over 5 seconds
		if (afkTimer > 150) {
			// can't collect information about an afk player
			return;
		}
		
		// update the stats
		decay_stats();
		avg_stats(&TRACKER_accum_deaths_enemy_knockback, TRACKER_death_reasons, 5);
		avg_stats(&TRACKER_accum_stars_prefer_boss, TRACKER_star_preferences_gameplay, 8);
		avg_stats(&TRACKER_accum_stars_prefer_buildings, TRACKER_star_preferences_level_type, 8);
		avg_stats(&TRACKER_accum_stars_prefer_wing_cap, TRACKER_star_preferences_cap, 3);
		
		// get targets for air_time and water
		switch (m->action & ACT_GROUP_MASK) {
			case ACT_GROUP_AIRBORNE:
				air_time_target = 1.0f;
				if (m->action == ACT_FLYING) {
					air_time_target = 2.0f;
				}
				break;
			
			case ACT_GROUP_SUBMERGED:
				water_target = 1.0f;
				if (m->forwardVel > 24.0f) { // top speed swimming
					water_target = 2.0f;
				}
				break;
		}
		
		// apply Mario's current stats
		LERP(TRACKER_speed, absf(m->forwardVel), TRACKER_speed_LERP / (m->forwardVel > TRACKER_speed ? 1.0f : 3.0f));
		LERP(TRACKER_air_time, air_time_target, TRACKER_air_time_LERP / (air_time_target > TRACKER_air_time ? 1.0f : 2.0f));
		LERP(TRACKER_water, water_target, TRACKER_water_LERP / (water_target > TRACKER_water ? 1.0f : 2.0f));
		LERP(TRACKER_hp, (f32)(m->health >> 8), TRACKER_hp_LERP / ((f32)(m->health >> 8) < TRACKER_hp ? 1.0f : 2.0f));
		
		// calculate "conclusions"
		// collect = sqrt(accum stars + accum stars collect + accum stars secret courses)
		TRACKER_prefer_collect = sqrtf(TRACKER_accum_stars + TRACKER_accum_stars_prefer_collect + TRACKER_accum_stars_prefer_secret_courses);
		// explore = min(1.0 - star_preferences_gameplay[0..7]) + min(1.0 - star_preferences_level_type[0..7]) - (TRACKER_speed / 100.0f) - (sqrt(TRACKER_accum_stars) / 2.0f)
		for (i = 0; i < 8; i++) {
			f32 value_star_prefs_gameplay = 1.0f - absf(1.0f - TRACKER_star_preferences_gameplay[i]);
			f32 value_star_prefs_level_type = 1.0f - absf(1.0f - TRACKER_star_preferences_level_type[i]);
			
			if (value_star_prefs_gameplay < min_star_prefs_gameplay)
				min_star_prefs_gameplay = value_star_prefs_gameplay;
			if (value_star_prefs_level_type < min_star_prefs_level_type)
				min_star_prefs_level_type = value_star_prefs_level_type;
		}
		TRACKER_prefer_exploration = min_star_prefs_gameplay + min_star_prefs_level_type - (TRACKER_speed / 100.0f) - (sqrtf(TRACKER_accum_stars) / 2.0f);
		TRACKER_prefer_lore = TRACKER_accum_social + TRACKER_accum_nerd - (TRACKER_speed / 24.0f);
		TRACKER_prefer_murder = TRACKER_accum_murder + TRACKER_star_preferences_gameplay[4];
		TRACKER_prefer_parkour = (TRACKER_speed / 24.0f) + TRACKER_air_time + TRACKER_star_preferences_gameplay[5];
		TRACKER_prefer_swimming = TRACKER_water + TRACKER_star_preferences_level_type[5];
		// modifiers, 1.0 is the base value
		if (unlockDynamicDifficulty)
			TRACKER_difficulty_modifier = __max(
				moveTo(
					(((TRACKER_speed - 24.0f) / 16.0f) + TRACKER_accum_stars / 4.0f - TRACKER_accum_deaths) / 6.0f + __max(TRACKER_accum_murder / 6.0f, 0.4f) + 0.5f + TRACKER_boss_performance / 20.0f
				, 1.0f, 0.1f)
			, 0.0f);
		else
			TRACKER_difficulty_modifier = 1.0f;
		if (unlockLevelScale) {
			TRACKER_level_scale_modifier_h = moveTo(__max((__min(TRACKER_speed, 64.0f) + 24.0f) / 48.0f, 0.5f), 1.0f, 0.075f);
			TRACKER_level_scale_modifier_v = moveTo(__max((__min(TRACKER_speed, 64.0f) + 24.0f) / 72.0f, 0.5f) + (TRACKER_air_time + 0.25f) / 2.0f, 1.0f, 0.075f);
		}
		else {
			TRACKER_level_scale_modifier_h = 1.0f;
			TRACKER_level_scale_modifier_v = 1.0f;
		}
		
		if (nightMode) {
			TRACKER_difficulty_modifier *= 0.9f;
		}
		//TRACKER_difficulty_modifier *= 3.3f;
		
		// some common calculations derived from the difficulty modifier for bhv use
		TRACKER_difficulty_modifier_half = 0.5f + TRACKER_difficulty_modifier / 2.0f;
		TRACKER_difficulty_modifier_sqrt_half = 0.5f + sqrtf(TRACKER_difficulty_modifier) / 2.0f;
	}
	
    spawningLootCoins = FALSE;
}

u8 isDead = FALSE;
void TRACKER_handle_mario_state_change(struct MarioState *m) {
	register u8 prevIsDead = isDead;
	register s32 action = m->action;
	register s32 prevAction = m->prevAction;
	register s32 currAction = m->action;
	
	isDead = (m->hurtCounter - m->healCounter) > (m->health >> 6) - 4; 
	
	if (currAction == ACT_EATEN_BY_BUBBA) {
		// murdered by an enemy, but the game never registers this otherwise
		TRACKER_accum_deaths += 1.0f;
		TRACKER_accum_deaths_enemy_knockback += 1.0f;
	}
	else if (isDead && !prevIsDead) {
		TRACKER_accum_deaths += 1.0f;
		
		// get death type
		if ((action & (ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE)) == (ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE)) {
			// mario dying, find out what caused it then
			action = prevAction;
		}
		
		if (action == ACT_BURNING_GROUND || action == ACT_BURNING_JUMP || action == ACT_BURNING_FALL || action == ACT_LAVA_BOOST) {
			TRACKER_accum_deaths_fire_lava += 1.0f;
		}
		else if (action & ACT_FLAG_INVULNERABLE) {
			// non-dying invulnerable actions are (mostly) knockbacks
			TRACKER_accum_deaths_enemy_knockback += 1.0f;
		}
		else if (action & ACT_FLAG_SWIMMING) {
			// drowning
			TRACKER_accum_deaths_skill_issue += 1.0f;
		}
		// death barrier and troll are tracked outside
	}
}

void TRACKER_on_npc_talk(register struct Object *o) {
	if (get_red_star_count(gCurrSaveFileNum - 1) == 0) return;

	if ((u32)o->header.gfx.sharedChild == (u32)gLoadedGraphNodes[MODEL_WOODEN_SIGNPOST])
		TRACKER_accum_nerd += 0.25f;
	else
		TRACKER_accum_social += 0.2f;
}



// Saving and loading of funny personalization
s16 f32_to_s16(f32 in) {
	s32 save = (s32)(in * 256.0f);
	if (save < -32768) save = -32768;
	if (save > 32767) save = 32767;
	
	return (s16)save;
}
u8 f32_to_u8(f32 in) {
	s32 save = (s32)(in * 16.0f);
	if (save < 0) save = 0;
	if (save > 255) save = 255;
	
	return (u8)save;
}

f32 s16_to_f32(s16 in) {
	return in / 256.0f;
}
f32 u8_to_f32(u8 in) {
	return in / 16.0f;
}

void TRACKER_inject_save() {
	s32 i;
	f32 *ptr;

	u16 *menuData = (u16*)&gSaveBuffer.menuData.aiData;
	
	// the backup system had to be disabled to fit everything into the 512 byte eeprom
	// hopefully save doesn't get corrupted :( that would be unfortunate
	
	// anyways let's add shit
	*(menuData++) = personalizationRandSeed;
	*(menuData++) = f32_to_s16(TRACKER_speed);
	*(menuData++) = (f32_to_u8(TRACKER_air_time * 4.0f) << 8) + f32_to_u8(TRACKER_water * 4.0f);
	*(menuData++) = (f32_to_u8(TRACKER_hp * 2.0f) << 8) + f32_to_u8(TRACKER_accum_deaths_enemy_knockback);
	*(menuData++) = (f32_to_u8(TRACKER_accum_deaths_skill_issue) << 8) + f32_to_u8(TRACKER_accum_deaths_fire_lava);
	*(menuData++) = (f32_to_u8(TRACKER_accum_deaths_death_barrier) << 8) + f32_to_u8(TRACKER_accum_deaths_trolled);
	*(menuData++) = f32_to_s16(TRACKER_boss_performance);
	*(menuData++) = f32_to_s16(TRACKER_accum_deaths);
	*(menuData++) = (f32_to_u8(TRACKER_accum_social) << 8) + f32_to_u8(TRACKER_accum_nerd);
	*(menuData++) = f32_to_s16(TRACKER_accum_murder);
	*(menuData++) = f32_to_s16(TRACKER_accum_stars);
	
	ptr = &TRACKER_accum_stars_prefer_boss;
	for (i = 0; i < 4; i++) {
		*(menuData++) = (f32_to_u8(*ptr) << 8) + f32_to_u8(*(ptr + 1));
		ptr += 2;
	}
	ptr = &TRACKER_accum_stars_prefer_buildings;
	for (i = 0; i < 4; i++) {
		*(menuData++) = (f32_to_u8(*ptr) << 8) + f32_to_u8(*(ptr + 1));
		ptr += 2;
	}
	
	*(menuData++) = (f32_to_u8(TRACKER_accum_stars_prefer_wing_cap) << 8) + f32_to_u8(TRACKER_accum_stars_prefer_metal_cap);
	*(menuData++) = (f32_to_u8(TRACKER_accum_stars_prefer_vanish_cap) << 8);
	*(menuData++) = TRACKER_flags;
}

void TRACKER_read_save() {
	s32 i;
	f32 *ptr;

	u16 *menuData = (u16*)&gSaveBuffer.menuData.aiData;
	
	personalizationRandSeed = *(menuData++);
	if (personalizationRandSeed == 0 || personalizationRandSeed == 0xFFFF) {
		// we don't actually have any data lol
		// randomly generate a cool new seed
		while (personalizationRandSeed == 0 || personalizationRandSeed == 0xFFFF) {
			personalizationRandSeed = random_u16() ^ (u16)osGetTime() ^ (u16)(rtcHour * 3600 + rtcMinute * 60 + rtcSecond);
		}
		return;
	}
	
	TRACKER_speed = __max(s16_to_f32(*(menuData++)), 0.0f);
	TRACKER_air_time = u8_to_f32((*menuData) >> 8) / 4.0f;
	TRACKER_water = u8_to_f32(*menuData) / 4.0f;
	menuData++;
	TRACKER_hp = u8_to_f32((*menuData) >> 8) / 2.0f;
	TRACKER_accum_deaths_enemy_knockback = u8_to_f32(*menuData);
	menuData++;
	TRACKER_accum_deaths_skill_issue = u8_to_f32((*menuData) >> 8);
	TRACKER_accum_deaths_fire_lava = u8_to_f32(*menuData);
	menuData++;
	TRACKER_accum_deaths_death_barrier = u8_to_f32((*menuData) >> 8);
	TRACKER_accum_deaths_trolled = u8_to_f32(*menuData);
	menuData++;
	TRACKER_boss_performance = s16_to_f32(*(menuData++));
	TRACKER_accum_deaths = s16_to_f32(*(menuData++));
	TRACKER_accum_social = u8_to_f32((*menuData) >> 8);
	TRACKER_accum_nerd = u8_to_f32(*menuData);
	menuData++;
	TRACKER_accum_murder = s16_to_f32(*(menuData++));
	TRACKER_accum_stars = s16_to_f32(*(menuData++));
	
	ptr = &TRACKER_accum_stars_prefer_boss;
	for (i = 0; i < 4; i++) {
		*(ptr++) = u8_to_f32((*menuData) >> 8);
		*(ptr++) = u8_to_f32(*menuData);
		menuData++;
	}
	ptr = &TRACKER_accum_stars_prefer_buildings;
	for (i = 0; i < 4; i++) {
		*(ptr++) = u8_to_f32((*menuData) >> 8);
		*(ptr++) = u8_to_f32(*menuData);
		menuData++;
	}
	
	TRACKER_accum_stars_prefer_wing_cap = u8_to_f32((*menuData) >> 8);
	TRACKER_accum_stars_prefer_metal_cap = u8_to_f32(*menuData);
	menuData++;
	TRACKER_accum_stars_prefer_vanish_cap = u8_to_f32((*menuData) >> 8);
	//TRACKER_accum_nerd = u8_to_f32(*menuData);
	menuData++;
	TRACKER_flags = *menuData;
}
