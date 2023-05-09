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

s32 dynamicSurfaceTris = 0;
struct Object *debugCurrObject = NULL;
u8 *watchDMADest = 0;
u8 *watchDMASrcStart = 0;
u32 watchDMASize = 0;

char *hudTypes[] = {
	"NO OBJ", "NORMAL", "B ROLL", "SHOSHINKAI", "E3", "DECEMBER"
};
char *terrainTypes[] = {
	"NO OBJ", "CAVE", "LAVA", "GRASS", "STONE", "SNOW", "SAND", "SPOOKY", "WATER", "SLIDE"
};
char *nightTypes[] = {
	"NO OBJ", "RTC OR DAY", "RTC OR NIGHT", "FLIP RTC OR DAY", "FLIP RTC OR NIGHT", "DAY", "NIGHT"
};

void print_level_information() {
	char float_buffer[10];
	
	u32 bparams = 0xFFFFFFFF;
	
	register s32 i;
	register struct Object *obj = &gObjectPool[0];
	for (i = 0; i < 240; i++) {
		if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && (u32)obj->behavior >= 0x80000000) {
			if (obj->behavior == segmented_to_virtual(bhvLoadBlueGomba)) {
				bparams = obj->oBehParams;
				break; // found the object
			}
		}
		
		obj++;
	}
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "STAGE INFO");
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 16, "COURSE");
	sprintf(float_buffer, "%x", gCurrLevelNum);
	print_text(HUD_LEFT_X + 96, HUD_TOP_Y - 16, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "AREA");
	sprintf(float_buffer, "%d", gCurrentArea->index);
	print_text(HUD_LEFT_X + 96, HUD_TOP_Y - 32, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 48, "HUD");
	print_text(HUD_LEFT_X + 96, HUD_TOP_Y - 48, hudTypes[(u8)((bparams >> 24) + 1)]);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 64, "TERRAIN");
	print_text(HUD_LEFT_X + 96, HUD_TOP_Y - 64, terrainTypes[(u8)((bparams >> 16) + 1)]);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 80, "NIGHT");
	print_text(HUD_LEFT_X + 96, HUD_TOP_Y - 80, nightTypes[(u8)((bparams >> 8) + 1)]);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 96, "POS");
	sprintf(float_buffer, "%d", (s32)gMarioState->pos[0]);
	print_text(HUD_LEFT_X + 64, HUD_TOP_Y - 96, float_buffer);
	sprintf(float_buffer, "%d", (s32)gMarioState->pos[1]);
	print_text(HUD_LEFT_X + 64+80, HUD_TOP_Y - 96, float_buffer);
	sprintf(float_buffer, "%d", (s32)gMarioState->pos[2]);
	print_text(HUD_LEFT_X + 64+80*2, HUD_TOP_Y - 96, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 112, "REDS");
	sprintf(float_buffer, "%d", get_red_star_count(gCurrSaveFileNum - 1));
	print_text(HUD_LEFT_X + 96, HUD_TOP_Y - 112, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 128, "GREENS");
	sprintf(float_buffer, "%d", get_green_star_count(gCurrSaveFileNum - 1));
	print_text(HUD_LEFT_X + 96, HUD_TOP_Y - 128, float_buffer);
}

extern float _gLastFrameTime;
extern s32 render_frame_count, last_render_frame_count;
void print_performance_information() {
	char float_buffer[10];
	
	u32 bparams = 0xFFFFFFFF;
	
	register s32 i;
	register struct Object *obj = &gObjectPool[0];
	register s32 objCount = 0;
	for (i = 0; i < 240; i++) {
		if (obj->activeFlags & ACTIVE_FLAG_ACTIVE && (u32)obj->behavior >= 0x80000000) {
			objCount++;
			if (obj->behavior == segmented_to_virtual(bhvLoadBlueGomba)) {
				bparams = obj->oBehParams;
			}
		}
		
		obj++;
	}
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "PROFILER");
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 16, "TIME");
	sprintf(float_buffer, "%.3f", _gLastFrameTime);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 16, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "FCOUNT");
	sprintf(float_buffer, "%d", render_frame_count);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 32, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 48, "DSURFS");
	sprintf(float_buffer, "%d", dynamicSurfaceTris);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 48, float_buffer);
	dynamicSurfaceTris = 0;
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 64, "OBJ");
	sprintf(float_buffer, "%d", objCount);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 64, float_buffer);
}

