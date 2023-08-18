#include <PR/ultratypes.h>
#include "sm64.h"
#include "stats_tracking.h"
#include "stats_tracking_debug.h"
#include "personalization_helpers.h"
#include "game/object_list_processor.h"
#include "game/spawn_object.h"
#include "behavior_data.h"

#include "game/print.h"
#include "game/level_update.h"
#include "game/game_init.h"

#include <stdio.h>


#define HUD_TOP_Y 185
#define HUD_LEFT_X 20

extern const BehaviorScript bhvTextOnScreen[];

s32 dynamicSurfaceTris = 0;
s32 displayHeapSize = 0x3500;
s32 *displayHeapUsed = NULL;

char *hudTypes[] = {
	"NO OBJ", "NORMAL", "B ROLL", "SHOSHINKAI", "E3", "DECEMBER"
};
char *terrainTypes[] = {
	"NO OBJ", "CAVE", "LAVA", "GRASS", "STONE", "SNOW", "SAND", "SPOOKY", "WATER", "SLIDE"
};
char *nightTypes[] = {
	"NO OBJ", "RTC OR DAY", "RTC OR NIGHT", "FLIP RTC OR DAY", "FLIP RTC OR NIGHT", "DAY", "NIGHT"
};

char float_buffer[10];

void print_text_value_helper(s32 xOffset, s32 yOffset, char *ptrLeft, char *ptrRight) {
	print_text(HUD_LEFT_X, yOffset, ptrLeft);
	print_text(xOffset, yOffset, ptrRight);
}

void sprintf_hex(s32 value) {
    sprintf(float_buffer, "%x", value);
}
void sprintf_decimal(s32 value) {
    sprintf(float_buffer, "%d", value);
}
void sprintf_float(f32 value) {
    sprintf(float_buffer, "%.3f", value);
}

void print_level_information() {
	u32 bparams = 0xFFFFFFFF;
	
	register s32 i;
	register struct ObjectNode *listHead;
	register struct Object *obj;
	for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
		listHead = &gObjectLists[i];

		obj = (struct Object *) listHead->next;

		while (obj != (struct Object *) listHead) {
			if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && (u32)obj->behavior >= 0x80000000) {
				if (obj->behavior == segmented_to_virtual(bhvLoadBlueGomba)) {
					bparams = obj->oBehParams;
					break; // found the object
				}
			}
			obj = (struct Object *) obj->header.next;
		}
	}
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "STAGE INFO");
	
	sprintf_hex(gCurrLevelNum);
	print_text_value_helper(HUD_LEFT_X + 96, HUD_TOP_Y - 16, "COURSE", float_buffer);
	
	sprintf_decimal(gCurrentArea->index);
	print_text_value_helper(HUD_LEFT_X + 96, HUD_TOP_Y - 32, "AREA", float_buffer);
	
	print_text_value_helper(HUD_LEFT_X + 96, HUD_TOP_Y - 48, "HUD", hudTypes[(u8)((bparams >> 24) + 1)]);
	
	print_text_value_helper(HUD_LEFT_X + 96, HUD_TOP_Y - 64, "TERRAIN", terrainTypes[(u8)((bparams >> 16) + 1)]);
	
	print_text_value_helper(HUD_LEFT_X + 96, HUD_TOP_Y - 80, "NIGHT", nightTypes[(u8)((bparams >> 8) + 1)]);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 96, "POS");
	sprintf_decimal((s32)gMarioState->pos[0]);
	print_text(HUD_LEFT_X + 64, HUD_TOP_Y - 96, float_buffer);
	sprintf_decimal((s32)gMarioState->pos[1]);
	print_text(HUD_LEFT_X + 64+80, HUD_TOP_Y - 96, float_buffer);
	sprintf_decimal((s32)gMarioState->pos[2]);
	print_text(HUD_LEFT_X + 64+80*2, HUD_TOP_Y - 96, float_buffer);
	
	sprintf_decimal(get_red_star_count(gCurrSaveFileNum - 1));
	print_text_value_helper(HUD_LEFT_X + 96, HUD_TOP_Y - 112, "REDS", float_buffer);
	
	sprintf_decimal(get_green_star_count(gCurrSaveFileNum - 1));
	print_text_value_helper(HUD_LEFT_X + 96, HUD_TOP_Y - 128, "GREENS", float_buffer);
}

