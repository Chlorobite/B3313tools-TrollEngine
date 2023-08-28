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
#include "game/ingame_menu.h"
#include "game/interaction.h"
#include "game/level_geo.h"
#include "game/level_update.h"
#include "game/mario.h"
#include "game/mario_step.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/obj_behaviors_2.h"
#include "game/paintings.h"
#include "game/save_file.h"
#include "surface_terrains.h"
#include "moving_texture_macros.h"
#include "level_misc_macros.h"
#include "macro_preset_names.h"
#include "special_preset_names.h"
#include "textures.h"
#include "dialog_ids.h"
#include "seq_ids.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "segment_symbols.h"

#include "make_const_nonconst.h"

// for the OS code
#include "lib/src/libultra_internal.h"




struct Area gAreaDataNew[24];

char levelType = 0;
char goombaType = 0, bobombType = 0, ampType = 0;
f32 levelScaleH = 1.0f, levelScaleV = 1.0f;
u8 renderingLevel = FALSE;
u8 rtcHour = 0, rtcMinute = 0, rtcSecond = 0;
u8 nightTime = FALSE;
u8 nightTimeButReal = FALSE;
u8 nightMode = FALSE;
u8 nightModeSetting = 0;
u8 sunsetMode = FALSE;
u8 skyboxLoaded = FALSE;
u8 spawningLootCoins = FALSE;
u8 spawningBossStar = FALSE;
u16 personalizationRandSeed = 0;
u32 personalization_beeparams = 0;
s32 isInstantWarping = FALSE;
s32 loadedObjectCount = 0;

u8 mus_tempooverride = 0;
f32 mus_pitchmul = 1.0f;
s16 mus_transposition = 0;
u8 mus_nlstday = 0;
u8 mus_nlstnight = 0;

u8 personalizationFlags = 0;
u8 pressaaaaaaaaaaaaaaaa[] = { 0x19, 0x1B, 0x0E, 0x1C, 0x1C, 0x9E, 0x95, 0x54, 0x95, 0x54, 0x95, 0x54, 0x95, 0x54, 0x95, 0x54, 0x95, 0x54, 0x95, 0x54, 0x95, 0x54, 0xFF, 0x00 };

typedef struct RgbColor
{
	u8 r;
	u8 g;
	u8 b;
} RgbColor;

typedef struct HsvColor
{
	u8 h;
	u8 s;
	u8 v;
} HsvColor;

RgbColor HsvToRgb(HsvColor hsv)
{
    RgbColor rgb;
    u8 region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6; 

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}

