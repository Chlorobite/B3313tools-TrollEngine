#include <PR/ultratypes.h>
#include <ultra64.h>
#include "sm64.h"
#include "stats_tracking.h"
#include "personalization_helpers.h"
#include "game/main.h"


#include "behavior_data.h"
#include "audio/external.h"
#include "geo_commands.h"
#include "game/camera.h"
#include "game/decompress.h"
#include "game/game_init.h"
#include "game/geo_misc.h"
#include "game/hud.h"
#include "game/interaction.h"
#include "game/ingame_menu.h"
#include "game/level_geo.h"
#include "game/level_update.h"
#include "game/mario.h"
#include "game/mario_step.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/object_list_processor.h"
#include "game/obj_behaviors_2.h"
#include "game/paintings.h"
#include "surface_terrains.h"
#include "moving_texture_macros.h"
#include "level_misc_macros.h"
#include "macro_preset_names.h"
#include "special_preset_names.h"
#include "textures.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "segment_symbols.h"

#include "gfx_dimensions.h"



#define HUD_TOP_Y 210


char hudType = 0;

extern u8 *hud_crosses_start;
extern u8 *hud_stars_start;
extern u8 *hud_numbers_start;

void dma_read_current_hud() {
	void *seg2 = segmented_to_virtual((void*)0x02000000);
	
	switch (hudType) {
		case 0: // broll
			// broll cross
			dma_read((void*)((u32)seg2 + 16*16*2*(10+23)), &hud_crosses_start, &hud_crosses_start + (1*16*16*2) / 4 - 1);
			// broll star
			dma_read((void*)((u32)seg2 + 16*16*2*(10+26)), &hud_stars_start, &hud_stars_start + (1*16*16*2) / 4 - 1);
			// beta numbers
			dma_read(seg2, &hud_numbers_start, &hud_numbers_start + (10*16*16*2) / 4 - 1);
			break;
		case 1: // shoshinkai 1995
			// sonk cross
			dma_read((void*)((u32)seg2 + 16*16*2*(10+23)), &hud_crosses_start + (1*16*16*2) / 4, &hud_crosses_start + (2*16*16*2) / 4 - 1);
			// sonk star
			dma_read((void*)((u32)seg2 + 16*16*2*(10+26)), &hud_stars_start + (1*16*16*2) / 4, &hud_stars_start + (2*16*16*2) / 4 - 1);
			// beta numbers
			dma_read(seg2, &hud_numbers_start, &hud_numbers_start + (10*16*16*2) / 4 - 1);
			break;
		case 2: // vanilla
		case 3: // december
			// vanilla cross
			dma_read((void*)((u32)seg2 + 16*16*2*(10+23)), &hud_crosses_start + (2*16*16*2) / 4, &hud_crosses_start + (3*16*16*2) / 4 - 1);
			// sonk star
			dma_read((void*)((u32)seg2 + 16*16*2*(10+26)), &hud_stars_start + (1*16*16*2) / 4, &hud_stars_start + (2*16*16*2) / 4 - 1);
			// vanilla numbers
			dma_read(seg2, &hud_numbers_start + (10*16*16*2) / 4, &hud_numbers_start + 2*(10*16*16*2) / 4 - 1);
			break;
	}
}



void set_hud_type(s32 bparam) {
	/*hudType += 1;
	if (hudType == 3) hudType = 0;
	return;*/
	
	switch (bparam) {
		case 0:
			if (gMarioState->health >= 0x800 && (hudType == 0 || hudType == 1)) {
				// randomly switch between broll and sonk
				if (random_u16() < 5042) {
					hudType ^= 1;
				}
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			hudType = bparam - 1;
			break;
	}
	
	dma_read_current_hud();
}

void troll_render_hud_mario_lives() {
	//dma_read_current_hud();
	
    switch (hudType) {
		case 0: // bé tekercs
			print_text(30, HUD_TOP_Y, *((u8*)0x807FFFED) ? "\x5A" : "\x2C"); // 'Mario Head' glyph
			print_text(46, HUD_TOP_Y, "*"); // 'X' glyph
			print_text_fmt_int(58, HUD_TOP_Y, "%02d", gHudDisplay.lives);
			break;
		case 1: // sonkás pizza
			print_text(30, HUD_TOP_Y, *((u8*)0x807FFFED) ? "\x5A" : "\x2C"); // 'Mario Head' glyph
			print_text(46, HUD_TOP_Y, "*"); // 'X' glyph
			print_text_fmt_int(60, HUD_TOP_Y, "%d", gHudDisplay.lives);
			break;
		case 2: // vanília fagyi
			print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(22), HUD_TOP_Y, *((u8*)0x807FFFED) ? "\x5A" : "\x2C"); // 'Mario Head' glyph
			print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(38), HUD_TOP_Y, "*"); // 'X' glyph
			print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(54), HUD_TOP_Y, "%d", gHudDisplay.lives);
			break;
		case 3: // december build
			print_text(30+15, HUD_TOP_Y, *((u8*)0x807FFFED) ? "\x5A" : "\x2C"); // 'Mario Head' glyph
			print_text(46+15, HUD_TOP_Y, "*"); // 'X' glyph
			print_text_fmt_int(60+15, HUD_TOP_Y, "%d", gHudDisplay.lives);
			break;
	}
}