extern float _gLastFrameTime;
extern s32 render_frame_count, last_render_frame_count;
void print_performance_information() {
	u32 bparams = 0xFFFFFFFF;
	
	register s32 i;
	register struct ObjectNode *listHead;
	register struct Object *obj;
	for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
		listHead = &gObjectLists[i];

		obj = (struct Object *) listHead->next;

		while (obj != (struct Object *) listHead) {
			if (obj->activeFlags & ACTIVE_FLAG_ACTIVE && (u32)obj->behavior >= 0x80000000) {
				if (obj->behavior == segmented_to_virtual(bhvLoadBlueGomba)) {
					bparams = obj->oBehParams;
				}
			}
			obj = (struct Object *) obj->header.next;
		}
	}
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "PROFILER");
	
	sprintf_float(_gLastFrameTime);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 16, "TIME", float_buffer);
	
	sprintf_decimal(render_frame_count);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 32, "FCOUNT", float_buffer);
	
	sprintf_decimal(dynamicSurfaceTris);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 48, "DSURFS", float_buffer);
	dynamicSurfaceTris = 0;
	
	sprintf_decimal(loadedObjectCount);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 64, "OBJ", float_buffer);
}

void print_general_stats() {
	print_text(HUD_LEFT_X, HUD_TOP_Y, "GEN TRACKS");
	
	sprintf_float(TRACKER_speed);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 16, "SPD", float_buffer);
	
	sprintf_float(TRACKER_air_time);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 32, "AIR", float_buffer);
	
	sprintf_float(TRACKER_water);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 48, "WATER", float_buffer);
	
	sprintf_float(TRACKER_hp);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 64, "HEALTH", float_buffer);
	
	sprintf_float(TRACKER_boss_performance);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 80, "BOSS", float_buffer);
	
	sprintf_float(TRACKER_accum_deaths);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 96, "DEATH", float_buffer);
	
	sprintf_float(TRACKER_accum_social);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 112, "SOCIAL", float_buffer);
	
	sprintf_float(TRACKER_accum_nerd);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 128, "NERD", float_buffer);
	
	sprintf_float(TRACKER_accum_murder);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 144, "KILL", float_buffer);
	
	sprintf_float(TRACKER_accum_stars);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 160, "STARS", float_buffer);
}

char *death_reasons_text[] = {
	"ENEMY", "BAD", "HOT", "FALL", "TROLL"
};
void print_death_stats() {
	s32 i;
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "DEATH REASONS");
	
	for (i = 0; i < 5; i++) {
		sprintf_float(TRACKER_death_reasons[i]);
		print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 16 * (i + 1), death_reasons_text[i], float_buffer);
	}
}

char *star_prefs_gameplay_text[] = {
	"BOSS", "CANNON", "GRAB", "FREE", "KILL", "WAHOO", "SLIDE", "TIMER"
};
void print_star_prefs_gameplay() {
	s32 i;
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "- PREFS GAMEPLAY");

	for (i = 0; i < 8; i++) {
		sprintf_float(TRACKER_star_preferences_gameplay[i]);
		print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 16 * (i + 1), star_prefs_gameplay_text[i], float_buffer);
	}
}

char *star_prefs_level_type_text[] = {
	"HOUSE", "GROUND", "SNOW", "DESERT", "UNDERG", "WATER", "FIRE", "SECRET"
};
void print_star_prefs_level_type() {
	s32 i;
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "- PREFS AREA TYPE");
	
	for (i = 0; i < 8; i++) {
		sprintf_float(TRACKER_star_preferences_level_type[i]);
		print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 16 * (i + 1), star_prefs_level_type_text[i], float_buffer);
	}
}

char *star_prefs_caps_text[] = {
	"WING", "METAL", "GHOST"
};
void print_star_prefs_caps() {
	s32 i;
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "- PREFS CAPS");
	
	for (i = 0; i < 3; i++) {
		sprintf_float(TRACKER_star_preferences_cap[i]);
		print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 16 * (i + 1), star_prefs_caps_text[i], float_buffer);
	}
}