HsvColor RgbToHsv(RgbColor rgb)
{
    HsvColor hsv;
    u8 rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * ((float)rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

void hueRotateRGBA5551(u16 *ptr, int size, int hue) {
	int i;
	RgbColor rgbColor;
	HsvColor hsvColor;
	u16 value;
	
	for (i = 0; i < size; i++) {
		value = *ptr;
		
		rgbColor.r = (value >> 11) << 3;
		rgbColor.g = ((value >> 6) & 0x1F) << 3;
		rgbColor.b = ((value >> 1) & 0x1F) << 3;
		
		hsvColor = RgbToHsv(rgbColor);
		hsvColor.h = (u8)(hsvColor.h + hue);
		rgbColor = HsvToRgb(hsvColor);
		
		*ptr = (
			// R
			((rgbColor.r >> 3) << 11)
			// G
			+ ((rgbColor.g >> 3) << 6)
			// B
			+ ((rgbColor.b >> 3) << 1)
			// A
			+ (value & 1)
		);
		
		ptr++;
	}
}

void rgbMultiplyRGBA16(u16 *ptr, int size, float _r, float _g, float _b) {
	int i;
	RgbColor rgbColor;
	u16 value;
	
	for (i = 0; i < size; i++) {
		value = *ptr;
		
		rgbColor.r = (value >> 11) << 3;
		rgbColor.g = ((value >> 6) & 0x1F) << 3;
		rgbColor.b = ((value >> 1) & 0x1F) << 3;
		
        rgbColor.r *= _r;
        rgbColor.g *= _g;
        rgbColor.b *= _b;
		
		*ptr = (
			// R
			((rgbColor.r >> 3) << 11)
			// G
			+ ((rgbColor.g >> 3) << 6)
			// B
			+ ((rgbColor.b >> 3) << 1)
			// A
			+ (value & 1)
		);
		
		ptr++;
	}
}

void texCopyRGBA16(u16 *ptrFrom, u16 *ptrTo, int size) {
	int i;
	
	for (i = 0; i < size; i++) {
		*ptrTo = *ptrFrom;
        
        ptrFrom++;
        ptrTo++;
	}
}


void darkenRGBA16(u16 *ptr, int size) {
	int i;
    
	for (i = 0; i < size; i++) {
        //*ptr = ((*ptr >> 1) & 0x7BDE) | (*ptr & 1);
        *ptr = (((*ptr >> 1) & 0x7BDE) + ((*ptr >> 2) & 0x39CE)) | (*ptr & 1);
        //*ptr = *ptr & 0xFFC1;
        
		ptr++;
	}
}

void darkenRGBA32(u32 *ptr, int size) {
	int i;
    
	for (i = 0; i < size; i++) {
        //*ptr = ((*ptr >> 1) & 0x7F7F7F00) | (*ptr & 0xFF);
        *ptr = (((*ptr >> 1) & 0x7F7F7F00) + ((*ptr >> 2) & 0x3F3F3F00)) | (*ptr & 0xFF);
        //*ptr = *ptr & 0xFFC1;
        
		ptr++;
	}
}

void rotateRGBA16(u16 *ptr, int size) {
	int i;
    
	for (i = 0; i < size / 2; i++) {
        u16 first = *(ptr + i);
        *(ptr + i) = *(ptr + size - 1 - i);
        *(ptr + size - 1 - i) = first;
	}
}

void rotateRGBA32(u32 *ptr, int size) {
	int i;
    
	for (i = 0; i < size / 2; i++) {
        u32 first = *(ptr + i);
        *(ptr + i) = *(ptr + size - 1 - i);
        *(ptr + size - 1 - i) = first;
	}
}

void invertRGBA16(u16 *ptr, int size, u8 lsd_texture_type) {
	int i;
    
    if (lsd_texture_type == 1) {
        for (i = 0; i < size; i++) {
            *ptr = (-(*ptr & ~1) << 1) | (*ptr & 1);
            ptr++;
        }
    }
    else {
        for (i = 0; i < size; i++) {
            // don't do the bitshift, causes weirder textures
            *ptr = -(*ptr & ~1) | (*ptr & 1);
            ptr++;
        }
    }
}

void invertRGBA32(u32 *ptr, int size, u8 lsd_texture_type) {
	int i;
    
    if (lsd_texture_type == 1) {
        for (i = 0; i < size; i++) {
            *ptr = (-(*ptr & ~0xFF) << 8) | (*ptr & 0xFF);
            ptr++;
        }
    }
    else {
        for (i = 0; i < size; i++) {
            // do a smaller bitshift, causes weirder textures
            *ptr = (-(*ptr & ~0xFF) << 7) | (*ptr & 0xFF);
            ptr++;
        }
    }
}

void normalizeRGBA16(u16 *ptr, int size) {
	int i;

    register s32 minR = 255, maxR = 0, minG = 255, maxG = 0, minB = 255, maxB = 0;
    
	for (i = 0; i < size; i++) {
        u16 val = *(ptr + i);
        s32 r = (val >> 11) & 0x1F;
        s32 g = (val >> 6) & 0x1F;
        s32 b = (val >> 1) & 0x1F;
        
        if (r < minR) minR = r;
        if (r > maxR) maxR = r;
        if (g < minB) minG = g;
        if (g > maxG) maxG = g;
        if (b < minB) minB = b;
        if (b > maxB) maxB = b;
	}
    
	for (i = 0; i < size; i++) {
        u16 val = *(ptr + i);
        s32 r = (val >> 11) & 0x1F;
        s32 g = (val >> 6) & 0x1F;
        s32 b = (val >> 1) & 0x1F;
        
        if (minR < maxR)
            r = ((r - minR) * 0x1F) / (maxR - minR);
        if (minG < maxG)
            g = ((g - minG) * 0x1F) / (maxG - minG);
        if (minB < maxB)
            b = ((b - minB) * 0x1F) / (maxB - minB);
        
        *(ptr + i) = ((r & 0x1F) << 11) | ((g & 0x1F) << 6) | ((b & 0x1F) << 1) | (val & 0x01);
	}
}

void normalizeRGBA32(u32 *ptr, int size) {
	int i;

    register s32 minR = 255, maxR = 0, minG = 255, maxG = 0, minB = 255, maxB = 0;
    
	for (i = 0; i < size; i++) {
        u32 val = *(ptr + i);
        s32 r = (val >> 24) & 0xFF;
        s32 g = (val >> 16) & 0xFF;
        s32 b = (val >> 8) & 0xFF;
        
        if (r < minR) minR = r;
        if (r > maxR) maxR = r;
        if (g < minB) minG = g;
        if (g > maxG) maxG = g;
        if (b < minB) minB = b;
        if (b > maxB) maxB = b;
	}
    
	for (i = 0; i < size; i++) {
        u32 val = *(ptr + i);
        s32 r = (val >> 24) & 0xFF;
        s32 g = (val >> 16) & 0xFF;
        s32 b = (val >> 8) & 0xFF;
        
        if (minR < maxR)
            r = ((r - minR) * 0xFF) / (maxR - minR);
        if (minG < maxG)
            g = ((g - minG) * 0xFF) / (maxG - minG);
        if (minB < maxB)
            b = ((b - minB) * 0xFF) / (maxB - minB);
        
        *(ptr + i) = ((r & 0xFF) << 24) | ((g & 0xFF) << 16) | ((b & 0xFF) << 8) | (val & 0xFF);
	}
}


//
// <RTC code>
//
extern u8 _osLastSentSiCmd;

typedef struct {
    u16 unk00;
    u8 unk02;
    u8 unk03;
} unkStruct;
typedef struct {
    u8 unk00;
    u8 unk01;
    u8 unk02;
    u8 unk03;
    u8 unk04;
    u8 unk05;
    u8 unk06;
    u8 unk07;
} unkStruct3;

typedef struct {
    u8 unk00;
    u8 unk01;
    u8 unk02;
    u8 unk03;
    unkStruct3 unk04;
} unkStruct2;

s32 __osEepStatus(OSMesgQueue *, unkStruct *);
s32 __osPackEepGetTime();

s32 osEepromGetTime(OSMesgQueue *mq, u8 *buffer) {
    s32 sp34;
    s32 sp30;
    u8 *sp2c;
    unkStruct sp28;
    unkStruct2 sp20;
    sp34 = 0;
    sp30 = 0;
    sp2c = (u8 *) &D_80365E00;
    __osSiGetAccess();
    sp34 = __osEepStatus(mq, &sp28);
    if (sp34 != 0 || sp28.unk00 != 0x8000) {

        return 8;
    }
    while (sp28.unk02 & 0x80) {
        __osEepStatus(mq, &sp28);
    }
    __osPackEepGetTime();
    sp34 = __osSiRawStartDma(OS_WRITE, &D_80365E00);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    for (sp30 = 0; sp30 < 0x10; sp30++) {
        (D_80365E00)[sp30] = 255;
    }
    D_80365E3C = 0;
    sp34 = __osSiRawStartDma(OS_READ, D_80365E00);
    _osLastSentSiCmd = 7;
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    for (sp30 = 0; sp30 < 4; sp30++) {
        sp2c++;
    }
    sp20 = *(unkStruct2 *) sp2c;
    sp34 = (sp20.unk01 & 0xc0) >> 4;
    if (sp34 == 0) {
        for (sp30 = 0; sp30 < 8; sp30++) {
            *buffer++ = ((u8 *) &sp20.unk04)[sp30];
        }
    }
    __osSiRelAccess();
    return sp34;
}

s32 __osPackEepGetTime() {
    u8 *sp14;
    unkStruct2 sp8;
    s32 sp4;
    sp14 = (u8 *) &D_80365E00;
    for (sp4 = 0; sp4 < 0x10; sp4++) {
        D_80365E00[sp4] = 255;
    }
    D_80365E3C = 1;
    sp8.unk00 = 2;
    sp8.unk01 = 9;
    sp8.unk02 = 7;
    sp8.unk03 = 2;
    for (sp4 = 0; sp4 < 8; sp4++) {
        ((u8 *) &sp8.unk04)[sp4] = 0;
    }
    for (sp4 = 0; sp4 < 4; sp4++) {
        *sp14++ = 0;
    }
    *(unkStruct2 *) sp14 = sp8;
    sp14 += 0xc;
    *sp14 = 254;
#ifdef AVOID_UB
    return 0;
#endif
}
//
// </RTC code>
//

void updateRTC() {
    u8 rtcbuffer[16];
    u8 h, m, s;
    
    rtcbuffer[2] = 0;

    if (get_red_star_count(gCurrSaveFileNum - 1) < 4) {
        h = 80;
    }
    else {
        osEepromGetTime(&gSIEventMesgQueue, rtcbuffer);
        h = rtcbuffer[2] - 0x80;
        h = ((h & 240) >> 4) * 10 + (h & 15);
        m = rtcbuffer[1];
        m = ((m & 240) >> 4) * 10 + (m & 15);
        s = rtcbuffer[0];
        s = ((s & 240) >> 4) * 10 + (s & 15);
    }

    if (h != 80) {
        rtcHour = h;
        rtcMinute = m;
        rtcSecond = s;
    }
    else {
        rtcHour = (nightModeSetting == 1 || nightModeSetting == 2) ? 0 : 12;
        rtcMinute = 0;
        rtcSecond = 0;
    }

    if (save_file_get_flags() & SAVE_FLAG_YS_POKEMON_MOON_MODE) {
        rtcHour = (rtcHour + 12) % 24;
    }

    nightTimeButReal = nightTime = rtcHour <= 4 || rtcHour >= 20;
    if (nightModeSetting >= 2) nightTime ^= TRUE;
}





u8 overrideBgTerrain = 255;
u8 overrideBgId = 255;

// TODO: add hardcoded pointers to the arrays
u32 nightSkyboxData[] = {
    TERRAIN_GRASS, 0, 0, 0, BACKGROUND_HAUNTED
};

u32 sunsetSkyboxData[] = {
    TERRAIN_GRASS, 0, 0, 0, BACKGROUND_OCEAN_SKY
};

static void interceptBgSegLoad(u8 **segStart, u8 **segEnd) {
    s32 i = 0;
    
    overrideBgTerrain = 255;
    overrideBgId = 255;
    
    skyboxLoaded = TRUE;

    updateRTC();

    sunsetMode = FALSE;
    if ((personalizationRandSeed & (3 << (gCurrLevelNum & 0xF))) == (3 << (gCurrLevelNum & 0xF))) {
        sunsetMode = TRUE;
    }
    
    if (nightTime) {
        nightMode = TRUE;
        
        /*for (i = 0; i < 0; i += 5) {
            if (*segStart == (u8*)nightSkyboxData[i + 1]) {
                *segStart = (u8*)nightSkyboxData[i + 2];
                *segEnd = (u8*)nightSkyboxData[i + 3];
                overrideBgTerrain = (u8)nightSkyboxData[i + 0];
                overrideBgId = (u8)nightSkyboxData[i + 4];
                return;
            }
        }*/
    }
    else if (sunsetMode) {
        for (i = 0; i < 5; i += 5) {
            if (*segStart == (u8*)sunsetSkyboxData[i + 1]) {
                *segStart = (u8*)sunsetSkyboxData[i + 2];
                *segEnd = (u8*)sunsetSkyboxData[i + 3];
                overrideBgTerrain = (u8)sunsetSkyboxData[i + 0];
                overrideBgId = (u8)sunsetSkyboxData[i + 4];
                return;
            }
        }
    }
}

void interceptSegLoad(u8 segId, u8 **segStart, u8 **segEnd) {
    if (segId == 0x0A) {
        interceptBgSegLoad(segStart, segEnd);
    }
}



u32 random_u32() {
    return (u32)(((u32)random_u16() << 16) | random_u16());
}

void call_a_random_ass_function() {
    typedef void func(s32 a, s32 b, s32 c, s32 d);
    const register u32 JR_RA = 0x03E00008;
    
    s32 funcCount = 0;
    u32 *funcptr = (u32*)0x80246000;
    
    for (; (u32)funcptr < 0x8033A580; funcptr++) {
        if ((u32)(*funcptr) == JR_RA) {
            funcCount++;
        }
    }
    
    funcCount = (s32)(random_float() * funcCount);
    funcptr = (u32*)0x80246000;
    
    for (; (u32)funcptr < 0x8033A580; funcptr++) {
        if ((u32)(*funcptr) == JR_RA) {
            if (--funcCount <= 0) {
                u32 arg1 = random_u32() ^ (u32)osGetTime();
                u32 arg2 = random_u32() ^ (u32)osGetTime();
                u32 arg3 = random_u32() ^ (u32)osGetTime();
                u32 arg4 = random_u32() ^ (u32)osGetTime();

                funcptr += 2;

                // mario...
                // majority of these functions take struct MarioState * as the first argument
                // so pass mario
                if ((u32)funcptr >= 0x80250940 && (u32)funcptr <= 0x80275FE0) {
                    arg1 = (u32)gMarioState;
                }

                ((func*)funcptr)(arg1, arg2, arg3, arg4);
                break;
            }
        }
    }
}

s32 coin_troll_hitbox(struct Object *o) {
	switch (o->oBehParams >> 24) {
		case 1: // green
			o->oDamageOrCoinValue = -3; // restores 3 HP and 0 coin
			break;
		case 2: // orange
			o->oDamageOrCoinValue = -2; // restores 2 HP and 1 coin
			break;
		case 3: // purple
			o->oDamageOrCoinValue = -1; // damages mario and removes a coin lol
			break;
		case 4: // white
            // they don't give any coins, behavior is below
			o->oDamageOrCoinValue = 0;
			break;
		case 5: // clear
			// check if vanish cap
            if (gMarioState->flags & MARIO_VANISH_CAP) {
                o->oDamageOrCoinValue = 5; // blue coin
                // can collide
                o->hitboxRadius = 100;
            }
            else {
                // can't collide
                o->hitboxRadius = -1000;
            }
			break;
		case 6: // pink
			o->oDamageOrCoinValue = 10; // restores 10 HP and 10 coins
			break;
		default:
			// defaults work here
			break;
	}
	
    if (o->oInteractStatus & INT_STATUS_INTERACTED
        && !(o->oInteractStatus & INT_STATUS_TOUCHED_BOB_OMB)) {
        if ((o->oBehParams >> 24) == 4) {
            /*if (random_u16() < 0x3313) {
                // CRASHMA GAME
                *((u32*)0) = 0;
            }
            else*/ {
                // boing
                play_sound(SOUND_GENERAL_BOING2, gGlobalSoundSource);
                call_a_random_ass_function();
            }
        }
        else {
            spawn_object(o, MODEL_SPARKLES, bhvGoldenCoinSparkles);
        }
        
        obj_mark_for_deletion(o);
        return TRUE;
    }

    o->oInteractStatus = 0;

    return FALSE;
}

u8 get_coin_bparam(struct Object *o, u8 type) {
    if (random_u16() < 20 + get_red_star_count(gCurrSaveFileNum - 1)) {
        return 4; // white
    }

    if (PERSONALIZATION_FLAG_DISABLE_RETEXTURE) {
        // unpersonalize the coin even if it's personalized
        return 0;
    }
    
    if ((o->oBehParams & 0xFF000000) != 0 || get_red_star_count(gCurrSaveFileNum - 1) < 6) {
        // coin already personalized
        return o->oBehParams >> 24;
    }
    
    switch (type) {
        case 0: // static
            break;
        case 1: // coin formation
            if (TRACKER_hp < 4.0f && (f32)(gMarioState->health >> 8) < 4.0f) {
                return 1; // green
            }
            break;
        case 2: // objects with physics (enemy drop, [!] drop)
            if (spawningLootCoins) {
                if (TRACKER_difficulty_modifier < 0.7f) {
                    return 2; // orange
                }
            }
            
            if ((gGlobalTimer % 256) < (TRACKER_difficulty_modifier - 2.0f) * 33) {
                return 3; // purple
            }
            break;
    }
    
    return 0; // yellow
}

void personalize_coin(struct Object *o, u8 type) {
    if ((u32)o->header.gfx.sharedChild == (u32)gLoadedGraphNodes[MODEL_YELLOW_COIN]) {
        o->oBehParams |= get_coin_bparam(o, type) << 24;
    }
}

// the part where numCoins and healCounter are usually incremented
void personalized_interact_coin(struct MarioState *m, struct Object *o) {
    switch (o->oDamageOrCoinValue) {
        case -3: // green coins
            m->healCounter += 4 * 3;
            break;
        case -2: // orange coins
            m->numCoins += 1;
            m->healCounter += 4 * 2;
            break;
        default:
            m->numCoins += o->oDamageOrCoinValue;
            if (o->oDamageOrCoinValue > 0) {
                m->healCounter += 4 * o->oDamageOrCoinValue;
            }
            else if (o->oDamageOrCoinValue < 0) {
                m->hurtCounter += -4 * o->oDamageOrCoinValue;
                
                if ((m->hurtCounter - m->healCounter) > (m->health >> 6) - 4) {
                    // death by troll
                    TRACKER_accum_deaths_trolled += 1.0f;
                }
            }
            break;
    }
}


extern s16 sInvulnerable;
u32 lava_goomba_funny_burn_mario(register struct Object *_o, register struct MarioState *m) {
    u32 burningAction = ACT_BURNING_JUMP;
    
    if (_o->oDistanceToMario > 200.0f) {
        return FALSE;
    }

    if (levelType == 6 && !sInvulnerable && !(m->flags & MARIO_METAL_CAP) && !(m->flags & MARIO_VANISH_CAP)) {
#if ENABLE_RUMBLE
        queue_rumble_data(5, 80);
#endif
        if ((m->action & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER))
            || m->waterLevel - m->pos[1] > 50.0f) {
            play_sound(SOUND_GENERAL_FLAME_OUT, m->marioObj->header.gfx.cameraToObject);
        } else {
            m->marioObj->oMarioBurnTimer = 0;
            update_mario_sound_and_camera(m);
            play_sound(SOUND_MARIO_ON_FIRE, m->marioObj->header.gfx.cameraToObject);

            if ((m->action & ACT_FLAG_AIR) && m->vel[1] <= 0.0f) {
                burningAction = ACT_BURNING_FALL;
            }

            return drop_and_set_mario_action(m, burningAction, 1);
        }
    }

    return FALSE;
}


u8 sGoombaImmuneAttackHandlers[6] = {
    /* ATTACK_PUNCH:                 */ ATTACK_HANDLER_SPECIAL_HUGE_GOOMBA_WEAKLY_ATTACKED,
    /* ATTACK_KICK_OR_TRIP:          */ ATTACK_HANDLER_SPECIAL_HUGE_GOOMBA_WEAKLY_ATTACKED,
    /* ATTACK_FROM_ABOVE:            */ ATTACK_HANDLER_SPECIAL_HUGE_GOOMBA_WEAKLY_ATTACKED,
    /* ATTACK_GROUND_POUND_OR_TWIRL: */ ATTACK_HANDLER_SPECIAL_HUGE_GOOMBA_WEAKLY_ATTACKED,
    /* ATTACK_FAST_ATTACK:           */ ATTACK_HANDLER_SPECIAL_HUGE_GOOMBA_WEAKLY_ATTACKED,
    /* ATTACK_FROM_BELOW:            */ ATTACK_HANDLER_SPECIAL_HUGE_GOOMBA_WEAKLY_ATTACKED,
};

void obj_scale_personalized(struct Object *obj) {
    obj->header.gfx.scale[0] *= levelScaleH;
    obj->header.gfx.scale[1] *= levelScaleV;
    obj->header.gfx.scale[2] *= levelScaleH;
}



u32 modelLoadedBits[8];

s32 get_model_loaded(u8 model) {
    return modelLoadedBits[model >> 5] & (1 << (model & 0x1F));
}

void set_model_loaded(u8 model, s32 isLoaded) {
    if (isLoaded) {
        modelLoadedBits[model >> 5] |= (1 << (model & 0x1F));
    }
    else {
        modelLoadedBits[model >> 5] &= ~(1 << (model & 0x1F));
    }
}

void clear_model_loaded() {
    s32 i;
    
    for (i = 0; i < 8; i++) {
        modelLoadedBits[i] = 0;
    }
}


void on_collected_star(struct Object *starObj) {
    struct Object *obj;
	struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(segmented_to_virtual(bhvCapSwitch))];
	u8 bparams = starObj->oBehParams & 0xFF;
    
	if ((bparams & 0xE0) == 0) {
        if ((u32)listHead >= 0x80000000) {
            obj = (struct Object *) listHead->next;
            
            while (obj != (struct Object *) listHead) {
                if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && obj->behavior == segmented_to_virtual(bhvCapSwitch)) {
                    *(&TRACKER_accum_stars_prefer_wing_cap + obj->oBehParams2ndByte) += 1.0f;
                }
                
                obj = (struct Object *) obj->header.next;
            }
        }
	}
	else {
        if (bparams & 0x20) {
            TRACKER_accum_stars_prefer_wing_cap += 1.0f;
        }
        if (bparams & 0x40) {
            TRACKER_accum_stars_prefer_vanish_cap += 1.0f;
        }
        if (bparams & 0x80) {
            TRACKER_accum_stars_prefer_metal_cap += 1.0f;
        }
    }
    
    if (bparams & 0x01) {
        TRACKER_accum_stars_prefer_cannon += 1.0f;
    }
    if (bparams & 0x02) {
        TRACKER_accum_stars_prefer_free += 1.0f;
    }
    if (bparams & 0x04) {
        TRACKER_accum_stars_prefer_murder += 1.0f;
    }
    if (bparams & 0x08) {
        TRACKER_accum_stars_prefer_platform += 1.0f;
    }
    if (bparams & 0x10) {
        TRACKER_accum_stars_prefer_slide += 1.0f;
    }
    
    if (starObj->oBooTurningSpeed & 0x01) {
        TRACKER_accum_stars_prefer_boss += 1.0f;
    }
    if (starObj->oBooTurningSpeed & 0x02) {
        TRACKER_accum_stars_prefer_collect += 1.0f;
    }
    if (starObj->oBooTurningSpeed & 0x04) {
        TRACKER_accum_stars_prefer_timed += 1.0f;
    }
    
    TRACKER_accum_stars += 1.0f;
    
    *(&TRACKER_accum_stars_prefer_buildings + levelType) += 1.0f;
}