void print_general_stats() {
	char float_buffer[10];
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "GEN TRACKS");
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 16, "SPD");
	sprintf(float_buffer, "%.3f", TRACKER_speed);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 16, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "AIR");
	sprintf(float_buffer, "%.3f", TRACKER_air_time);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 32, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 48, "WATER");
	sprintf(float_buffer, "%.3f", TRACKER_water);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 48, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 64, "HEALTH");
	sprintf(float_buffer, "%.3f", TRACKER_hp);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 64, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 80, "BOSS");
	sprintf(float_buffer, "%.3f", TRACKER_boss_performance);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 80, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 96, "DEATH");
	sprintf(float_buffer, "%.3f", TRACKER_accum_deaths);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 96, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 112, "SOCIAL");
	sprintf(float_buffer, "%.3f", TRACKER_accum_social);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 112, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 128, "NERD");
	sprintf(float_buffer, "%.3f", TRACKER_accum_nerd);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 128, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 144, "KILL");
	sprintf(float_buffer, "%.3f", TRACKER_accum_murder);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 144, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 160, "STARS");
	sprintf(float_buffer, "%.3f", TRACKER_accum_stars);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 160, float_buffer);
}

void print_death_stats() {
	char float_buffer[10];
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "DEATH REASONS");
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 16, "ENEMY");
	sprintf(float_buffer, "%.3f", TRACKER_death_reasons[0]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 16, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "BAD");
	sprintf(float_buffer, "%.3f", TRACKER_death_reasons[1]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 32, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 48, "HOT");
	sprintf(float_buffer, "%.3f", TRACKER_death_reasons[2]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 48, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 64, "FALL");
	sprintf(float_buffer, "%.3f", TRACKER_death_reasons[3]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 64, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 80, "TROLL");
	sprintf(float_buffer, "%.3f", TRACKER_death_reasons[4]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 80, float_buffer);
}

void print_star_prefs_gameplay() {
	char float_buffer[10];
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "- PREFS GAMEPLAY");
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 16, "BOSS");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_gameplay[0]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 16, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "CANNON");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_gameplay[1]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 32, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 48, "GRAB");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_gameplay[2]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 48, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 64, "FREE");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_gameplay[3]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 64, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 80, "KILL");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_gameplay[4]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 80, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 96, "WAHOO");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_gameplay[5]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 96, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 112, "SLIDE");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_gameplay[6]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 112, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 128, "TIMER");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_gameplay[7]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 128, float_buffer);
}

void print_star_prefs_level_type() {
	char float_buffer[10];
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "- PREFS AREA TYPE");
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 16, "HOUSE");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_level_type[0]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 16, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "GROUND");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_level_type[1]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 32, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 48, "SNOW");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_level_type[2]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 48, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 64, "DESERT");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_level_type[3]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 64, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 80, "UNDERG");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_level_type[4]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 80, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 96, "WATER");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_level_type[5]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 96, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 112, "FIRE");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_level_type[6]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 112, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 128, "SECRET");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_level_type[7]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 128, float_buffer);
}

void print_star_prefs_caps() {
	char float_buffer[10];
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "- PREFS CAPS");
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 16, "WING");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_cap[0]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 16, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "METAL");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_cap[1]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 32, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 48, "GHOST");
	sprintf(float_buffer, "%.3f", TRACKER_star_preferences_cap[2]);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 48, float_buffer);
}