void troll_render_hud_coins() {
    switch (hudType) {
		case 0: // bé tekercs
			print_text(170, HUD_TOP_Y - 17, "+"); // 'Coin' glyph
			print_text(186, HUD_TOP_Y - 17, "*"); // 'X' glyph
			print_text_fmt_int(198, HUD_TOP_Y - 17, "%02d", gHudDisplay.coins);
			//print_text(234, HUD_TOP_Y - 17, "I");
			//print_text(242, HUD_TOP_Y - 17, "100");
			break;
		case 1: // sonkás pizza
		case 3: // december build
			print_text(170, HUD_TOP_Y - 17, "+"); // 'Coin' glyph
			print_text(186, HUD_TOP_Y - 17, "*"); // 'X' glyph
			print_text_fmt_int(200, HUD_TOP_Y - 17, "%d", gHudDisplay.coins);
			break;
		case 2: // vanília fagyi
			print_text(168, HUD_TOP_Y, "+"); // 'Coin' glyph
			print_text(184, HUD_TOP_Y, "*"); // 'X' glyph
			print_text_fmt_int(198, HUD_TOP_Y, "%d", gHudDisplay.coins);
			break;
	}
}

void troll_render_hud_stars() {
	if (gHudFlash == 1 && gGlobalTimer & 8) {
		return;
	}

	switch (hudType) {
		case 0: // bé tekercs
			print_text(170, HUD_TOP_Y, "-"); // 'Star' glyph
			print_text(186, HUD_TOP_Y, "*"); // 'X' glyph
			print_text_fmt_int(198, HUD_TOP_Y, "%02d", gHudDisplay.stars);
			break;
		case 1: // sonkás pizza
		case 3: // december build
			print_text(170, HUD_TOP_Y, "-"); // 'Star' glyph
			print_text(186, HUD_TOP_Y, "*"); // 'X' glyph
			print_text_fmt_int(200, HUD_TOP_Y, "%d", gHudDisplay.stars);
			break;
		case 2: // vanília fagyi
		{
			s8 showX = 0;

			if (gHudDisplay.stars < 100) {
				showX = 1;
			}

			print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(73), HUD_TOP_Y, "-"); // 'Star' glyph
			if (showX == 1) {
				print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(73) + 16, HUD_TOP_Y, "*"); // 'X' glyph
			}
			print_text_fmt_int((showX * 14) + GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(73 - 16),
						HUD_TOP_Y, "%d", gHudDisplay.stars);
		}	break;
	}
}





struct PowerMeterHUD {
    s8 animation;
    s16 x;
    s16 y;
    f32 unused;
};

extern u8 *sonk_start;
extern u8 *broll2_start;
extern u8 *broll1_start;
extern u8 *vanilla_start;
extern const Gfx *dl_power_meter_base;
extern const Gfx *dl_power_meter_health_segments_begin;
extern const Gfx *dl_power_meter_health_segments_end;
extern struct PowerMeterHUD sPowerMeterHUD;

/******************************************************************************************
		Damege Meter 
******************************************************************************************/

static Vtx vtx_damegemeter[] = {
	{{{ -32 , -32 ,  0},  0 , { (1 << 4),              0},   {255,255,255,220}}}, 
	{{{   0 , -32 ,  0},  0 , {(31 << 5),              0},   {255,255,255,220}}}, 
	{{{   0 ,  32 ,  0},  0 , {(31 << 5),    ( -63 << 5)},   {255,255,255,220}}}, 
	{{{ -32 ,  32 ,  0},  0 , { (1 << 4),    ( -63 << 5)},   {255,255,255,220}}}, 
	
	{{{   0 , -32 ,  0},  0 , { (1 << 4),              0},   {255,255,255,220}}}, 
	{{{  32 , -32 ,  0},  0 , {(31 << 5),              0},   {255,255,255,220}}}, 
	{{{  32 ,  32 ,  0},  0 , {(31 << 5),    ( -63 << 5)},   {255,255,255,220}}}, 
	{{{   0 ,  32 ,  0},  0 , { (1 << 4),    ( -63 << 5)},   {255,255,255,220}}}, 
};


