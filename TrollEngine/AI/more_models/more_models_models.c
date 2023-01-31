#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"


/********************************************************************************
	RCP_HmsItemStar [ New New Version ]
														[ Nob 19, 1995 ]
 ********************************************************************************/

#define		STAR_POLYGON_R		255
#define		STAR_POLYGON_G		255
#define		STAR_POLYGON_B		 46
#define		STAR_POLYGON_A		255
#define	gsxDPLoadTextureImage(fmt, siz, width, height,		\
		pal, cms, cmt, masks, maskt, shifts, shiftt)		\
									\
	gsDPSetTile(fmt, siz, 0, 0, G_TX_LOADTILE, 0 , cmt, maskt,	\
		shiftt, cms, masks, shifts),				\
	gsDPLoadSync(),							\
	gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (width)*(height)-1,		\
				 	CALC_DXT(width, siz##_BYTES)),					\
	gsDPSetTile(fmt, siz, ((((width) * siz##_LINE_BYTES)+7)>>3), 0,	\
		G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks,	\
		shifts),						\
	gsDPSetTileSize(G_TX_RENDERTILE, 0, 0,				\
		((width)-1) << G_TEXTURE_IMAGE_FRAC,			\
		((height)-1) << G_TEXTURE_IMAGE_FRAC)

/********************************************************************************/
/*	Vertex data [common].														*/
/********************************************************************************/
static const Vtx vtx_star[] = {
	{{{ -64,   0, 0},  0,	   {0<<5, 31<<5},		{STAR_POLYGON_R, STAR_POLYGON_G, STAR_POLYGON_B, STAR_POLYGON_A 	}}},
	{{{  64,	 0, 0},  0,	  {31<<5, 31<<5},		{STAR_POLYGON_R, STAR_POLYGON_G, STAR_POLYGON_B, STAR_POLYGON_A	}}},
	{{{  64, 128, 0},  0,	  {31<<5,  0<<5},		{STAR_POLYGON_R, STAR_POLYGON_G, STAR_POLYGON_B, STAR_POLYGON_A	}}},
	{{{ -64, 128, 0},  0,	   {0<<5,  0<<5},		{STAR_POLYGON_R, STAR_POLYGON_G, STAR_POLYGON_B, STAR_POLYGON_A	}}},
};

/********************************************************************************/
/*	Texture data of star coin.													*/
/********************************************************************************/
// where's the source material for the white stars?
// uhh idk nintendo forced us to delete them lol hf
// (protip: they're literally just the white stars from the title screen)
static const u8 star_coin1_txt[] = {
#include "white_star_0.rgba16.inc.c"
};
static const u8 star_coin2_txt[] = {
#include "white_star_1.rgba16.inc.c"
};
static const u8 star_coin3_txt[] = {
#include "white_star_2.rgba16.inc.c"
};
static const u8 star_coin4_txt[] = {
#include "white_star_3.rgba16.inc.c"
};
static const u8 star_coin5_txt[] = {
#include "white_star_4.rgba16.inc.c"
};
static const u8 star_coin6_txt[] = {
#include "white_star_5.rgba16.inc.c"
};
static const u8 star_coin7_txt[] = {
#include "white_star_6.rgba16.inc.c"
};
static const u8 star_coin8_txt[] = {
#include "white_star_7.rgba16.inc.c"
};


/********************************************************************************/
/*	Graphic display list of star coin.											*/
/********************************************************************************/
static const Gfx gfx_star[] = {
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),

    gsxDPLoadTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
			 G_TX_CLAMP | G_TX_NOMIRROR, G_TX_CLAMP | G_TX_NOMIRROR,
			 5, 5, G_TX_NOLOD, G_TX_NOLOD),

	gsSPVertex(&vtx_star[0], 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),

    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPEndDisplayList()
};

/*------------------------------------------------------------------------------*/

Gfx RCP_sstar1[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin1_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_sstar2[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin2_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_sstar3[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin3_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_sstar4[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin4_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_sstar5[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin5_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_sstar6[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin6_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_sstar7[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin7_txt),
	gsSPBranchList(gfx_star)
};
Gfx RCP_sstar8[] = {
	gsDPPipeSync(),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, star_coin8_txt),
	gsSPBranchList(gfx_star)
};




//
// Doors
//

extern const Lights1 door_seg3_lights_03009CF8;

static const u8 custom_door_1_top_txt[] = {
#include "custom_door_1_top_txt.rgba16.inc.c"
};
static const u8 custom_door_1_side_txt[] = {
#include "custom_door_1_side_txt.rgba16.inc.c"
};
static const u8 custom_door_2_top_txt[] = {
#include "custom_door_2_top_txt.rgba16.inc.c"
};
static const u8 custom_door_2_side_txt[] = {
#include "custom_door_2_side_txt.rgba16.inc.c"
};
static const u8 custom_door_3_top_txt[] = {
#include "custom_door_3_top_txt.rgba16.inc.c"
};
static const u8 custom_door_3_side_txt[] = {
#include "custom_door_3_side_txt.rgba16.inc.c"
};
static const u8 custom_door_4_top_txt[] = {
#include "custom_door_4_top_txt.rgba16.inc.c"
};
static const u8 custom_door_4_side_txt[] = {
#include "custom_door_4_side_txt.rgba16.inc.c"
};




const Gfx custom_door_tex_setup[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};


const Gfx custom_door_1_dl_near[] = {
    gsSPDisplayList(custom_door_tex_setup),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, custom_door_1_top_txt),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPDisplayList(0x030149C0), // top and bottom, left and right
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (64 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, custom_door_1_side_txt),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 64 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPDisplayList(0x03014A20), // front and back
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPDisplayList(0x007FE380), // knobs
    gsSPEndDisplayList(),
};

const Gfx custom_door_1_dl_far[] = {
    gsSPDisplayList(custom_door_tex_setup),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (64 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, custom_door_1_side_txt),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 64 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPDisplayList(0x03014EF0), // front and back
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPLight(&door_seg3_lights_03009CF8.l, 1),
    gsSPLight(&door_seg3_lights_03009CF8.a, 2),
    gsSPDisplayList(0x007FE380), // knobs
    gsSPEndDisplayList(),
};


const Gfx custom_door_2_dl_near[] = {
    gsSPDisplayList(custom_door_tex_setup),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, custom_door_2_top_txt),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPDisplayList(0x030149C0), // top and bottom, left and right
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (64 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, custom_door_2_side_txt),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 64 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPDisplayList(0x03014A20), // front and back
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPDisplayList(0x007FE380), // knobs
    gsSPEndDisplayList(),
};

const Gfx custom_door_2_dl_far[] = {
    gsSPDisplayList(custom_door_tex_setup),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (64 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, custom_door_2_side_txt),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 64 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPDisplayList(0x03014EF0), // front and back
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPLight(&door_seg3_lights_03009CF8.l, 1),
    gsSPLight(&door_seg3_lights_03009CF8.a, 2),
    gsSPDisplayList(0x007FE380), // knobs
    gsSPEndDisplayList(),
};


const Gfx custom_door_3_dl_near[] = {
    gsSPDisplayList(custom_door_tex_setup),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, custom_door_3_top_txt),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPDisplayList(0x030149C0), // top and bottom, left and right
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (64 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, custom_door_3_side_txt),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 64 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPDisplayList(0x03014A20), // front and back
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPDisplayList(0x007FE380), // knobs
    gsSPEndDisplayList(),
};

const Gfx custom_door_3_dl_far[] = {
    gsSPDisplayList(custom_door_tex_setup),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (64 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, custom_door_3_side_txt),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 64 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPDisplayList(0x03014EF0), // front and back
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPLight(&door_seg3_lights_03009CF8.l, 1),
    gsSPLight(&door_seg3_lights_03009CF8.a, 2),
    gsSPDisplayList(0x007FE380), // knobs
    gsSPEndDisplayList(),
};


const Gfx custom_door_4_dl_near[] = {
    gsSPDisplayList(custom_door_tex_setup),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, custom_door_4_top_txt),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPDisplayList(0x030149C0), // top and bottom, left and right
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (64 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, custom_door_4_side_txt),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 64 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPDisplayList(0x03014A20), // front and back
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_SHADING_SMOOTH),
    gsSPDisplayList(0x007FE380), // knobs
    gsSPEndDisplayList(),
};

const Gfx custom_door_4_dl_far[] = {
    gsSPDisplayList(custom_door_tex_setup),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (64 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, custom_door_4_side_txt),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 64 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPDisplayList(0x03014EF0), // front and back
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPLight(&door_seg3_lights_03009CF8.l, 1),
    gsSPLight(&door_seg3_lights_03009CF8.a, 2),
    gsSPDisplayList(0x007FE380), // knobs
    gsSPEndDisplayList(),
};