void AI_star_set_platform_bparam_if_above_average_y(struct Object *starObj) {
    s32 i;
    struct ObjectNode *listHead;
	struct Object *obj;
	f32 minY = 32767.0f, maxY = -32768.0f, avgY;
    
	for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
		listHead = &gObjectLists[i];

		obj = (struct Object *) listHead->next;

		while (obj != (struct Object *) listHead) {
            if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && (u32)obj->behavior >= 0x80000000) {
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
	
	avgY = (minY + maxY) / 2.0f;
    
    if (starObj->oPosY > avgY) {
        // platform
        starObj->oBehParams |= 0x08;
    }
}

void AI_star_set_bparams(struct Object *starObj) {
    if (spawningBossStar) {
        starObj->oBooTurningSpeed |= 0x01;
        spawningBossStar = FALSE;
    }
    
    if (starObj->behavior == segmented_to_virtual(bhvSpawnedStarNoLevelExit)) {
        if ((starObj->oBehParams >> 24) == 6) {
            // collect (100 coin star)
            starObj->oBooTurningSpeed |= 0x02;
        }
        else {
            // free (toad or mips star)
            starObj->oBehParams |= 0x02;
        }
    }
    
    if (gCurrentArea->terrainType == TERRAIN_SLIDE) {
        // slide
        starObj->oBehParams |= 0x10;
    }
    else if (levelType == 7) {
        // platform
        starObj->oBehParams |= 0x08;
    }
    
    if (gHudDisplay.flags & HUD_DISPLAY_FLAG_TIMER) {
        // timed star
        starObj->oBooTurningSpeed |= 0x04;
    }
}


f32 AI_star_get_preference_by_bparams(struct Object *starObj) {
	u8 bparams = starObj->oBehParams & 0xFF;
    f32 result = 1.0f;
    
    if (bparams & 0x01) {
        result *= TRACKER_accum_stars_prefer_cannon;
    }
    if (bparams & 0x02) {
        result *= TRACKER_accum_stars_prefer_free;
    }
    if (bparams & 0x04) {
        result *= TRACKER_accum_stars_prefer_murder;
    }
    if (bparams & 0x08) {
        result *= TRACKER_accum_stars_prefer_platform;
    }
    if (bparams & 0x10) {
        result *= TRACKER_accum_stars_prefer_slide;
    }
    if (bparams & 0x20) {
        result *= TRACKER_accum_stars_prefer_wing_cap;
    }
    if (bparams & 0x40) {
        result *= TRACKER_accum_stars_prefer_vanish_cap;
    }
    if (bparams & 0x80) {
        result *= TRACKER_accum_stars_prefer_metal_cap;
    }
    
    if (starObj->oBooTurningSpeed & 0x01) {
        result *= TRACKER_accum_stars_prefer_boss;
    }
    if (starObj->oBooTurningSpeed & 0x02) {
        result *= TRACKER_accum_stars_prefer_collect;
    }
    if (starObj->oBooTurningSpeed & 0x04) {
        result *= TRACKER_accum_stars_prefer_timed;
    }
    
    return result;
}


extern s16 gMatStackIndex;
extern Mat4 gMatStack[32];
extern Mtx *gMatStackFixed[32];

void _geo_append_display_list(void *displayList, s32 layer);
void scaled_geo_append_display_list(void *displayList, s32 layer) {
    if (gCurGraphNodeMasterList != 0) {
        Vec3f scaleVec;
        Mtx *mtx;
        
        if (!renderingLevel) {
            _geo_append_display_list(displayList, layer);
            return;
        }
        
        mtx = alloc_display_list(sizeof(*mtx));
        vec3f_set(scaleVec, levelScaleH, levelScaleV, levelScaleH);
        mtxf_scale_vec3f(gMatStack[gMatStackIndex + 1], gMatStack[gMatStackIndex], scaleVec);
        gMatStackIndex++;
        mtxf_to_mtx(mtx, gMatStack[gMatStackIndex]);
        gMatStackFixed[gMatStackIndex] = mtx;
        _geo_append_display_list(displayList, layer);
        gMatStackIndex--;
    }
}

void troll_load_environmental_regions(register s16 **data) {
    register s32 numRegions;
    register s32 i;

    gEnvironmentRegions = *data;
    numRegions = *(*data)++;

    for (i = 0; i < numRegions; i++) {
        *data += 5;
        gEnvironmentLevels[i] = (*(*data)++) * levelScaleV;
    }
}

void vec3f_scaled_set(register Vec3f dest, register f32 x, register f32 y, register f32 z) {
    dest[0] = x * levelScaleH;
    dest[1] = y * levelScaleV;
    dest[2] = z * levelScaleH;
}

void scale_instant_warp(register struct InstantWarp *warp) {
    warp->displacement[0] *= levelScaleH;
    warp->displacement[1] *= levelScaleV;
    warp->displacement[2] *= levelScaleH;
}

struct GraphNodeBackground *troll_init_graph_node_background(struct AllocOnlyPool *pool,
                                                       struct GraphNodeBackground *graphNode,
                                                       u16 background, GraphNodeFunc backgroundFunc) {
    if (backgroundFunc != NULL && overrideBgId != 255) {
        background = overrideBgId;
    }
    
    return init_graph_node_background(pool, graphNode, background, backgroundFunc, 0);
}

u32 troll_knockback(register struct MarioState *m, register struct Object *o, register u32 damage) {
    // Lava bob-ombs cause lava boost instead of knockback
	if (levelType == 6 && o->behavior == segmented_to_virtual(bhvExplosion)) {
        return drop_and_set_mario_action(m, ACT_LAVA_BOOST, 1);
    }
    return drop_and_set_mario_action(m, determine_knockback_action(m, o->oDamageOrCoinValue), damage);
}

void *dynamic_dma_read(u8 *srcStart, u8 *srcEnd, u32 side);
void *troll_dynamic_dma_read_and_set_segment(u8 *srcStart, u8 *srcEnd, u32 side, register s32 segment) {
    void *addr = dynamic_dma_read(srcStart, srcEnd, side);

    if (addr != NULL) {
        set_segment_base_addr(segment, addr);
    }
    
    return addr;
}

void on_death_barrier_warp() {
    TRACKER_accum_deaths_death_barrier += 1.0f; // death by death barrier
    TRACKER_accum_deaths += 1.0f; // never gets updated by mario's state so update as well
}

void weird_function(void *ptr);
void spawn_boss_star(register void *weirdptr) {
    TRACKER_accum_murder += 1.0f;
    spawningBossStar = TRUE;
    
    weird_function(weirdptr);
}



u32 troll_bowser_actions_bitdw_not_reacting(register struct Object *_o, register f32 rand, register f32 diff_div_10) {
    if (_o->oBowserStatus & BOWSER_STATUS_ANGLE_MARIO) {
        if (_o->oDistanceToMario < 1500.0f) { // nearby
            if (rand < diff_div_10 - 0.15f) {
                return BOWSER_ACT_SPIT_FIRE_INTO_SKY;
            }
            else {
                return BOWSER_ACT_BREATH_FIRE;
            }
        } else { // far away
            return BOWSER_ACT_QUICK_JUMP;
        }
    } else {
        // Keep walking
        if (rand < diff_div_10 - 0.1f) {
            return BOWSER_ACT_CHARGE_MARIO;
        }
    }
    
    return BOWSER_ACT_WALK_TO_MARIO;
}

u32 troll_bowser_actions_bitfs_not_reacting(register struct Object *_o, register f32 rand, register f32 diff_div_10) {
    if (_o->oBowserStatus & BOWSER_STATUS_ANGLE_MARIO) {
        if (_o->oDistanceToMario < 1300.0f) {  // nearby
            if (rand < diff_div_10 - 0.2f) {
                return BOWSER_ACT_BIG_JUMP;
            }
            else if (rand < 0.5) { // 50% chance
                return BOWSER_ACT_TELEPORT;
            } else {
                rand = random_float();
                if (rand < diff_div_10 - 0.15f) {
                    return BOWSER_ACT_SPIT_FIRE_INTO_SKY;
                }
                
                return BOWSER_ACT_SPIT_FIRE_ONTO_FLOOR;
            }
        } else { // far away
            if (500.0f < _o->oBowserDistToCenter && _o->oBowserDistToCenter < 1500.0f
                && rand < 0.5) { // 50% chance
                return BOWSER_ACT_BIG_JUMP;
            }
            return BOWSER_ACT_CHARGE_MARIO;
        }
    } else {
        // Keep walking
        if (rand < diff_div_10 - 0.2f) {
            if (_o->oDistanceToMario < 1300.0f) {
                return BOWSER_ACT_TELEPORT;
            }
            
            return BOWSER_ACT_CHARGE_MARIO;
        }
        else if (rand < diff_div_10 - 0.1f) {
            return BOWSER_ACT_BIG_JUMP;
        }
    }
    
    return BOWSER_ACT_WALK_TO_MARIO;
}

u32 troll_bowser_actions_bits_angle_mario(register struct Object *_o, register f32 rand, register f32 diff_div_10) {
    if (_o->oDistanceToMario < 1000.0f) { // nearby
        if (rand < TRACKER_difficulty_modifier * 0.4f) {
            return BOWSER_ACT_SPIT_FIRE_INTO_SKY; // 40% chance
        } else if (rand < 0.8f) {
            return BOWSER_ACT_SPIT_FIRE_ONTO_FLOOR; // 80% chance
        }
        return BOWSER_ACT_BREATH_FIRE;
    } else { // far away
        if (rand < 0.5f) {
            if (rand < diff_div_10 - 0.2f) {
                return BOWSER_ACT_TELEPORT;
            }
            return BOWSER_ACT_CHARGE_MARIO;
        }
        return BOWSER_ACT_BIG_JUMP; // 50% chance
    }
    
    return BOWSER_ACT_WALK_TO_MARIO;
}

u32 troll_bowser_actions_bits_reacting(register struct Object *_o, register f32 rand, register f32 diff_div_10) {
    // Keep walking
    if (rand < diff_div_10 - 0.2f) {
        return BOWSER_ACT_TELEPORT;
    }
    else if (rand < diff_div_10 - 0.1f) {
        return BOWSER_ACT_BIG_JUMP;
    }
    
    return BOWSER_ACT_WALK_TO_MARIO;
}

s32 troll_bowser_check_hit_mine() {
    f32 dist;
    register struct Object *mine = cur_obj_find_nearest_object_with_behavior(bhvBowserBomb, &dist);

    if (mine != NULL && dist < 800.0f) {
        TRACKER_boss_performance += 1.0f; // bonk
        mine->oInteractStatus |= INT_STATUS_HIT_MINE;
        return TRUE;
    }

    return FALSE;
}

s32 troll_bowser_nyoom(register struct Object *_o) {
    if (cur_obj_check_if_near_animation_end()) {
        return TRUE;
    }
    
    if (TRACKER_difficulty_modifier > 2.0f) {
        _o->oForwardVel *= 1.5f;
        _o->header.gfx.animInfo.animFrame++;
    }
    
    return FALSE;
}

void troll_mark_goomba_as_dead() {
    register struct Object *_o = gCurrentObject;
    register struct Object *_o_parentObj = _o->parentObj;

	// direct contact with lava goomba burns mario after killing
    lava_goomba_funny_burn_mario(_o, gMarioState);

    if (_o_parentObj != _o) {
        register s32 trol = (_o->oBehParams2ndByte & GOOMBA_BP_TRIPLET_FLAG_MASK);

        set_object_respawn_info_bits(_o_parentObj, trol >> 2);

        _o_parentObj->oBehParams |= trol << 6;
    }
}

void troll_goomba_wander(register struct Object *_o) {
    _o->oGoombaRelativeSpeed = (goombaType == 2 || goombaType == 3) ? 2.0f : (4.0f / 3.0f);

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

#include "game/save_file.h"
extern s32 sCapSaveFlags[3];
extern struct SaveBuffer gSaveBuffer;
void troll_yellow_switch_or_set_flags(s32 __oBehParams2ndByte) {
    u16 *fuck;
    s32 i;
    
    if (__oBehParams2ndByte < 3) {
        save_file_set_flags(sCapSaveFlags[__oBehParams2ndByte]);
        return;
    }
    
    // YLELOW SWTICH WYELOLWS SWCH YLEOW SWITCH
    
    // completely randomize the save flags
    save_file_clear_flags(0xFFFFFFFF);
    save_file_set_flags(((random_u16() << 16) | random_u16()) ^ (u32)osGetTime());
    
    // fuck the AI too
    fuck = (u16*)&gSaveBuffer.menuData.aiData;
    
    for (i = 0; i < sizeof(gSaveBuffer.menuData.aiData); i += 2) {
        *fuck = random_u16();
        fuck++;
    }
    
    // enjoy this garbage lol
    TRACKER_read_save();
    TRACKER_boss_performance = 0; // massive difficulty spikes are caused by this value, so 'fix it'
}

u8 rand_dialog_id = 0;
void randomize_dialog_id() {
    do {
        rand_dialog_id = (u8)random_u16();
    }
    while (rand_dialog_id > 169);
}

s32 troll_cap_switch_dialog(s32 __oBehParams2ndByte) {
    if (__oBehParams2ndByte < 3)
        return cur_obj_update_dialog_with_cutscene(MARIO_DIALOG_LOOK_FRONT, 
            (DIALOG_FLAG_TEXT_RESPONSE | DIALOG_FLAG_UNK_CAPSWITCH), CUTSCENE_CAP_SWITCH_PRESS, 0);
    
    return cur_obj_update_dialog(MARIO_DIALOG_LOOK_FRONT, (DIALOG_FLAG_TEXT_RESPONSE | DIALOG_FLAG_UNK_CAPSWITCH), rand_dialog_id, 0);
}

void corrupt_entrypoint(u32 a0);
extern s32 corruptTimer;
void fuck_textures() {
    randomize_dialog_id();
    corruptTimer = 6;
    corrupt_entrypoint(0xf2000000 + (random_u16() << 16));
}

s32 troll_perform_ground_step(register struct MarioState *m) {
    register s32 i;
    register u32 stepResult;
    Vec3f intendedPos;
    register s32 anim = m->marioObj->header.gfx.animInfo.animID;

    for (i = 0; i < 4; i++) {
        register f32 velX = m->vel[0];
        register f32 velZ = m->vel[2];
        
        if (m->wall != NULL && (anim == MARIO_ANIM_PUSHING || anim == MARIO_ANIM_SIDESTEP_LEFT || anim == MARIO_ANIM_SIDESTEP_RIGHT)) {
            register f32 mag = sqrtf(velX * velX + velZ * velZ);
            if (mag > 6.f) {
                velX /= mag / 6.f;
                velZ /= mag / 6.f;
            }
        }
        
        intendedPos[0] = m->pos[0] + m->floor->normal.y * (velX / 4.0f);
        intendedPos[2] = m->pos[2] + m->floor->normal.y * (velZ / 4.0f);
        intendedPos[1] = m->pos[1];

        stepResult = perform_ground_quarter_step(m, intendedPos);
        if (stepResult == GROUND_STEP_LEFT_GROUND || stepResult == GROUND_STEP_HIT_WALL_STOP_QSTEPS) {
            break;
        }
    }

    m->terrainSoundAddend = mario_get_terrain_sound_addend(m);
    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);

    if (stepResult == GROUND_STEP_HIT_WALL_CONTINUE_QSTEPS) {
        return GROUND_STEP_HIT_WALL;
    }
    return stepResult;
}

void troll_change_area(register s32 index) {
    register s32 areaFlags = gCurrentArea->flags;

    if (gCurrAreaIndex != index) {
        unload_area();
        load_area(index);

        gCurrentArea->flags = areaFlags;
        gMarioObject->oActiveParticleFlags = 0;
        
		// ADD: (change_area) on instant warp, reset BGM and play painting sound
        set_background_music(gCurrentArea->musicParam, gCurrentArea->musicParam2, 0);
        play_sound(SOUND_GENERAL_PAINTING_EJECT, gGlobalSoundSource);
		// END ADD
    }

    if (areaFlags & 0x01) {
        gMarioObject->header.gfx.areaIndex = gMarioSpawnInfo->areaIndex = index;
    }
}

extern f32 g_trampolineSinkDegree;
extern struct Object *g_currentTrampoline;
f32 troll_get_additive_y_vel_for_jumps(void) {
    if (g_currentTrampoline)
        return 1.0f * g_trampolineSinkDegree;
    return 0.0f;
}

void troll_initiate_warp(s16 destLevel, s16 destArea, s16 destWarpNode, s32 arg3) {
    if (destLevel == 0x10) { // CG
        if (get_red_star_count(gCurrSaveFileNum - 1) == 0)
            destArea = 4;
        if (destArea == 4 && gCurrentArea->index != 8 && (gCurrentArea->index == 4 || (random_u16() & 1)))
            destArea = 8;
    }
    
    initiate_warp(destLevel, destArea, destWarpNode, arg3);
}

struct Object *funnyMarioDupe = NULL;
struct Object *funnyMarioDupeUseDoor = NULL;
s32 funnyMarioSwapma = TRUE;
void troll_door_animation_and_reset(s32 sp18) {
	s32 starCount = save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1);
    struct Object *otherDoor;
    
    
    cur_obj_init_animation_with_sound(sp18);
    if (cur_obj_check_if_near_animation_end()) {
        gCurrentObject->oAction = 0;
        
        if (funnyMarioDupe != NULL) {
            funnyMarioDupe->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // unload
            funnyMarioDupe = NULL;
        }
    }
    
    if (!(gCurrentObject->oBehParams & 0x8000) && random_u16() >= starCount) {
        return;
    }
    funnyMarioSwapma = (gCurrentObject->oBehParams & 0x0100) || (!(gCurrentObject->oBehParams & 0x8000) && (random_u16() & 1));
    
    otherDoor = gCurrentObject;
    otherDoor--;
    if (otherDoor->behavior != gCurrentObject->behavior || otherDoor->oPosY != gCurrentObject->oPosY) {
        otherDoor += 2;
        if (otherDoor->behavior != gCurrentObject->behavior || otherDoor->oPosY != gCurrentObject->oPosY) {
            otherDoor = NULL;
        }
    }
    
    if (otherDoor != NULL) {
        struct Object *tolleybus = gCurrentObject;
        
        if (gCurrentObject->oTimer == 0) {// && gCurrentObject->oBehParams & 0x80) {
            struct SpawnInfo spawnInfo;
            register u32 *spawnBhv = segmented_to_virtual((void*)0x1F002C00);
            
            spawnInfo.startPos[0] = (gCurrentObject->oPosX + otherDoor->oPosX) * 0.5f;
            spawnInfo.startPos[1] = (gCurrentObject->oPosY + otherDoor->oPosY) * 0.5f;
            spawnInfo.startPos[2] = (gCurrentObject->oPosZ + otherDoor->oPosZ) * 0.5f;
            
            // spawn a mario
            if (funnyMarioDupe == NULL) {
                funnyMarioDupe = (struct Object*)create_object(spawnBhv);

                // Behavior parameters are often treated as four separate bytes, but
                // are stored as an s32.
                funnyMarioDupe->oBehParams = 0x05000000 | (funnyMarioSwapma ? 0x20000000 : 0);
                // The second byte of the behavior parameters is copied over to a special field
                // as it is the most frequently used by objects.
                funnyMarioDupe->oBehParams2ndByte = ((funnyMarioDupe->oBehParams) >> 16) & 0xFF;

                funnyMarioDupe->behavior = spawnBhv;
                funnyMarioDupe->unused1 = 0;

                // Record death/collection in the SpawnInfo
                funnyMarioDupe->respawnInfoType = RESPAWN_INFO_TYPE_32;
                funnyMarioDupe->respawnInfo = &funnyMarioDupe->oBehParams;

                spawnInfo.startAngle[0] = 0.0f;
                spawnInfo.startAngle[1] = 0.0f;
                spawnInfo.startAngle[2] = 0.0f;

                spawnInfo.areaIndex = gCurrentArea->index;
                spawnInfo.activeAreaIndex = gCurrentArea->index;
                spawnInfo.model = gLoadedGraphNodes[funnyMarioSwapma ? ((*((u8*)0x807FFFED)) ? 1 : 2) : 0];

                geo_obj_init_spawninfo(&funnyMarioDupe->header.gfx, &spawnInfo);

                funnyMarioDupe->oPosX = spawnInfo.startPos[0];
                funnyMarioDupe->oPosY = spawnInfo.startPos[1];
                funnyMarioDupe->oPosZ = spawnInfo.startPos[2];
            }
            
            funnyMarioDupeUseDoor = otherDoor;
            
            otherDoor->oAction = gCurrentObject->oAction;
            otherDoor->oPrevAction = gCurrentObject->oAction;
            otherDoor->oSubAction = 0;
            otherDoor->oTimer = 1;
            gCurrentObject = otherDoor;
            cur_obj_init_animation_with_sound(sp18);
            gCurrentObject = tolleybus;
        }
    }
}