void print_prefs() {
	print_text(HUD_LEFT_X, HUD_TOP_Y, "PREFERENCES");
	
	sprintf_float(TRACKER_prefer_collect);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 16, "GRAB", float_buffer);
	
	sprintf_float(TRACKER_prefer_exploration);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 32, "EXPLOR", float_buffer);
	
	sprintf_float(TRACKER_prefer_lore);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 48, "LORE", float_buffer);
	
	sprintf_float(TRACKER_prefer_murder);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 64, "KILL", float_buffer);

	sprintf_float(TRACKER_prefer_parkour);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 80, "WAHOO", float_buffer);
	
	sprintf_float(TRACKER_prefer_swimming);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 96, "WATER", float_buffer);
}

void print_mods() {
	print_text(HUD_LEFT_X, HUD_TOP_Y, "MODIFIERS");

	sprintf_float(TRACKER_difficulty_modifier);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 16, "HARD", float_buffer);

	sprintf_float(TRACKER_level_scale_modifier_h);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 32, "SCALEH", float_buffer);

	sprintf_float(TRACKER_level_scale_modifier_v);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 48, "SCALEY", float_buffer);

	sprintf_float(levelScaleH);
	print_text_value_helper(HUD_LEFT_X + 144, HUD_TOP_Y - 80, "SCALEH REAL", float_buffer);

	sprintf_float(levelScaleV);
	print_text_value_helper(HUD_LEFT_X + 144, HUD_TOP_Y - 96, "SCALEY REAL", float_buffer);
}

int musSelection = 0;
int musForceNLST = 0;
int musTempo = 0;
int musPitch = 7;
int musTranspose = 0;
int nowPlaying_seqId = 0;
int nowPlaying_nlst = 0;
int nowPlaying_tempo = 0;
int soundTestSelection = 0;
void sound_test() {
	print_text(HUD_LEFT_X, HUD_TOP_Y, "SOUND TEST");
	
	print_text(HUD_LEFT_X + (soundTestSelection == 0 ? 16 : 0), HUD_TOP_Y - 24, "MUSIC");
	sprintf_decimal(musSelection);
	print_text(HUD_LEFT_X + (soundTestSelection == 0 ? 16 : 0) + 80, HUD_TOP_Y - 24, float_buffer);
	if (soundTestSelection == 0) {
		if (gPlayer1Controller->buttonPressed & L_JPAD) {
			if (--musSelection < 0) musSelection = 0;
		}
		if (gPlayer1Controller->buttonPressed & R_JPAD) {
			if (++musSelection > 255) musSelection = 255;
		}
	}
	
	print_text(HUD_LEFT_X + (soundTestSelection == 1 ? 16 : 0), HUD_TOP_Y - 40, "NLST");
	if (musForceNLST == 0) {
		print_text(HUD_LEFT_X + (soundTestSelection == 1 ? 16 : 0) + 80, HUD_TOP_Y - 40, "DEFAULT");
	}
	else {
		sprintf_decimal(musForceNLST);
		print_text(HUD_LEFT_X + (soundTestSelection == 1 ? 16 : 0) + 80, HUD_TOP_Y - 40, float_buffer);
	}
	if (soundTestSelection == 1) {
		if (gPlayer1Controller->buttonPressed & L_JPAD) {
			if (--musForceNLST < 0) musForceNLST = 0;
		}
		if (gPlayer1Controller->buttonPressed & R_JPAD) {
			if (++musForceNLST > 37) musForceNLST = 37;
		}
	}
	
	print_text(HUD_LEFT_X + (soundTestSelection == 2 ? 16 : 0), HUD_TOP_Y - 56, "TEMPO");
	sprintf_decimal(musTempo);
	print_text(HUD_LEFT_X + (soundTestSelection == 2 ? 16 : 0) + 80, HUD_TOP_Y - 56, float_buffer);
	if (soundTestSelection == 2) {
		if (gPlayer1Controller->buttonDown & L_JPAD) {
			if (--musTempo < 0) musTempo = 0;
		}
		if (gPlayer1Controller->buttonDown & R_JPAD) {
			if (++musTempo > 255) musTempo = 255;
		}
	}
	
	print_text(HUD_LEFT_X + (soundTestSelection == 3 ? 16 : 0), HUD_TOP_Y - 72, "PITCH");
	sprintf_decimal(musPitch);
	print_text(HUD_LEFT_X + (soundTestSelection == 3 ? 16 : 0) + 80, HUD_TOP_Y - 72, float_buffer);
	if (soundTestSelection == 3) {
		if (gPlayer1Controller->buttonPressed & L_JPAD) {
			if (--musPitch < 0) musPitch = 0;
		}
		if (gPlayer1Controller->buttonPressed & R_JPAD) {
			if (++musPitch > 15) musPitch = 15;
		}
	}
	
	print_text(HUD_LEFT_X + (soundTestSelection == 4 ? 16 : 0), HUD_TOP_Y - 88, "TRANSP");
	sprintf_decimal(musTranspose);
	print_text(HUD_LEFT_X + (soundTestSelection == 4 ? 16 : 0) + 80, HUD_TOP_Y - 88, float_buffer);
	if (soundTestSelection == 4) {
		if (gPlayer1Controller->buttonPressed & L_JPAD) {
			if (--musTranspose < -8) musTranspose = -8;
		}
		if (gPlayer1Controller->buttonPressed & R_JPAD) {
			if (++musTranspose > 7) musTranspose = 7;
		}
	}
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 110, "L  PLAY");
	
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 132, "CURRENT");
	sprintf_decimal(nowPlaying_seqId);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 148, "MUSIC", float_buffer);
	sprintf_decimal(nowPlaying_nlst);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 164, "NLST", float_buffer);
	sprintf_decimal(nowPlaying_tempo);
	print_text_value_helper(HUD_LEFT_X + 80, HUD_TOP_Y - 180, "TEMPO", float_buffer);
	
	
	
	if (gPlayer1Controller->buttonPressed & U_JPAD) {
		if (--soundTestSelection < 0) soundTestSelection = 4;
	}
	if (gPlayer1Controller->buttonPressed & D_JPAD) {
		if (++soundTestSelection > 4) soundTestSelection = 0;
	}
}