extern Gfx RCP_damegemeter_on[] = {
	gsDPPipeSync(),
	gsSPClearGeometryMode(G_LIGHTING),
	gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_MODULATERGBA),
	gsDPSetRenderMode(G_RM_XLU_SURF,G_RM_XLU_SURF2),
	gsDPSetTextureFilter(G_TF_POINT),
	gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),
	gsSPVertex(&vtx_damegemeter[0],  8, 0), 
	gsSPEndDisplayList() 
};


extern Gfx RCP_damegemeter_txt[] = {
    gsxDPLoadTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0,
			 		G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
			 					5,6, G_TX_NOLOD, G_TX_NOLOD),
	gsSPEndDisplayList() 
};


extern Gfx RCP_damegemeter_off[] = {
	gsDPPipeSync(),
	gsSPTexture(0xffff, 0xffff, 0, 0, 0),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPSetGeometryMode(G_SHADING_SMOOTH|G_LIGHTING),
	gsSPEndDisplayList() 
};


// only does dma_read when necessary
void *powermeter_last_romstart = NULL;
void dma_read_wrapper_powermeter(void *ram, void *romstart, void *romend) {
	if (romstart != powermeter_last_romstart) {
		powermeter_last_romstart = romstart;
		dma_read(ram, romstart, romend);
	}
}

/**
 * Renders power meter health segment texture using a table list.
 */
void vanilla_render_power_meter_health_segment(s16 numHealthWedges) {
    dma_read_wrapper_powermeter(segmented_to_virtual((void*)0x030253E0), &vanilla_start + (numHealthWedges - 1) * (32*32*2) / 4, &vanilla_start + numHealthWedges * (32*32*2) / 4 - 1);

    gDPPipeSync(gDisplayListHead++);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, 0x030253E0);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES));
    gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
    gSP1Triangle(gDisplayListHead++, 0, 2, 3, 0);
}

void beta_render_power_meter_health_segment(s16 numHealthWedges) {
	u32 base = (u32)&broll1_start;
	switch (hudType) {
		case 0:
			if (personalizationRandSeed & 0x8000)
				base = (u32)&broll2_start;
			break;
		case 1:
			base = (u32)&sonk_start;
			break;
	}
    dma_read_wrapper_powermeter(segmented_to_virtual((void*)0x030273E0), (void*)(base + numHealthWedges * (64*64*2)), (void*)(base + (numHealthWedges + 1) * (64*64*2) - 1));
	
	gDPPipeSync(gDisplayListHead++);
	gSPDisplayList(gDisplayListHead++, &RCP_damegemeter_txt);
	g_Tani_LoadTextureImage2(gDisplayListHead++, 0x030273E0, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, 7);
	gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
    gSP1Triangle(gDisplayListHead++, 0, 2, 3, 0);
	gDPPipeSync(gDisplayListHead++);
	g_Tani_LoadTextureImage2(gDisplayListHead++, 0x030283E0, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, 7);
	gSP1Triangle(gDisplayListHead++, 4, 5, 6, 0);
    gSP1Triangle(gDisplayListHead++, 4, 6, 7, 0);
}

/**
 * Renders power meter display lists.
 * That includes the "POWER" base and the colored health segment textures.
 */
void troll_render_dl_power_meter(s16 numHealthWedges) {
    Mtx *mtx = alloc_display_list(sizeof(Mtx));

    if (mtx == NULL) {
        return;
    }

    guTranslate(mtx, (f32) sPowerMeterHUD.x, (f32) sPowerMeterHUD.y, 0);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx++),
              G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    
    if (hudType >= 2) { // vanilla
		gSPDisplayList(gDisplayListHead++, &dl_power_meter_base);

		if (numHealthWedges != 0) {
			gSPDisplayList(gDisplayListHead++, &dl_power_meter_health_segments_begin);
			vanilla_render_power_meter_health_segment(numHealthWedges);
			gSPDisplayList(gDisplayListHead++, &dl_power_meter_health_segments_end);
		}
	}
	else { // beta
        gSPDisplayList(gDisplayListHead++, (void*)((u32)(&RCP_damegemeter_on) & 0x00FFFFFF));
        beta_render_power_meter_health_segment(numHealthWedges);
        gSPDisplayList(gDisplayListHead++, (void*)((u32)(&RCP_damegemeter_off) & 0x00FFFFFF));
	}

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}