extern struct PlayerCameraState *sMarioCamState;
void troll_swap_mario() {
    if (funnyMarioDupe != NULL) {
        register struct GraphNodeObject *mirrorMario = (struct GraphNodeObject *)(&funnyMarioDupe->header);
        void (*tol) ();
        
        gMarioState->usedObj = funnyMarioDupeUseDoor;
        sMarioCamState->pos[0] =       mirrorMario->pos[0];
        sMarioCamState->pos[1] =       mirrorMario->pos[1];
        sMarioCamState->pos[2] =       mirrorMario->pos[2];
        sMarioCamState->faceAngle[1] += 0x8000;
        
        // swapma
        if (funnyMarioSwapma) {
            tol = (void*)0x8029ADA8;
            tol();
        }
    }
}

void troll_print_generic_string(s16 x, s16 y, u8 *str) {
    if (str == (u8*)0x802DB134 && PERSONALIZATION_FLAG_TROLL_FAST_TRAVEL) {
        str = pressaaaaaaaaaaaaaaaa;
    }
    print_generic_string(x, y, str);
}



enum ToadMessageStates {
    TOAD_MESSAGE_FADED,
    TOAD_MESSAGE_OPAQUE,
    TOAD_MESSAGE_OPACIFYING,
    TOAD_MESSAGE_FADING,
    TOAD_MESSAGE_TALKING
};