void haks() {
	print_text(HUD_LEFT_X, HUD_TOP_Y, "HAKS");
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 16, "LEFT   DEBUG MOVE");
	if (gPlayer1Controller->buttonPressed & L_JPAD) {
		set_mario_action(gMarioState, gMarioState->action == ACT_DEBUG_FREE_MOVE ? ACT_IDLE : ACT_DEBUG_FREE_MOVE, 0);
	}

	/*print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "RIGHT  FUNY SPAWN");
	if (gPlayer1Controller->buttonPressed & R_JPAD) {
		// spawn object
		u8 spawnModel = 0;
		u32 *spawnBhv = (u32*)segmented_to_virtual(bhvTextOnScreen);

		struct Object *obj = create_object(spawnBhv);
		struct SpawnInfo spawnInfo;

		// Behavior parameters are often treated as four separate bytes, but
		// are stored as an s32.
		obj->oBehParams = ((random_u16() % 10) << 16) | (random_u16() & 0x0300);
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

		obj->oPosX = gMarioObject->oPosX;
		obj->oPosY = gMarioObject->oPosY;
		obj->oPosZ = gMarioObject->oPosZ;
	}*/
}

int tab = 0;
int tab2 = 0;

void stats_tracking_debug_display() {
	switch (tab2) {
		case 0:
			switch (tab) {
				case 0:
					break;
				case 1:
					print_level_information();
					break;
				case 2:
					print_performance_information();
					break;
				case 3:
					print_general_stats();
					break;
				case 4:
					print_death_stats();
					break;
				case 5:
					print_star_prefs_gameplay();
					break;
				case 6:
					print_star_prefs_level_type();
					break;
				case 7:
					print_star_prefs_caps();
					break;
				case 8:
					print_prefs();
					break;
				case 9:
					print_mods();
					break;
			}
			break;
		case 1:
			sound_test();
			break;
		case 2:
			haks();
			break;
	}
	
	
	if (gPlayer1Controller->buttonPressed & L_TRIG) {
		if (gPlayer1Controller->buttonDown & U_CBUTTONS) {
			tab2 = (tab2 + 1) % 3;
		}
		else {
			switch (tab2) {
				case 0:
					tab = (tab + 1) % 10;
					break;
				case 1:
					// playma music
					mus_tempooverride = musTempo;
					mus_pitchmul = (musPitch + 1) / 8.0f;
					mus_transposition = musTranspose;
					mus_nlstday = musForceNLST;
					mus_nlstnight = musForceNLST;
					
					set_background_music(gCurrentArea->musicParam, musSelection, 0);
					break;
			}
		}
	}
}


void set_cur_obj_debug_information() {
}

#define ALIGN16(val) (((val) + 0xF) & ~0xF)
void intercept_dma(u8 *dest, u8 *srcStart, u8 *srcEnd) {
}