void print_prefs() {
	char float_buffer[10];
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "PREFERENCES");
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 16, "GRAB");
	sprintf(float_buffer, "%.3f", TRACKER_prefer_collect);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 16, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "EXPLOR");
	sprintf(float_buffer, "%.3f", TRACKER_prefer_exploration);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 32, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 48, "LORE");
	sprintf(float_buffer, "%.3f", TRACKER_prefer_lore);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 48, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 64, "KILL");
	sprintf(float_buffer, "%.3f", TRACKER_prefer_murder);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 64, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 80, "WAHOO");
	sprintf(float_buffer, "%.3f", TRACKER_prefer_parkour);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 80, float_buffer);
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 96, "WATER");
	sprintf(float_buffer, "%.3f", TRACKER_prefer_swimming);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 96, float_buffer);
}

void print_mods() {
	char float_buffer[10];

	print_text(HUD_LEFT_X, HUD_TOP_Y, "MODIFIERS");

	print_text(HUD_LEFT_X, HUD_TOP_Y - 16, "HARD");
	sprintf(float_buffer, "%.3f", TRACKER_difficulty_modifier);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 16, float_buffer);

	print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "SCALEH");
	sprintf(float_buffer, "%.3f", TRACKER_level_scale_modifier_h);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 32, float_buffer);

	print_text(HUD_LEFT_X, HUD_TOP_Y - 48, "SCALEY");
	sprintf(float_buffer, "%.3f", TRACKER_level_scale_modifier_v);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 48, float_buffer);
}

void print_obj_debug() {
	char float_buffer[10];

	if (debugCurrObject == NULL) {
		print_text(HUD_LEFT_X, HUD_TOP_Y, "NO OBJ FOUND");
		return;
	}

	print_text(HUD_LEFT_X, HUD_TOP_Y, "OBJ DEBUG");

	{
		s32 modelId = 0;
		for (; modelId <= 255; modelId++) {
			if (debugCurrObject->header.gfx.sharedChild == gLoadedGraphNodes[modelId]) break;
		}

		print_text(HUD_LEFT_X, HUD_TOP_Y - 16, "MODEL");
		sprintf(float_buffer, "%d", modelId);
		print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 16, float_buffer);

		print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "BHV");
		sprintf(float_buffer, "%x", debugCurrObject->curBhvCommand);
		print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 32, float_buffer);

		print_text(HUD_LEFT_X, HUD_TOP_Y - 48, "*BHV");
		sprintf(float_buffer, "%x", *debugCurrObject->curBhvCommand);
		print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 48, float_buffer);

		print_text(HUD_LEFT_X, HUD_TOP_Y - 64, "DMA");
		sprintf(float_buffer, "%x", watchDMADest);
		print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 64, float_buffer);
		sprintf(float_buffer, "%x", watchDMASrcStart);
		print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 80, float_buffer);
		sprintf(float_buffer, "%x", watchDMASize);
		print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 96, float_buffer);
	}

	/*print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "SCALEH");
	sprintf(float_buffer, "%.3f", TRACKER_level_scale_modifier_h);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 32, float_buffer);

	print_text(HUD_LEFT_X, HUD_TOP_Y - 48, "SCALEY");
	sprintf(float_buffer, "%.3f", TRACKER_level_scale_modifier_v);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 48, float_buffer);*/
}