void troll_toad_message_talking() {
    register s32 savema = gCurrentObject->oBehParams & 0x200;
    register s32 response = cur_obj_update_dialog(MARIO_DIALOG_LOOK_DOWN, 
            savema ? (DIALOG_FLAG_TURN_TO_MARIO | DIALOG_FLAG_TEXT_RESPONSE) : (DIALOG_FLAG_TURN_TO_MARIO | DIALOG_FLAG_TEXT_DEFAULT),
            gCurrentObject->oToadMessageDialogId, 0);

    if (response) {
		register s32 dialogIdPostStar = (gCurrentObject->oBehParams >> 16) & 0xFF;
		register s32 starId = (gCurrentObject->oBehParams & 0x1FF);
		register s32 spawnStarId = starId;
		register s32 courseId = COURSE_NUM_TO_INDEX(gCurrCourseNum);

		if (starId >= 16) {
			courseId = COURSE_NUM_TO_INDEX(COURSE_MIN) + ((starId - 16) >> 3);
			starId &= 7;
		}

        gCurrentObject->oToadMessageRecentlyTalked = TRUE;
        gCurrentObject->oToadMessageState = TOAD_MESSAGE_FADING;

        if (savema && response == DIALOG_RESPONSE_YES) {
            save_file_do_save(gCurrSaveFileNum - 1);
        }

		if (dialogIdPostStar != 0 && !(save_file_get_star_flags(gCurrSaveFileNum - 1, courseId) & (1 << starId))) {
            gCurrentObject->oToadMessageDialogId = dialogIdPostStar;
            bhv_spawn_star_no_level_exit(spawnStarId);
		}
    }
}

s32 troll_cur_obj_update_dialog(s32 actionArg, s32 dialogFlags, s32 dialogID) {
    register struct Object *o = gCurrentObject;
    s32 dialogResponse = DIALOG_RESPONSE_NONE;
    s32 doneTurning = TRUE;

    switch (o->oDialogState) {
		// EDIT: restore JP behavior (beta)
        case DIALOG_STATUS_ENABLE_TIME_STOP:
            //! We enable time stop even if Mario is not ready to speak. This
            //  allows us to move during time stop as long as Mario never enters
            //  an action that can be interrupted with text.
            if (gMarioState->health >= 0x100) {
                gTimeStopState |= TIME_STOP_ENABLED;
                o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
                o->oDialogState++;
            }
            break;
		// END EDIT

        case DIALOG_STATUS_INTERRUPT:
            // ADD: Handle flag that makes the NPC rotate towards to Mario
            if (dialogFlags & DIALOG_FLAG_TURN_TO_MARIO) {
                doneTurning = cur_obj_rotate_yaw_toward(obj_angle_to_object(o, gMarioObject), 0x800);
                // Failsafe just in case it takes more than 33 frames somehow
				// IT'S B33 AGAIN THEY LITERALLY JUST MADE IT AGAIN
                if (o->oDialogResponse >= 33) {
                    doneTurning = TRUE;
                }
            }
			// END ADD
			
            // Interrupt status until Mario is actually speaking with the NPC and if the
            // object is done turning to Mario
            if (set_mario_npc_dialog(actionArg) == MARIO_DIALOG_STATUS_SPEAK && doneTurning) {
                o->oDialogState++;
            }
            break;

        case DIALOG_STATUS_START_DIALOG:
            // Starts dialog, depending of the flag defined, it calls
            // a default dialog or a dialog with response.
            if (dialogFlags & DIALOG_FLAG_TEXT_RESPONSE) {
                create_dialog_box_with_response(dialogID);
            } else if (dialogFlags & DIALOG_FLAG_TEXT_DEFAULT) {
                create_dialog_box(dialogID);
            }
            o->oDialogState++;
            break;

        case DIALOG_STATUS_STOP_DIALOG:
            // Stops dialog, if the flag dialog response was called
            // then it defines the value to let the object do the rest.
            if (dialogFlags & DIALOG_FLAG_TEXT_RESPONSE) {
                if (gDialogResponse != DIALOG_RESPONSE_NONE) {
                    cur_obj_end_dialog(dialogFlags, gDialogResponse);
                }
            } else if (dialogFlags & DIALOG_FLAG_TEXT_DEFAULT) {
                if (get_dialog_id() == DIALOG_NONE) {
                    cur_obj_end_dialog(dialogFlags, DIALOG_RESPONSE_NOT_DEFINED);
                }
            } else {
                cur_obj_end_dialog(dialogFlags, DIALOG_RESPONSE_NOT_DEFINED);
            }
            break;

        case DIALOG_STATUS_DISABLE_TIME_STOP:
            // We disable time stop for a few seconds when Mario is no longer
            // speaking or the flag is defined, then we enable it again.
            // Usually, an object disables time stop using a separate function
            // after a certain condition is met.
            if (gMarioState->action != ACT_READING_NPC_DIALOG || (dialogFlags & DIALOG_FLAG_TIME_STOP_ENABLED)) {
                gTimeStopState &= ~TIME_STOP_ENABLED;
                o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
                dialogResponse = o->oDialogResponse;
                o->oDialogState = DIALOG_STATUS_ENABLE_TIME_STOP;
            }
            break;

        default:
            o->oDialogState = DIALOG_STATUS_ENABLE_TIME_STOP;
            break;
    }

    return dialogResponse;
}

extern const BehaviorScript *bhvStarMagnet;
struct Object *troll_spawn_star(struct Object *sp30, f32 sp34, f32 sp38, f32 sp3C) {
    register f32 starMagnetDist = 33130.f;
	register s32 i;
	register struct ObjectNode *listHead;
	register struct Object *obj;
    register struct Object *o = gCurrentObject;
    
    sp30 = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStarSpawnCoordinates, o->oPosX, o->oPosY,
                                     o->oPosZ, 0, 0, 0);
    sp30->oBehParams = o->oBehParams;

    sp30->oHomeX = sp34 * levelScaleH;
    sp30->oHomeY = sp38 * levelScaleV;
    sp30->oHomeZ = sp3C * levelScaleH;

    // Find closest star magnet, if any.
	for (i = OBJ_LIST_PLAYER; i < NUM_OBJ_LISTS; i++) {
		listHead = &gObjectLists[i];

		obj = (struct Object *) listHead->next;

		while (obj != (struct Object *) listHead) {
            if (!(obj->activeFlags & ACTIVE_FLAG_DEACTIVATED) && obj->behavior == segmented_to_virtual(&bhvStarMagnet)) {
                register f32 dist = dist_between_objects(o, obj);

                if (dist < starMagnetDist) {
                    starMagnetDist = dist;
                    sp30->oHomeX = obj->oPosX;
                    sp30->oHomeY = obj->oPosY;
                    sp30->oHomeZ = obj->oPosZ;

                    sp30->oBehParams = obj->oBehParams;
                    sp30->oBehParams2ndByte = obj->oBehParams2ndByte;
                }
            }
			obj = (struct Object *) obj->header.next;
        }
	}

    sp30->oFaceAnglePitch = 0;
    sp30->oFaceAngleRoll = 0;
    return sp30;
}

void troll_spawn_bowser_star() {
    register struct Object *obj;
    
    if (((gCurrentObject->oBehParams) & 0x01FF) >= 0x01FE)
        // key
        obj = spawn_object(gCurrentObject, MODEL_BOWSER_KEY, bhvBowserKey);
    else
        // star
        obj = spawn_object_relative(0, 0, 200.0f, 200.0f, gCurrentObject, MODEL_STAR, bhvStar);
    
    obj->oBehParams |= ((gCurrentObject->oBehParams) & 0x0100) | ((gCurrentObject->oBehParams & 0xFF) << 24);
    obj_spawn_loot_yellow_coins(gCurrentObject, 20, 10.0f);
}

extern const BehaviorScript bhvSilverStar[];
extern const BehaviorScript bhvHiddenSilverStarStar[];
struct Object *silverStarCeleb = NULL;
s32 isSilverStar = FALSE;
void troll_general_star_dance_handler(struct MarioState *m, s32 isInWater) {
    register s32 dialogID;
    isSilverStar |= m->interactObj->behavior == segmented_to_virtual(bhvSilverStar);
    
    if (m->actionState == 0) {
        switch (++m->actionTimer) {
            case 1:
                silverStarCeleb = spawn_object(m->marioObj, isSilverStar ? 0x97 : MODEL_STAR, bhvCelebrationStar);
                disable_background_sound();
                if (isSilverStar) {
                    play_music(SEQ_PLAYER_ENV, SEQUENCE_ARGS(15, SEQ_EVENT_CUTSCENE_COLLECT_KEY), 0);
                    m->actionArg |= 2;
                }
                else if (m->actionArg & 1) {
                    play_course_clear();
                }
                else {
                    play_music(SEQ_PLAYER_ENV, SEQUENCE_ARGS(15, SEQ_EVENT_CUTSCENE_COLLECT_STAR), 0);
                }
                break;

            case 42:
                if (isSilverStar) {
                    m->actionState = 2;
                }
                else {
                    play_sound(SOUND_MARIO_HERE_WE_GO, m->marioObj->header.gfx.cameraToObject);
                }
                break;

            case 80:
                if (!(m->actionArg & 1)) {
                    level_trigger_warp(m, WARP_OP_STAR_EXIT);
                } else {
                    enable_time_stop();
                    create_dialog_box_with_response(gLastCompletedStarNum == 7 ? DIALOG_013 : DIALOG_014);
                    m->actionState = 1;
                }
                break;
        }
    } else if (m->actionState == 1 && gDialogResponse != DIALOG_RESPONSE_NONE) {
        if (gDialogResponse == DIALOG_RESPONSE_YES) {
            save_file_do_save(gCurrSaveFileNum - 1);
        }
        m->actionState = 2;
    } else if (m->actionState == 2 && is_anim_at_end(m)) {
        disable_time_stop();
        enable_background_sound();
        set_mario_action(m, isInWater ? ACT_WATER_IDLE : ACT_IDLE, 0);
        seq_player_fade_out(SEQ_PLAYER_ENV, 10);
        
        if (isSilverStar && silverStarCeleb != NULL && (silverStarCeleb->activeFlags & ACTIVE_FLAG_ACTIVE)) {
            uintptr_t *behaviorAddr = segmented_to_virtual(bhvHiddenSilverStarStar);
            struct Object *obj;
            // iterate through the list respective to the behavior, much faster than checking all objects
            struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(segmented_to_virtual(bhvHiddenSilverStarStar))];
            
            play_sound(SOUND_MENU_STAR_SOUND, m->marioObj->header.gfx.cameraToObject);
            silverStarCeleb->activeFlags &= ~ACTIVE_FLAG_ACTIVE; // unload
            
            // Find silver star spawner
            if ((u32)listHead >= 0x80000000) {
                obj = (struct Object *) listHead->next;
                
                while (obj != (struct Object *) listHead) {
                    if ((obj->activeFlags & ACTIVE_FLAG_ACTIVE) && obj->behavior == behaviorAddr) {
                        if (++obj->oHiddenStarTriggerCounter != 5) {
                            gCurrentObject = gMarioObject;
                            spawn_orange_number(obj->oHiddenStarTriggerCounter, 0, 0, 0);
                        }
                    }
                    
                    obj = (struct Object *) obj->header.next;
                }
            }
            
            isSilverStar = FALSE;
        }
    }
}

void toyal_bhv_castle_floor_trap_init(void) {
    register struct Object *sp2C;
    register f32 toyal = 358.f * levelScaleH;
    
    spawn_object_relative(0, -toyal, 0, 0, gCurrentObject, MODEL_CASTLE_BOWSER_TRAP, bhvFloorTrapInCastle);
    sp2C = spawn_object_relative(0, toyal, 0, 0, gCurrentObject, MODEL_CASTLE_BOWSER_TRAP, bhvFloorTrapInCastle);
    sp2C->oMoveAngleYaw += 0x8000;
}

void troll_setup() {
    s32 i;
    
    for (i = 1; i < 256; i++) {
        gLoadedGraphNodes[i] = (void*)(0x80003313 | (i << 16));
    }
}