int musSelection = 0;
int musForceNLST = 0;
int musTempo = 0;
int musPitch = 7;
int musTranspose = 0;
int nowPlaying_nlst = 0;
int nowPlaying_tempo = 0;
int soundTestSelection = 0;
void sound_test() {
	char float_buffer[10];
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "SOUND TEST");
	
	print_text(HUD_LEFT_X + (soundTestSelection == 0 ? 16 : 0), HUD_TOP_Y - 24, "MUSIC");
	sprintf(float_buffer, "%d", musSelection);
	print_text(HUD_LEFT_X + (soundTestSelection == 0 ? 16 : 0) + 80, HUD_TOP_Y - 24, float_buffer);
	if (soundTestSelection == 0) {
		if (gPlayer1Controller->buttonPressed & L_JPAD) {
			if (--musSelection < 0) musSelection = 0;
		}
		if (gPlayer1Controller->buttonPressed & R_JPAD) {
			if (++musSelection > 100) musSelection = 100;
		}
	}
	
	print_text(HUD_LEFT_X + (soundTestSelection == 1 ? 16 : 0), HUD_TOP_Y - 40, "NLST");
	if (musForceNLST == 0) {
		print_text(HUD_LEFT_X + (soundTestSelection == 1 ? 16 : 0) + 80, HUD_TOP_Y - 40, "DEFAULT");
	}
	else {
		sprintf(float_buffer, "%d", musForceNLST);
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
	sprintf(float_buffer, "%d", musTempo);
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
	sprintf(float_buffer, "%d", musPitch);
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
	sprintf(float_buffer, "%d", musTranspose);
	print_text(HUD_LEFT_X + (soundTestSelection == 4 ? 16 : 0) + 80, HUD_TOP_Y - 88, float_buffer);
	if (soundTestSelection == 4) {
		if (gPlayer1Controller->buttonPressed & L_JPAD) {
			if (--musTranspose < -8) musTranspose = -8;
		}
		if (gPlayer1Controller->buttonPressed & R_JPAD) {
			if (++musTranspose > 7) musTranspose = 7;
		}
	}
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 112, "L  PLAY");
	
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 136, "CURRENT");
	print_text(HUD_LEFT_X, HUD_TOP_Y - 152, "NLST");
	sprintf(float_buffer, "%d", nowPlaying_nlst);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 152, float_buffer);
	print_text(HUD_LEFT_X, HUD_TOP_Y - 168, "TEMPO");
	sprintf(float_buffer, "%d", nowPlaying_tempo);
	print_text(HUD_LEFT_X + 80, HUD_TOP_Y - 168, float_buffer);
	
	
	
	if (gPlayer1Controller->buttonPressed & U_JPAD) {
		if (--soundTestSelection < 0) soundTestSelection = 4;
	}
	if (gPlayer1Controller->buttonPressed & D_JPAD) {
		if (++soundTestSelection > 4) soundTestSelection = 0;
	}
}

void haks() {
	char float_buffer[10];
	
	print_text(HUD_LEFT_X, HUD_TOP_Y, "HAKS");
	
	print_text(HUD_LEFT_X, HUD_TOP_Y - 16, "LEFT   DEBUG MOVE");
	if (gPlayer1Controller->buttonPressed & L_JPAD) {
		set_mario_action(gMarioState, gMarioState->action == ACT_DEBUG_FREE_MOVE ? ACT_IDLE : ACT_DEBUG_FREE_MOVE, 0);
	}

	print_text(HUD_LEFT_X, HUD_TOP_Y - 32, "RIGHT  FUNY SPAWN");
	if (gPlayer1Controller->buttonPressed & R_JPAD) {
		// spawn object
		u8 spawnModel = 1;
		u32 *spawnBhv = (u32*)segmented_to_virtual(bhvPoleGrabbing);

		struct Object *obj = create_object(spawnBhv);
		struct SpawnInfo spawnInfo;

		// Behavior parameters are often treated as four separate bytes, but
		// are stored as an s32.
		obj->oBehParams = (random_u16() & 255) << 16;
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
	}
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
				case 10:
					print_obj_debug();
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
					tab = (tab + 1) % 11;
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
	if (gCurrentObject->header.gfx.sharedChild == gLoadedGraphNodes[MODEL_MARIO])
		debugCurrObject = gCurrentObject;
}

#define ALIGN16(val) (((val) + 0xF) & ~0xF)
void intercept_dma(u8 *dest, u8 *srcStart, u8 *srcEnd) {
    register u32 size = ALIGN16(srcEnd - srcStart);
	register u32 checkPtr = 0x8011A3A0;
	register u32 startPtr = (u32)dest;

	if (startPtr <= checkPtr && checkPtr < (startPtr + size)) {
		watchDMADest = dest;
		watchDMASrcStart = srcStart;
		watchDMASize = size;
	}
}