void troll_check_instant_warp(void) {
    s16 cameraAngle;
    struct Surface *floor;

    if (get_red_star_count(gCurrSaveFileNum - 1) == 0)
        return;

    if ((floor = gMarioState->floor) != NULL) {
        s32 index = floor->type - SURFACE_INSTANT_WARP_1B;
        if (index >= INSTANT_WARP_INDEX_START && index < INSTANT_WARP_INDEX_STOP
            && gCurrentArea->instantWarps != NULL) {
            struct InstantWarp *warp = &gCurrentArea->instantWarps[index];

            if (warp->id != 0) {
                gMarioState->pos[0] += warp->displacement[0];
                gMarioState->pos[1] += warp->displacement[1];
                gMarioState->pos[2] += warp->displacement[2];

                gMarioState->marioObj->oPosX = gMarioState->pos[0];
                gMarioState->marioObj->oPosY = gMarioState->pos[1];
                gMarioState->marioObj->oPosZ = gMarioState->pos[2];

                cameraAngle = gMarioState->area->camera->yaw;

                isInstantWarping = TRUE;
                change_area(warp->area);
                isInstantWarping = FALSE;
                gMarioState->area = gCurrentArea;

                warp_camera(warp->displacement[0], warp->displacement[1], warp->displacement[2]);

                gMarioState->area->camera->yaw = cameraAngle;
            }
        }
    }
}

void set_mario_y_vel_based_on_fspeed(struct MarioState *m, f32 initialVelY, f32 multiplier);

void triple_jump_set_mario_y_vel_based_on_fspeed(struct MarioState *m, f32 initialVelY, f32 multiplier) {
    if (get_red_star_count(gCurrSaveFileNum - 1) < 2)
        initialVelY = 62.f;
    set_mario_y_vel_based_on_fspeed(m, initialVelY, multiplier);
}
void sideflip_set_mario_y_vel_based_on_fspeed(struct MarioState *m, f32 initialVelY, f32 multiplier) {
    if (get_red_star_count(gCurrSaveFileNum - 1) == 0)
        initialVelY = 48.f;
    set_mario_y_vel_based_on_fspeed(m, initialVelY, multiplier);
}

s32 troll_act_crouch_slide(struct MarioState *m) {
    s32 cancel;

    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BUTT_SLIDE, 0);
    }

    if (get_red_star_count(gCurrSaveFileNum - 1) >= 2) {
        if (m->actionTimer < 30) {
            m->actionTimer++;
            if (m->input & INPUT_A_PRESSED) {
                if (m->forwardVel > 10.0f) {
                    return set_jumping_action(m, ACT_LONG_JUMP, 0);
                }
            }
        }

        if (get_red_star_count(gCurrSaveFileNum - 1) >= 3) {
            if (m->input & INPUT_B_PRESSED) {
                return set_mario_action(m, ACT_SQUATKICK, 9);
            }
        }
    }

    if (m->input & INPUT_A_PRESSED) {
        return set_jumping_action(m, ACT_JUMP, 0);
    }

    if (m->input & INPUT_FIRST_PERSON) {
        return set_mario_action(m, ACT_BRAKING, 0);
    }

    cancel = common_slide_action_with_jump(m, ACT_CROUCHING, ACT_JUMP, ACT_FREEFALL,
                                           MARIO_ANIM_START_CROUCHING);
    return cancel;
}

s32 troll_act_crouching(struct MarioState *m) {
    if (m->input & INPUT_STOMPED) {
        return set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

    if (m->input & INPUT_A_PRESSED) {
        return set_jumping_action(m, get_red_star_count(gCurrSaveFileNum - 1) >= (2 + ((personalizationRandSeed & 8) ? 1 : 0)) ? ACT_BACKFLIP : ACT_JUMP, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }

    if (m->input & INPUT_FIRST_PERSON) {
        return set_mario_action(m, ACT_STOP_CROUCHING, 0);
    }

    if (!(m->input & INPUT_Z_DOWN)) {
        return set_mario_action(m, ACT_STOP_CROUCHING, 0);
    }

    if (get_red_star_count(gCurrSaveFileNum - 1) >= 2) {
        if (m->input & INPUT_NONZERO_ANALOG) {
            return set_mario_action(m, ACT_START_CRAWLING, 0);
        }

        if (get_red_star_count(gCurrSaveFileNum - 1) >= 3) {
            if (m->input & INPUT_B_PRESSED) {
                return set_mario_action(m, ACT_PUNCHING, 9);
            }
        }
    }

    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_CROUCHING);
    return FALSE;
}

s32 troll_act_start_crouching(struct MarioState *m) {
    if (m->input & INPUT_STOMPED) {
        return set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_A_PRESSED) {
        return set_jumping_action(m, get_red_star_count(gCurrSaveFileNum - 1) >= (2 + ((personalizationRandSeed & 8) ? 1 : 0)) ? ACT_BACKFLIP : ACT_JUMP, 0);
    }

    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }

    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_START_CROUCHING);
    if (is_anim_past_end(m)) {
        set_mario_action(m, ACT_CROUCHING, 0);
    }
    return FALSE;
}

s32 troll_act_stop_crouching(struct MarioState *m) {
    if (m->input & INPUT_STOMPED) {
        return set_mario_action(m, ACT_SHOCKWAVE_BOUNCE, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, ACT_FREEFALL, 0);
    }

    if (m->input & INPUT_A_PRESSED) {
        return set_jumping_action(m, get_red_star_count(gCurrSaveFileNum - 1) >= (2 + ((personalizationRandSeed & 8) ? 1 : 0)) ? ACT_BACKFLIP : ACT_JUMP, 0);
    }

    if (m->input & INPUT_ABOVE_SLIDE) {
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);
    }

    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_STOP_CROUCHING);
    if (is_anim_past_end(m)) {
        set_mario_action(m, ACT_IDLE, 0);
    }
    return FALSE;
}


s32 can_pass_through_walls() {
    // If an object can pass through a vanish cap wall, pass through.
    if (gCurrentObject != NULL) {
        // If an object can pass through a vanish cap wall, pass through.
        if (gCurrentObject->activeFlags & ACTIVE_FLAG_MOVE_THROUGH_GRATE) return 1;

        // If Mario has a vanish cap, pass through the vanish cap wall.
        if (gCurrentObject == gMarioObject && (gMarioState->flags & MARIO_VANISH_CAP)) return 1;
    }

    return 0;
}


// We kept running low on heap memory, causing the most random crashes
// We use the 0x3500 bytes of unused space after Motos now, unless we actually have more space in heap somehow!!
#define ALIGN4(val) (((val) + 0x3) & ~0x3)
struct AllocOnlyPool *troll_render_pool_init() {
    void *addr = (void*)0x8041CB00;
    struct AllocOnlyPool *subPool = NULL;

    if (main_pool_available() > 0x3500) {
        subPool = alloc_only_pool_init(main_pool_available() - sizeof(struct AllocOnlyPool),
                                                MEMORY_POOL_LEFT);
        main_pool_free(subPool);
    }
    else {
        subPool = (struct AllocOnlyPool *) addr;
        subPool->totalSpace = ALIGN4(0x3500 - sizeof(struct AllocOnlyPool));
        subPool->usedSpace = 0;
        subPool->startPtr = (u8 *) addr + sizeof(struct AllocOnlyPool);
        subPool->freePtr = (u8 *) addr + sizeof(struct AllocOnlyPool);
    }

    return subPool;
}


// A separate load_patchable_table function to allow more animations to be defined in the Mario directory
extern u32 mario_patchable_table_TWO[];
s32 mario_anim_load_patchable_table(struct DmaHandlerList *list, s32 index) {
    s32 ret = FALSE;
    struct DmaTable *table = list->dmaTable;
    u8 *addr;
    s32 size;

    if ((u32)index < table->count) {
        addr = table->srcAddr + table->anim[index].offset;
        size = table->anim[index].size;

        if (list->currentAddr != addr) {
            dma_read(list->bufTarget, addr, addr + size);
            list->currentAddr = addr;
            ret = TRUE;
        }
    }
    else {
        // Out of bounds, oh no !
        // We must now use the other array lol
        index -= table->count;

        addr = (u8*)mario_patchable_table_TWO[index * 2];
        size = mario_patchable_table_TWO[index * 2 + 1];

        if (size > 0 && list->currentAddr != addr) {
            dma_read(list->bufTarget, addr, addr + size);
            list->currentAddr = addr;
            ret = TRUE;
        }
    }
    return ret;
}




// Climbma
// original code by iProgramInCpp (2021)
// adapted by a benedani for bee (2023)
// trolls to get this function to be used are in Mario directory
f32 prevControlStickY = 0;
s32 act_climbing_wall(struct MarioState *m) {
	s16 wallDYaw, ang, xm, ym;
	if (m->input & INPUT_A_PRESSED) {
		m->vel[1] = 52.0f;
		m->faceAngle[1] += 0x8000;
		return set_mario_action(m, ACT_WALL_KICK_AIR, 0);
	}
	
	if (m->wall == NULL || m->wall->type != SURFACE_HANGABLE) {
        mario_set_forward_vel(m, 0.0f);
        return set_mario_action(m, ACT_FREEFALL, 0);
    }
    
    m->marioObj->header.gfx.animInfo.animAccel = 0;
    wallDYaw = atan2s(m->wall->normal.z, m->wall->normal.x);
    m->faceAngle[1] = 0X8000 + wallDYaw;
    m->marioObj->header.gfx.angle[1] = 0X8000 + wallDYaw;
    mario_set_forward_vel(m, 10.0f);
    
    if (m->marioObj->header.gfx.animInfo.animID < MARIO_ANIM_CLIMBING ||
        m->marioObj->header.gfx.animInfo.animID > MARIO_ANIM_CLIMBING_RIGHT) {
        set_mario_animation(m, MARIO_ANIM_CLIMBING);
        m->marioObj->header.gfx.animInfo.animAccel = 0;
        m->marioObj->header.gfx.animInfo.animFrame = 0;
    }
    
    if (m->controller->stickY == 0) {
        set_mario_animation(m, MARIO_ANIM_CLIMBING_DOWN);
        //set_mario_anim_with_accel(m, MARIO_ANIM_CLIMBING_DOWN,  0x10000 * m->vel[1] * 12);
        m->marioObj->header.gfx.animInfo.animFrame = 10;
    }
    if (m->input & INPUT_NONZERO_ANALOG) {
        //stick X - left/right
        //stick Y - up/down
        if (m->controller->stickY > 16.f || m->controller->stickY < -16.f) {
            m->vel[1] = m->controller->stickY * .25f;
            // set anim speed
            if (m->controller->stickY > 16.f) {
                set_mario_animation(m, MARIO_ANIM_CLIMBING);
                m->vel[1] *= .50f;
                m->marioObj->header.gfx.animInfo.animAccel = 0x40000;
            } else {
                m->vel[1] *= 1.0f;
                set_mario_animation(m, MARIO_ANIM_CLIMBING_DOWN);
                m->marioObj->header.gfx.animInfo.animFrame = 0;
                m->particleFlags |= PARTICLE_DUST;
                play_sound(SOUND_MOVING_TERRAIN_SLIDE, m->marioObj->header.gfx.cameraToObject);
                
            }
        } else { 
            m->particleFlags &= ~PARTICLE_DUST;
            m->vel[1] = 0; 
            if (m->marioObj->header.gfx.animInfo.animID != MARIO_ANIM_CLIMBING_DOWN) {
                set_mario_animation(m, MARIO_ANIM_CLIMBING_DOWN);
                m->marioObj->header.gfx.animInfo.animFrame = 10;
            }
        }
        if (m->controller->stickX > 16.f || m->controller->stickX < -16.f) {
            //m->vel[1] = m->controller->stickY * .5f;
            ang = m->faceAngle[1];
            // Calculate the X and Y would-be movements based on the angle
            if (m->controller->stickX > 16.f) {
                // going right
                ang -= 0x2000;
                xm = (s16)(8.f * sins(ang));
                ym = (s16)(8.f * coss(ang));
                // X and Y movement
                m->vel[0] = xm;
                m->vel[2] = ym;
                
                set_mario_animation(m, MARIO_ANIM_CLIMBING_RIGHT);
                m->marioObj->header.gfx.animInfo.animAccel = 0x5000 * 12;
            } else {
                // going left
                ang += 0x2000;
                xm = (s16)(8.f * sins(ang));
                ym = (s16)(8.f * coss(ang));
                // X and Y movement
                m->vel[0] = xm;
                m->vel[2] = ym;
                set_mario_animation(m, MARIO_ANIM_CLIMBING_LEFT);
                m->marioObj->header.gfx.animInfo.animAccel = 0x5000 * 12;
            }
        } else { 
            if (m->marioObj->header.gfx.animInfo.animID == MARIO_ANIM_CLIMBING) 
                ;//m->marioObj->header.gfx.animInfo.animAccel = 0x10000;
            else
                m->marioObj->header.gfx.animInfo.animAccel = 0;
            m->vel[0] = 0; 
            m->vel[2] = 0; 
        }
    } else { 
        m->vel[1] = 0; 
        m->vel[0] = 0;
        m->vel[2] = 0;
    }
    
    switch (perform_air_step(m, 0)) {
        case AIR_STEP_LANDED:
            mario_set_forward_vel(m, 0.0f);
            if (!check_fall_damage_or_get_stuck(m, ACT_HARD_BACKWARD_GROUND_KB)) {
                return set_mario_action(m, ACT_FREEFALL_LAND, 0);
            }
            break;
    }
    ang = m->faceAngle[1];
    xm = (s16)(50.f * sins(ang));
    ym = (s16)(50.f * coss(ang));
    m->marioObj->header.gfx.pos[0] += xm;
    m->marioObj->header.gfx.pos[2] += ym;
	
	//! this doesn't actually work according to 2021 me
	if (m->marioObj->header.gfx.animInfo.animFrame < 0) m->marioObj->header.gfx.animInfo.animFrame = 0;
	
	prevControlStickY = m->controller->stickY;
	return FALSE;
}

// called by common_air_action_step
s32 set_mario_animation_then_check_for_climbable_wall(struct MarioState *m, s32 animation) {
    set_mario_animation(m, animation);

    if (m->wall) {
        if (m->wall->type == SURFACE_HANGABLE) {
            s16 wallDYaw = atan2s(m->wall->normal.z, m->wall->normal.x);
            m->faceAngle[1] = wallDYaw;
            return set_mario_action(m, ACT_CLIMBING_WALL, 0);
        }
    }
    
    return 0;
}


s32 act_electric_idle(struct MarioState *m) {
    play_sound_if_no_flag(m, SOUND_MARIO_ATTACKED, MARIO_ACTION_SOUND_PLAYED);
    play_sound(SOUND_MOVING_SHOCKED, m->marioObj->header.gfx.cameraToObject);
    set_camera_shake_from_hit(SHAKE_SHOCK);
    if (m->actionTimer == 0 || set_mario_animation(m, MARIO_ANIM_SHOCKED) == 0) {
        m->actionTimer++;
        m->flags |= MARIO_METAL_SHOCK;
    }

    if (m->actionTimer >= 4) {
        m->invincTimer = 30;
        set_mario_action(m, m->health < 0x0100 ? ACT_ELECTROCUTION : ACT_IDLE, 0);
    }
    stationary_ground_step(m);
    return FALSE;
}

s32 act_squatkick(struct MarioState *m) {
	// m->actionState should be zero by default

	set_mario_animation (m, MARIO_ANIM_SQUATKICKING);

	if (m->actionState == 0) 
	{
		//mario_set_forward_vel(m, 20.0f);
		if (m->actionTimer == 0)
		{
			m->forwardVel += 25.0f;
			m->actionTimer = 1;
		}
		m->vel[1] += 8.0f;
		if (m->marioObj->header.gfx.animInfo.animFrame >= 2)
			perform_air_step(m, 0);
		if (m->marioObj->header.gfx.animInfo.animFrame >= 3)
            m->actionState++;
		play_mario_sound(m, SOUND_ACTION_TERRAIN_JUMP, 0);
	}
	else 
	{
		switch (perform_air_step(m, 0))
		{
			case AIR_STEP_HIT_LAVA_WALL:
				lava_boost_on_wall(m);
			case AIR_STEP_HIT_WALL:
				//mario_set_forward_vel(m, 0.0f);
				// we should bonk
                mario_set_forward_vel(m, -8.0f);
                return set_mario_action(m, ACT_SOFT_BONK, 0);
				
				break;
				
			case AIR_STEP_NONE: 
				if (m->actionState == 1)
				{
					m->flags |= MARIO_KICKING;
					update_air_without_turn(m);
					if (is_anim_past_end(m)) m->actionState++;
				}
				else if (m->actionState == 2)
				{
					update_air_without_turn(m);
				}
				break;
			case AIR_STEP_LANDED: 
				set_mario_action(m, ACT_BUTT_SLIDE, 0);
				play_mario_landing_sound(m, SOUND_ACTION_TERRAIN_LANDING);
				break;
		}
	}
	return 0;
}


// for red coins, silver star etc
struct Object *troll_spawn_star_bypass_magnets(struct Object *star, f32 posX, f32 posY, f32 posZ) {
    register struct Object *o = gCurrentObject;

    star = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStarSpawnCoordinates, o->oPosX, o->oPosY,
                                     o->oPosZ, 0, 0, 0);
    star->oBehParams = o->oBehParams;

    star->oHomeX = posX;
    star->oHomeY = posY;
    star->oHomeZ = posZ;

    star->oFaceAnglePitch = 0;
    star->oFaceAngleRoll = 0;
    return star;
}

s32 is_object_star_spawner(struct Object *obj) {
    return (obj->behavior == segmented_to_virtual(bhvHiddenRedCoinStar) ||
        obj->behavior == segmented_to_virtual(bhvBowserCourseRedCoinStar) ||
		obj->behavior == segmented_to_virtual(bhvHiddenStar) || // secret
		obj->behavior == segmented_to_virtual(bhvHiddenSilverStarStar));
}



extern s16 sInvulnerable;
extern u8 sDisplayingDoorText;



u32 troll_interact_door(register struct MarioState *m, UNUSED u32 interactType, register struct Object *o) {
    register s16 requiredNumStars = o->oBehParams >> 24;
	// EDIT: check red stars if the required number of stars is 13 every time
    register s16 numStars =
        (o->oBehParams & 0x0200) ? save_file_get_total_betakey_count(gCurrSaveFileNum - 1) : (
            requiredNumStars == 13 ? get_red_star_count(gCurrSaveFileNum - 1) :
            save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1)
    );
	// END EDIT

    if (m->action == ACT_WALKING || m->action == ACT_DECELERATING) {
        register s32 doorOpen = numStars >= requiredNumStars;
        if (o->oBehParams & 0x0400) {
            doorOpen = save_file_get_betakey_flags(gCurrSaveFileNum - 1) & (1 << requiredNumStars);
        }

        if (doorOpen) {
            u32 actionArg = should_push_or_pull_door(m, o);
            u32 enterDoorAction;
            u32 doorSaveFileFlag;

            if (actionArg & 0x00000001) {
                enterDoorAction = ACT_PULLING_DOOR;
            } else {
                enterDoorAction = ACT_PUSHING_DOOR;
            }

            doorSaveFileFlag = get_door_save_file_flag(o);
            m->interactObj = o;
            m->usedObj = o;

            if (o->oInteractionSubtype & INT_SUBTYPE_STAR_DOOR) {
                enterDoorAction = ACT_ENTERING_STAR_DOOR;
            }

            return set_mario_action(m, enterDoorAction, actionArg);
        } else if (!sDisplayingDoorText) {
            u32 text = DIALOG_022 << 16;

            // TODO: Use other dialog for key doors?
            if (!(o->oBehParams & 0x0600)) {
                switch (requiredNumStars) {
                    case 1:
                        text = DIALOG_024 << 16;
                        break;
                    case 3:
                        text = DIALOG_025 << 16;
                        break;
                    case 8:
                        text = DIALOG_026 << 16;
                        break;
                    case 30:
                        text = DIALOG_027 << 16;
                        break;
                    case 50:
                        text = DIALOG_028 << 16;
                        break;
                    case 13: // EDIT: 70 star doors are unused, so replace with 13
                        text = DIALOG_029 << 16;
                        break;
                }
            }

            text += requiredNumStars - numStars;

            sDisplayingDoorText = TRUE;
            return set_mario_action(m, ACT_READING_AUTOMATIC_DIALOG, text);
        }
    }
	// REMOVE: special case where the 70 star door lets you in anyway, as they are unused

    return FALSE;
}


void troll_chain_chomp_released_jump_away(void) {
    if (gCurrentObject->oMoveFlags & OBJ_MOVE_MASK_ON_GROUND || gCurrentObject->oPosY < gCurrentObject->oHomeY - 1000.f) {
        gObjCutsceneDone = TRUE;
        gCurrentObject->oChainChompReleaseStatus = CHAIN_CHOMP_RELEASED_END_CUTSCENE;
    }
}

s32 is_pointer_valid(void *ptr) {
    return ((u32)ptr & 0xFF800003) == 0x80000000;
}


