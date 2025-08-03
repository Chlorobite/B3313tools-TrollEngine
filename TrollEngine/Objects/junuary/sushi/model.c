#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "geo_commands_leek.h"
#include "make_const_nonconst.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"
/**************************************************************/
/*	shape data shark_basedata (skeleton & animation) */
/**************************************************************/

#define ShapeColor_shark(r,g,b)   {{r/4 ,g/4 ,b/4 ,0 ,r/4 ,g/4 ,b/4 ,0 },{ r,g,b,0, r,g,b,0, 40, 40, 40, 0} }


/*-------------------------------------------------------------*/
/*	light data                                                 */
/*-------------------------------------------------------------*/
static Lights1 light_shark[] = {
	ShapeColor_shark(255,255,255),
	ShapeColor_shark(150,  0,  0),
	ShapeColor_shark( 34, 26, 28),
};

/*-------------------------------------------------------------*/
/*	Texture data                                               */
/*-------------------------------------------------------------*/
ALIGNED8 static const Texture shark_eye_txt[] = {
#include "shark_eye_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture shark_skin_txt[] = {
#include "shark_skin_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture shark_skinB_txt[] = {
#include "shark_skinB_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture shark_skinC_txt[] = {
#include "shark_skinC_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture shark_skinD_txt[] = {
#include "shark_skinD_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture shark_skinE_txt[] = {
#include "shark_skinE_txt.rgba16.inc.c"
};

ALIGNED8 static const Texture shark_tooth_txt[] = {
#include "shark_tooth_txt.rgba16.inc.c"
};

/***************************************************************************************************
		Convert from NINGEN		(s_head) 																
****************************************************************************************************/
static Vtx vtx_s_head_0[] = {
	{     63,      0,    -80,     0,     0,   178,   101,   36,  -66,  255}, 
	{      2,     52,    -45,     0, 17836,    64,    37,   94,  -75,  255}, 
	{      5,     54,    -41,     0, 17852,   894,    52,   99,  -58,  255}, 
	{     66,      0,    -75,     0,     0,   990,    86,  -49,  -78,  255}, 
	{      5,    -53,    -41,     0, 17852,   894,    39,  -93,  -76,  255}, 
	{      2,    -51,    -45,     0, 17836,    64,    52,  -99,  -58,  255}, 
};

static Vtx vtx_s_head_1[] = {
	{      0,    -53,     29,     0,     0,   162,   -48,  -72,   92,  255}, 
	{    105,    -54,     29,     0,   828,   158,    19, -116,   47,  255}, 
	{    105,      0,     43,     0,   828,   480,     7,    0,  126,  255}, 
	{    105,     55,     29,     0,   828,   800,    19,  116,   47,  255}, 
	{      0,     54,     29,     0,     0,   796,   -48,   71,   92,  255}, 
	{      0,      0,     43,     0,     0,   478,  -114,    0,   55,  255}, 
	{    235,     14,     23,     0,  1892,   562,    83,   81,   49,  255}, 
	{    235,    -13,     23,     0,  1892,   396,    82,  -82,   50,  255}, 
};

static Vtx vtx_s_head_2[] = {
	{      0,    -53,    -44,     0,  1006,   986,   -48,  -97,  -65,  255}, 
	{      0,      0,    -67,     0,   836,   482,  -101,    0,  -76,  255}, 
	{     63,      0,    -80,     0,   -38,   524,   101,   36,  -66,  255}, 
	{      0,     54,    -44,     0,   988,   -38,   -48,   97,  -65,  255}, 
};

static Vtx vtx_s_head_3[] = {
	{    105,    -36,    -31,     0,  1120,   738,    35,  -99,  -70,  255}, 
	{    158,      0,    -43,     0,   676,   860,    52,    0, -115,  255}, 
	{    236,      0,      7,     0,    20,   204,    93,   -1,  -85,  255}, 
	{    235,    -13,     23,     0,    32,     8,    82,  -82,   50,  255}, 
	{    105,    -54,     29,     0,  1112,   -18,    19, -116,   47,  255}, 
	{     80,      0,    -67,     0,  1328,  1188,    10,    0, -126,  255}, 
	{      0,    -53,    -44,     0,  1996,   934,   -48,  -97,  -65,  255}, 
	{     80,      0,    -67,     0,  1296,  1064,    10,    0, -126,  255}, 
	{      0,     54,    -44,     0,  1968,   846,   -48,   97,  -65,  255}, 
	{    105,     37,    -31,     0,  1096,   724,    35,   98,  -71,  255}, 
	{    105,     55,     29,     0,  1092,   144,    19,  116,   47,  255}, 
	{    235,     14,     23,     0,    12,   202,    83,   81,   49,  255}, 
	{    158,      0,    -43,     0,   652,   834,    52,    0, -115,  255}, 
	{    236,      0,      7,     0,     0,   352,    93,   -1,  -85,  255}, 
	{      0,     54,     29,     0,  1964,   144,   -48,   71,   92,  255}, 
	{      0,    -53,     29,     0,  1984,    20,   -48,  -72,   92,  255}, 
};

static Vtx vtx_s_head_4[] = {
	{    235,    -13,     23,     0,     0,   456,    82,  -82,   50,  255}, 
	{    236,      0,      7,     0,   444,   652,    93,   -1,  -85,  255}, 
	{    235,     14,     23,     0,    -4,   844,    83,   81,   49,  255}, 
	{      0,    -53,    -44,     0,  1952,   810,   -48,  -97,  -65,  255}, 
	{      0,    -53,     29,     0,  1948,   262,   -48,  -72,   92,  255}, 
	{      0,      0,     43,     0,  1368,   164,  -114,    0,   55,  255}, 
	{      0,      0,    -67,     0,  1376,   992,  -101,    0,  -76,  255}, 
	{      0,     54,    -44,     0,   796,   834,   -48,   97,  -65,  255}, 
	{      0,     54,     29,     0,   792,   284,   -48,   71,   92,  255}, 
};

static Vtx vtx_s_head_5[] = {
	{      0,    -53,    -44,     0,     0,     0,   -48,  -97,  -65,  255}, 
	{     63,      0,    -80,     0,     0,     0,   101,   36,  -66,  255}, 
	{     17,      0,    -52,     0,     0,     0,   117,    0,  -47,  255}, 
	{      0,     54,    -44,     0,     0,     0,   -48,   97,  -65,  255}, 
	{      0,     54,    -44,     0,     0,     0,   -48,   97,  -65,  255}, 
	{     80,      0,    -67,     0,     0,     0,    10,    0, -126,  255}, 
	{     17,      0,    -52,     0,     0,     0,   117,    0,  -47,  255}, 
	{      0,    -53,    -44,     0,     0,     0,   -48,  -97,  -65,  255}, 
};

static Gfx gfx_s_head_0[] = {
	gs_Tani_LoadTextureImage2(shark_tooth_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),

	gsSPLight((&light_shark[0].l[0]),1 ),
	gsSPLight((&light_shark[0].a   ),2 ),

	gsSPVertex(&vtx_s_head_0[0],  6, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 0, 2, 3, 0),
	gsSP1Triangle( 3, 4, 5, 0),
	gsSP1Triangle( 3, 5, 0, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_head_1[] = {
	gs_Tani_LoadTextureImage2(shark_skin_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPLight((&light_shark[0].l[0]),1 ),
	gsSPLight((&light_shark[0].a   ),2 ),
	gsSPVertex(&vtx_s_head_1[0],  8, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 2, 3, 4, 0),
	gsSP1Triangle( 4, 5, 2, 0),
	gsSP1Triangle( 2, 5, 0, 0),
	gsSP1Triangle( 6, 3, 2, 0),
	gsSP1Triangle( 2, 1, 7, 0),
	gsSP1Triangle( 7, 6, 2, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_head_2[] = {
	gs_Tani_LoadTextureImage2(shark_skinB_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPVertex(&vtx_s_head_2[0],  4, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 1, 3, 2, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_head_3[] = {
	gs_Tani_LoadTextureImage2(shark_eye_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 64, 32, 0, 7),
	gsSPVertex(&vtx_s_head_3[0], 16, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 2, 3, 0, 0),
	gsSP1Triangle( 0, 3, 4, 0),
	gsSP1Triangle( 5, 1, 0, 0),
	gsSP1Triangle( 6, 0, 4, 0),
	gsSP1Triangle( 6, 5, 0, 0),
	gsSP1Triangle( 7, 8, 9, 0),
	gsSP1Triangle(10,11, 9, 0),
	gsSP1Triangle( 9,12, 7, 0),
	gsSP1Triangle(13,12, 9, 0),
	gsSP1Triangle( 9,11,13, 0),
	gsSP1Triangle(10, 9, 8, 0),
	gsSP1Triangle( 8,14,10, 0),
	gsSP1Triangle( 6, 4,15, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_head_4[] = {
	gs_Tani_LoadTextureImage2(shark_skinE_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 64, 32, 0, 7),
	gsSPVertex(&vtx_s_head_4[0],  9, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 4, 5, 0),
	gsSP1Triangle( 6, 3, 5, 0),
	gsSP1Triangle( 7, 6, 5, 0),
	gsSP1Triangle( 7, 5, 8, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_head_5[] = {

	gsSPLight((&light_shark[1].l[0]),1 ),
	gsSPLight((&light_shark[1].a   ),2 ),

	gsSPVertex(&vtx_s_head_5[0],  4, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 2, 1, 0),

	gsSPLight((&light_shark[2].l[0]),1 ),
	gsSPLight((&light_shark[2].a   ),2 ),

	gsSPVertex(&vtx_s_head_5[4],  4, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 2, 1, 0),
	gsSPEndDisplayList() 
};

const Gfx RCP_s_head_surf[] = {
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gsDPSetTile(G_IM_FMT_RGBA,G_IM_SIZ_16b, 0, 0, 7, 0,  0,0,0, 0,0,0),

	gsSPTexture(0xffff,0xffff, 0, 0, G_ON),

	gs_Tani_SetUpTileDescrip(G_IM_FMT_RGBA,G_IM_SIZ_16b, 32, 32,  0, 0, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD),

		gsSPDisplayList(gfx_s_head_1),
		gsSPDisplayList(gfx_s_head_2),

	gs_Tani_SetUpTileDescrip(G_IM_FMT_RGBA,G_IM_SIZ_16b, 64, 32,  0, 0, G_TX_WRAP|G_TX_NOMIRROR, 6, G_TX_NOLOD, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD),

		gsSPDisplayList(gfx_s_head_3),
		gsSPDisplayList(gfx_s_head_4),

	gsSPTexture(0xffff, 0xffff, 0, 0, G_OFF),

	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),

		gsSPDisplayList(gfx_s_head_5),

	gsSPEndDisplayList() 
};

const Gfx RCP_s_head_sprite[] = {
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
	gsSPClearGeometryMode(G_CULL_BACK),
	gsDPSetTile(G_IM_FMT_RGBA,G_IM_SIZ_16b, 0, 0, 7, 0,  0,0,0, 0,0,0),

	gsSPTexture(0xffff,0xffff, 0, 0, G_ON),

	gs_Tani_SetUpTileDescrip(G_IM_FMT_RGBA,G_IM_SIZ_16b, 32, 32,  0, 0, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD),

		gsSPDisplayList(gfx_s_head_0),

	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPSetGeometryMode(G_CULL_BACK),
	gsSPEndDisplayList() 
};

/***************************************************************************************************
		Convert from NINGEN		(s_bodyA) 																
****************************************************************************************************/
static Vtx vtx_s_bodyA_0[] = {
	{     52,     18,     37,     0,  -100,   382,   111,   41,   45,  255}, 
	{     51,     49,     21,     0,   -92,   222,    79,   92,   33,  255}, 
	{    -55,     52,     28,     0,  1000,   206,   -34,   99,   70,  255}, 
	{    -54,      0,     42,     0,   990,   478,  -112,   -1,   59,  255}, 
	{    -53,    -54,     28,     0,   980,   752,   -30, -101,   70,  255}, 
	{     53,    -46,     21,     0,  -112,   714,    83,  -89,   33,  255}, 
	{     53,    -15,     37,     0,  -106,   554,    95,  -52,   64,  255}, 
};

static Vtx vtx_s_bodyA_1[] = {
	{    -19,    -54,    -44,     0,   396,   -90,     4, -104,  -71,  255}, 
	{    -53,    -54,    -45,     0,   124,   -96,   -96,  -69,  -44,  255}, 
	{    -54,      0,    -68,     0,   114,   458,   -55,    0, -114,  255}, 
	{     52,      1,    -58,     0,   990,   478,    63,    1, -110,  255}, 
	{     53,    -25,    -51,     0,   994,   202,   121,    9,  -35,  255}, 
	{    -55,     53,    -45,     0,   106,  1008,   -98,   66,  -44,  255}, 
	{    -21,     54,    -44,     0,   378,  1016,     0,  105,  -71,  255}, 
	{     52,     28,    -51,     0,   986,   756,   118,   -5,  -44,  255}, 
};

static Vtx vtx_s_bodyA_2[] = {
	{    -54,      0,     42,     0,  1046,   448,  -112,   -1,   59,  255}, 
	{     53,      1,    136,     0,   160,    -4,   115,    3,   52,  255}, 
	{     52,     18,     37,     0,   312,   838,   111,   41,   45,  255}, 
	{     53,    -45,    -43,     0,   862,   224,    92,  -61,   61,  255}, 
	{    110,   -122,   -100,     0,   138,   844,   105,   -3,   70,  255}, 
	{     53,    -25,    -51,     0,   978,   340,   121,    9,  -35,  255}, 
	{     52,     28,    -51,     0,    94,   426,   118,   -5,  -44,  255}, 
	{    105,    127,   -100,     0,  1118,   836,   104,    8,   71,  255}, 
	{     51,     48,    -43,     0,   254,   218,    89,   66,   61,  255}, 
	{    -21,     54,    -44,     0,  -168,   428,     0,  105,  -71,  255}, 
	{     51,     48,    -43,     0,   610,   770,    89,   66,   61,  255}, 
	{    105,    127,   -100,     0,  1860,    54,   104,    8,   71,  255}, 
	{    110,   -122,   -100,     0,   868,   142,   105,   -3,   70,  255}, 
	{     53,    -45,    -43,     0,   484,   772,    92,  -61,   61,  255}, 
	{    -19,    -54,    -44,     0,  -152,   524,     4, -104,  -71,  255}, 
	{     53,    -15,     37,     0,   388,   930,    95,  -52,   64,  255}, 
	{     53,      1,    136,     0,  1000,   -22,   115,    3,   52,  255}, 
	{    -54,      0,     42,     0,  -216,   526,  -112,   -1,   59,  255}, 
	{     53,      1,    136,     0,   458,   -18,   115,    3,   52,  255}, 
	{     53,    -15,     37,     0,   260,   876,    95,  -52,   64,  255}, 
	{     52,     18,     37,     0,   656,   876,   111,   41,   45,  255}, 
};

static Vtx vtx_s_bodyA_3[] = {
	{     53,    -25,    -51,     0,   138,   892,   121,    9,  -35,  255}, 
	{    110,   -122,   -100,     0,  1096,   424,   105,   -3,   70,  255}, 
	{    -19,    -54,    -44,     0,    -2,   -20,     4, -104,  -71,  255}, 
	{    105,    127,   -100,     0,   -24,   296,   104,    8,   71,  255}, 
	{     52,     28,    -51,     0,   896,   634,   118,   -5,  -44,  255}, 
	{    -21,     54,    -44,     0,   968,   -30,     0,  105,  -71,  255}, 
};

static Vtx vtx_s_bodyA_4[] = {
	{     53,    -25,    -51,     0,  1008,   930,   121,    9,  -35,  255}, 
	{     52,      1,    -58,     0,  1320,   988,    63,    1, -110,  255}, 
	{     52,     18,     37,     0,  1484,    24,   111,   41,   45,  255}, 
	{    -53,    -54,    -45,     0,  1800,   872,   -96,  -69,  -44,  255}, 
	{    -53,    -54,     28,     0,  1792,   118,   -30, -101,   70,  255}, 
	{    -54,      0,     42,     0,  1244,     0,  -112,   -1,   59,  255}, 
	{    -54,      0,    -68,     0,  1244,  1108,   -55,    0, -114,  255}, 
	{    -55,     53,    -45,     0,   692,   872,   -98,   66,  -44,  255}, 
	{    -55,     52,     28,     0,   700,   118,   -34,   99,   70,  255}, 
	{     52,     28,    -51,     0,  1628,   896,   118,   -5,  -44,  255}, 
	{     51,     49,     21,     0,  1844,   166,    79,   92,   33,  255}, 
	{     51,     48,    -43,     0,  1852,   810,    89,   66,   61,  255}, 
	{     53,    -15,     37,     0,  1096,    44,    95,  -52,   64,  255}, 
	{     51,     49,     21,     0,  1832,    80,    79,   92,   33,  255}, 
	{     51,     48,    -43,     0,  1832,   964,    89,   66,   61,  255}, 
	{    -21,     54,    -44,     0,  1076,   978,     0,  105,  -71,  255}, 
	{     53,    -45,    -43,     0,   780,   866,    92,  -61,   61,  255}, 
	{     53,    -25,    -51,     0,  1008,   930,   121,    9,  -35,  255}, 
	{     53,    -46,     21,     0,   748,   224,    83,  -89,   33,  255}, 
	{     53,    -15,     37,     0,  1096,    44,    95,  -52,   64,  255}, 
	{    -19,    -54,    -44,     0,  1180,   978,     4, -104,  -71,  255}, 
	{     53,    -46,     21,     0,  1848,    80,    83,  -89,   33,  255}, 
	{    -53,    -54,     28,     0,   876,   -18,   -30, -101,   70,  255}, 
	{    -53,    -54,    -45,     0,   876,   990,   -96,  -69,  -44,  255}, 
	{     53,    -45,    -43,     0,  1848,   964,    92,  -61,   61,  255}, 
	{    -55,     52,     28,     0,   736,   -18,   -34,   99,   70,  255}, 
	{     51,     49,     21,     0,  1832,    80,    79,   92,   33,  255}, 
	{    -21,     54,    -44,     0,  1076,   978,     0,  105,  -71,  255}, 
	{    -55,     53,    -45,     0,   736,   990,   -98,   66,  -44,  255}, 
};

static Gfx gfx_s_bodyA_0[] = {
	gs_Tani_LoadTextureImage2(shark_skin_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),

	gsSPLight((&light_shark[0].l[0]),1 ),
	gsSPLight((&light_shark[0].a   ),2 ),

	gsSPVertex(&vtx_s_bodyA_0[0],  7, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 2, 3, 0, 0),
	gsSP1Triangle( 4, 5, 6, 0),
	gsSP1Triangle( 3, 4, 6, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_bodyA_1[] = {
	gs_Tani_LoadTextureImage2(shark_skinB_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPVertex(&vtx_s_bodyA_1[0],  8, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 0, 2, 0),
	gsSP1Triangle( 3, 4, 0, 0),
	gsSP1Triangle( 2, 5, 6, 0),
	gsSP1Triangle( 3, 2, 6, 0),
	gsSP1Triangle( 7, 3, 6, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_bodyA_2[] = {
	gs_Tani_LoadTextureImage2(shark_skinC_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPVertex(&vtx_s_bodyA_2[0], 15, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 4, 5, 0),
	gsSP1Triangle( 6, 7, 8, 0),
	gsSP1Triangle( 9,10,11, 0),
	gsSP1Triangle(12,13,14, 0),
	gsSPVertex(&vtx_s_bodyA_2[15],  6, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 4, 5, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_bodyA_3[] = {
	gs_Tani_LoadTextureImage2(shark_skinD_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPVertex(&vtx_s_bodyA_3[0],  6, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 4, 5, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_bodyA_4[] = {
	gs_Tani_LoadTextureImage2(shark_skinE_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 64, 32, 0, 7),
	gsSPVertex(&vtx_s_bodyA_4[0], 16, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 4, 5, 0),
	gsSP1Triangle( 6, 3, 5, 0),
	gsSP1Triangle( 7, 5, 8, 0),
	gsSP1Triangle( 7, 6, 5, 0),
	gsSP1Triangle( 1, 9, 2, 0),
	gsSP1Triangle( 9,10, 2, 0),
	gsSP1Triangle( 9,11,10, 0),
	gsSP1Triangle( 0, 2,12, 0),
	gsSP1Triangle(13,14,15, 0),
	gsSPVertex(&vtx_s_bodyA_4[16], 13, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 1, 3, 2, 0),
	gsSP1Triangle( 4, 5, 6, 0),
	gsSP1Triangle( 6, 7, 4, 0),
	gsSP1Triangle( 4, 8, 5, 0),
	gsSP1Triangle( 9,10,11, 0),
	gsSP1Triangle(11,12, 9, 0),
	gsSPEndDisplayList() 
};

const Gfx RCP_s_bodyA[] = {
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gsDPSetTile(G_IM_FMT_RGBA,G_IM_SIZ_16b, 0, 0, 7, 0,  0,0,0, 0,0,0),

	gsSPTexture(0xffff,0xffff, 0, 0, G_ON),

	gs_Tani_SetUpTileDescrip(G_IM_FMT_RGBA,G_IM_SIZ_16b, 32, 32,  0, 0, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD),

		gsSPDisplayList(gfx_s_bodyA_0),
		gsSPDisplayList(gfx_s_bodyA_1),
		gsSPDisplayList(gfx_s_bodyA_2),
		gsSPDisplayList(gfx_s_bodyA_3),

	gs_Tani_SetUpTileDescrip(G_IM_FMT_RGBA,G_IM_SIZ_16b, 64, 32,  0, 0, G_TX_WRAP|G_TX_NOMIRROR, 6, G_TX_NOLOD, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD),

		gsSPDisplayList(gfx_s_bodyA_4),

	gsSPTexture(0xffff, 0xffff, 0, 0, G_OFF),

	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPEndDisplayList() 
};

/***************************************************************************************************
		Convert from NINGEN		(s_bodyB) 																
****************************************************************************************************/
static Vtx vtx_s_bodyB_0[] = {
	{    104,    -39,     17,     0,   -48,   718,    57,  -77,   83,  255}, 
	{    104,      1,     30,     0,   -48,   478,   108,    0,   65,  255}, 
	{      2,      1,     37,     0,  1604,   478,   -13,    0,  126,  255}, 
	{      2,    -47,     21,     0,  1604,   764,    -7, -115,   52,  255}, 
	{    104,     42,     17,     0,   -48,   240,    86,   62,   68,  255}, 
	{      2,     50,     21,     0,  1604,   194,    -7,  115,   52,  255}, 
	{    -17,     45,     17,     0,  1932,   222,   -68,   88,   60,  255}, 
	{    -17,      1,     32,     0,  1932,   480,  -112,    0,   59,  255}, 
	{    -17,    -42,     17,     0,  1932,   736,   -87,  -75,   51,  255}, 
};

static Vtx vtx_s_bodyB_1[] = {
	{    104,     32,    -38,     0, -3140,   908,    50,   88,  -75,  255}, 
	{      2,      1,    -58,     0,   990,   478,    -8,    0, -126,  255}, 
	{      2,     47,    -45,     0,   990,  1108,   -14,  109,  -63,  255}, 
	{    104,      1,    -47,     0, -3140,   480,   109,    0,  -65,  255}, 
	{    104,    -30,    -38,     0, -3140,    50,    75,  -78,  -65,  255}, 
	{    -17,      1,    -54,     0,  1810,   478,  -111,    0,  -61,  255}, 
	{    -17,     43,    -41,     0,  1810,  1046,   -93,   53,  -66,  255}, 
	{      2,    -44,    -45,     0,   990,  -152,   -14, -109,  -63,  255}, 
	{    -17,    -40,    -41,     0,  1810,   -90,   -71,  -66,  -81,  255}, 
};

static Vtx vtx_s_bodyB_2[] = {
	{      2,    -44,    -45,     0,   920,   990,   -14, -109,  -63,  255}, 
	{    104,    -30,    -38,     0,  2024,   906,    75,  -78,  -65,  255}, 
	{      2,    -47,     21,     0,   920,    88,    -7, -115,   52,  255}, 
	{    -17,     43,    -41,     0,   940,   838,   -93,   53,  -66,  255}, 
	{    -17,      1,    -54,     0,  1428,   982,  -111,    0,  -61,  255}, 
	{    -17,      1,     32,     0,  1428,     0,  -112,    0,   59,  255}, 
	{    -17,    -42,     17,     0,  1940,   140,   -87,  -75,   51,  255}, 
	{    -17,    -40,    -41,     0,  1916,   838,   -71,  -66,  -81,  255}, 
	{    -17,     45,     17,     0,   916,   140,   -68,   88,   60,  255}, 
	{    104,      1,    -47,     0,  1384,   884,   109,    0,  -65,  255}, 
	{    104,     32,    -38,     0,  1780,   778,    50,   88,  -75,  255}, 
	{    104,     42,     17,     0,  1896,   122,    86,   62,   68,  255}, 
	{    104,      1,     30,     0,  1384,     0,   108,    0,   65,  255}, 
	{    104,    -30,    -38,     0,   988,   778,    75,  -78,  -65,  255}, 
	{    104,    -39,     17,     0,   868,   122,    57,  -77,   83,  255}, 
	{    -17,    -42,     17,     0,   704,   132,   -87,  -75,   51,  255}, 
	{    -17,    -40,    -41,     0,   704,   944,   -71,  -66,  -81,  255}, 
	{      2,    -44,    -45,     0,   920,   990,   -14, -109,  -63,  255}, 
	{    104,    -30,    -38,     0,  2024,   906,    75,  -78,  -65,  255}, 
	{    104,    -39,     17,     0,  2024,   140,    57,  -77,   83,  255}, 
	{      2,    -47,     21,     0,   920,    88,    -7, -115,   52,  255}, 
	{    104,     32,    -38,     0,  1900,   906,    50,   88,  -75,  255}, 
	{      2,     47,    -45,     0,   928,   990,   -14,  109,  -63,  255}, 
	{      2,     50,     21,     0,   928,    88,    -7,  115,   52,  255}, 
	{    -17,     45,     17,     0,   736,   132,   -68,   88,   60,  255}, 
	{    104,     42,     17,     0,  1900,   140,    86,   62,   68,  255}, 
	{    -17,     43,    -41,     0,   736,   944,   -93,   53,  -66,  255}, 
};

static Gfx gfx_s_bodyB_0[] = {
	gs_Tani_LoadTextureImage2(shark_skin_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),

	gsSPLight((&light_shark[0].l[0]),1 ),
	gsSPLight((&light_shark[0].a   ),2 ),

	gsSPVertex(&vtx_s_bodyB_0[0],  9, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 2, 3, 0, 0),
	gsSP1Triangle( 2, 1, 4, 0),
	gsSP1Triangle( 4, 5, 2, 0),
	gsSP1Triangle( 6, 7, 2, 0),
	gsSP1Triangle( 2, 7, 8, 0),
	gsSP1Triangle( 5, 6, 2, 0),
	gsSP1Triangle( 2, 8, 3, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_bodyB_1[] = {
	gs_Tani_LoadTextureImage2(shark_skinB_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPVertex(&vtx_s_bodyB_1[0],  9, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 0, 3, 1, 0),
	gsSP1Triangle( 1, 3, 4, 0),
	gsSP1Triangle( 1, 5, 6, 0),
	gsSP1Triangle( 1, 6, 2, 0),
	gsSP1Triangle( 7, 8, 1, 0),
	gsSP1Triangle( 8, 5, 1, 0),
	gsSP1Triangle( 4, 7, 1, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_bodyB_2[] = {
	gs_Tani_LoadTextureImage2(shark_skinE_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 64, 32, 0, 7),
	gsSPVertex(&vtx_s_bodyB_2[0], 15, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 4, 5, 0),
	gsSP1Triangle( 4, 6, 5, 0),
	gsSP1Triangle( 4, 7, 6, 0),
	gsSP1Triangle( 3, 5, 8, 0),
	gsSP1Triangle( 9,10,11, 0),
	gsSP1Triangle( 9,11,12, 0),
	gsSP1Triangle(13, 9,12, 0),
	gsSP1Triangle(13,12,14, 0),
	gsSPVertex(&vtx_s_bodyB_2[15], 12, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 4, 5, 0),
	gsSP1Triangle( 5, 0, 2, 0),
	gsSP1Triangle( 6, 7, 8, 0),
	gsSP1Triangle( 7, 9, 8, 0),
	gsSP1Triangle( 8,10, 6, 0),
	gsSP1Triangle( 7,11, 9, 0),
	gsSPEndDisplayList() 
};

const Gfx RCP_s_bodyB[] = {
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gsDPSetTile(G_IM_FMT_RGBA,G_IM_SIZ_16b, 0, 0, 7, 0,  0,0,0, 0,0,0),

	gsSPTexture(0xffff,0xffff, 0, 0, G_ON),

	gs_Tani_SetUpTileDescrip(G_IM_FMT_RGBA,G_IM_SIZ_16b, 32, 32,  0, 0, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD),

		gsSPDisplayList(gfx_s_bodyB_0),
		gsSPDisplayList(gfx_s_bodyB_1),

	gs_Tani_SetUpTileDescrip(G_IM_FMT_RGBA,G_IM_SIZ_16b, 64, 32,  0, 0, G_TX_WRAP|G_TX_NOMIRROR, 6, G_TX_NOLOD, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD),

		gsSPDisplayList(gfx_s_bodyB_2),

	gsSPTexture(0xffff, 0xffff, 0, 0, G_OFF),

	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPEndDisplayList() 
};

/***************************************************************************************************
		Convert from NINGEN		(s_bodyC) 																
****************************************************************************************************/
static Vtx vtx_s_bodyC_0[] = {
	{    122,     11,     16,     0,  -110,   420,    69,   87,   60,  255}, 
	{      1,     41,     17,     0,   988,   244,    -6,  108,   66,  255}, 
	{      0,      1,     30,     0,   990,   478,   -15,    0,  126,  255}, 
	{    -19,     36,     13,     0,  1178,   276,   -70,   91,   53,  255}, 
	{    -20,      1,     24,     0,  1182,   478,  -112,    1,   58,  255}, 
	{    -20,    -32,     13,     0,  1184,   680,   -91,  -75,   45,  255}, 
	{      0,    -38,     17,     0,   992,   714,    -9, -108,   65,  255}, 
	{    122,    -10,     16,     0,  -108,   548,    67,  -88,   60,  255}, 
};

static Vtx vtx_s_bodyC_1[] = {
	{      0,      1,    -47,     0,   650,   478,   -11,    0, -126,  255}, 
	{    -19,     28,    -34,     0,   934,   844,   -91,   54,  -69,  255}, 
	{      1,     32,    -38,     0,   646,   902,     1,  108,  -65,  255}, 
	{    122,      0,    -19,     0, -1000,   466,    80,   -1,  -98,  255}, 
	{    -20,    -24,    -34,     0,   940,   120,   -72,  -62,  -83,  255}, 
	{    -20,      1,    -42,     0,   936,   482,  -111,    0,  -60,  255}, 
	{      0,    -29,    -38,     0,   652,    56,    -2, -108,  -65,  255}, 
};

static Vtx vtx_s_bodyC_2[] = {
	{      0,    -38,     17,     0,   992,   250,    -9, -108,   65,  255}, 
	{    -20,    -32,     13,     0,   820,   304,   -91,  -75,   45,  255}, 
	{      0,    -29,    -38,     0,   992,  1014,    -2, -108,  -65,  255}, 
	{    -20,      1,    -42,     0,  1300,   838,  -111,    0,  -60,  255}, 
	{    -20,    -24,    -34,     0,  1716,   752,   -72,  -62,  -83,  255}, 
	{    -20,    -32,     13,     0,  1832,   212,   -91,  -75,   45,  255}, 
	{    -20,      1,     24,     0,  1292,    84,  -112,    1,   58,  255}, 
	{    -19,     28,    -34,     0,   884,   748,   -91,   54,  -69,  255}, 
	{    -19,     36,     13,     0,   756,   208,   -70,   91,   53,  255}, 
	{      1,     32,    -38,     0,   996,  1014,     1,  108,  -65,  255}, 
	{      1,     41,     17,     0,   996,   250,    -6,  108,   66,  255}, 
	{    122,     11,     16,     0,  1984,   258,    69,   87,   60,  255}, 
	{    -19,     36,     13,     0,   824,   304,   -70,   91,   53,  255}, 
	{    -19,     28,    -34,     0,   824,   958,   -91,   54,  -69,  255}, 
	{    122,      0,    -19,     0,  1984,   746,    80,   -1,  -98,  255}, 
	{    -20,    -24,    -34,     0,   820,   958,   -72,  -62,  -83,  255}, 
	{      0,    -29,    -38,     0,   992,  1014,    -2, -108,  -65,  255}, 
	{    122,    -10,     16,     0,  1984,   258,    67,  -88,   60,  255}, 
	{      0,    -38,     17,     0,   992,   250,    -9, -108,   65,  255}, 
	{    122,      0,    -19,     0,  1984,   746,    80,   -1,  -98,  255}, 
	{    122,      0,    -19,     0,  1472,   936,    80,   -1,  -98,  255}, 
	{    122,     11,     16,     0,  1568,   266,    69,   87,   60,  255}, 
	{    122,    -10,     16,     0,  1152,   290,    67,  -88,   60,  255}, 
};

static Gfx gfx_s_bodyC_0[] = {
	gs_Tani_LoadTextureImage2(shark_skin_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),

	gsSPLight((&light_shark[0].l[0]),1 ),
	gsSPLight((&light_shark[0].a   ),2 ),

	gsSPVertex(&vtx_s_bodyC_0[0],  8, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 4, 2, 0),
	gsSP1Triangle( 1, 3, 2, 0),
	gsSP1Triangle( 2, 4, 5, 0),
	gsSP1Triangle( 2, 5, 6, 0),
	gsSP1Triangle( 7, 0, 2, 0),
	gsSP1Triangle( 2, 6, 7, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_bodyC_1[] = {
	gs_Tani_LoadTextureImage2(shark_skinB_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPVertex(&vtx_s_bodyC_1[0],  7, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 0, 2, 0),
	gsSP1Triangle( 4, 5, 0, 0),
	gsSP1Triangle( 3, 6, 0, 0),
	gsSP1Triangle( 6, 4, 0, 0),
	gsSP1Triangle( 0, 5, 1, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_bodyC_2[] = {
	gs_Tani_LoadTextureImage2(shark_skinE_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 64, 32, 0, 7),
	gsSPVertex(&vtx_s_bodyC_2[0], 16, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 4, 5, 0),
	gsSP1Triangle( 3, 5, 6, 0),
	gsSP1Triangle( 7, 3, 6, 0),
	gsSP1Triangle( 7, 6, 8, 0),
	gsSP1Triangle( 9,10,11, 0),
	gsSP1Triangle( 9,12,10, 0),
	gsSP1Triangle( 9,13,12, 0),
	gsSP1Triangle(14, 9,11, 0),
	gsSP1Triangle( 1,15, 2, 0),
	gsSPVertex(&vtx_s_bodyC_2[16],  7, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 0, 3, 1, 0),
	gsSP1Triangle( 4, 5, 6, 0),
	gsSPEndDisplayList() 
};

const Gfx RCP_s_bodyC[] = {
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gsDPSetTile(G_IM_FMT_RGBA,G_IM_SIZ_16b, 0, 0, 7, 0,  0,0,0, 0,0,0),

	gsSPTexture(0xffff,0xffff, 0, 0, G_ON),

	gs_Tani_SetUpTileDescrip(G_IM_FMT_RGBA,G_IM_SIZ_16b, 32, 32,  0, 0, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD),

		gsSPDisplayList(gfx_s_bodyC_0),
		gsSPDisplayList(gfx_s_bodyC_1),

	gs_Tani_SetUpTileDescrip(G_IM_FMT_RGBA,G_IM_SIZ_16b, 64, 32,  0, 0, G_TX_WRAP|G_TX_NOMIRROR, 6, G_TX_NOLOD, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD),

		gsSPDisplayList(gfx_s_bodyC_2),

	gsSPTexture(0xffff, 0xffff, 0, 0, G_OFF),

	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPEndDisplayList() 
};

/***************************************************************************************************
		Convert from NINGEN		(s_bodyD) 																
****************************************************************************************************/
static Vtx vtx_s_bodyD_0[] = {
	{      1,      0,    -21,     0,   760,   960,  -112,   -8,  -57,  255}, 
	{      2,     -8,     17,     0,   610,   436,   -74, -100,   18,  255}, 
	{      1,      9,     17,     0,   248,   572,   -81,   95,   17,  255}, 
	{     59,      1,      2,     0,   992,   980,    81,    0,  -97,  255}, 
	{    114,      3,    160,     0,  -382,  1456,   -88,   -7,   90,  255}, 
	{      2,     -8,     17,     0,   704,  1846,   -74, -100,   18,  255}, 
	{      1,      9,     17,     0,   704,  1852,   -81,   95,   17,  255}, 
	{      2,     -8,     17,     0,   816,   752,   -74, -100,   18,  255}, 
	{      1,      0,    -21,     0,   186,   778,  -112,   -8,  -57,  255}, 
	{     59,      1,      2,     0,   568,   -20,    81,    0,  -97,  255}, 
	{      1,      9,     17,     0,   816,   758,   -81,   95,   17,  255}, 
	{      1,      0,    -21,     0,    56,   684,  -112,   -8,  -57,  255}, 
	{     59,      1,      2,     0,    94,    24,    81,    0,  -97,  255}, 
	{    104,      3,    -88,     0,  1054,   304,    -1,   -1,   -1,  255}, 
};

static Vtx vtx_s_bodyD_1[] = {
	{    114,      3,    160,     0,  5680,   492,   -88,   -7,   90,  255}, 
	{      1,      9,     17,     0,    10,   844,   -81,   95,   17,  255}, 
	{      2,     -8,     17,     0,     0,   118,   -74, -100,   18,  255}, 
};

static Gfx gfx_s_bodyD_0[] = {
	gs_Tani_LoadTextureImage2(shark_skinC_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),

	gsSPLight((&light_shark[0].l[0]),1 ),
	gsSPLight((&light_shark[0].a   ),2 ),

	gsSPVertex(&vtx_s_bodyD_0[0], 14, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 4, 5, 0),
	gsSP1Triangle( 4, 3, 6, 0),
	gsSP1Triangle( 7, 8, 9, 0),
	gsSP1Triangle( 8,10, 9, 0),
	gsSP1Triangle(11,12,13, 0),
	gsSP1Triangle(11,13,12, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_s_bodyD_1[] = {
	gs_Tani_LoadTextureImage2(shark_skin_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPVertex(&vtx_s_bodyD_1[0],  3, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSPEndDisplayList() 
};

const Gfx RCP_s_bodyD[] = {
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gsDPSetTile(G_IM_FMT_RGBA,G_IM_SIZ_16b, 0, 0, 7, 0,  0,0,0, 0,0,0),

	gsSPTexture(0xffff,0xffff, 0, 0, G_ON),

	gs_Tani_SetUpTileDescrip(G_IM_FMT_RGBA,G_IM_SIZ_16b, 32, 32,  0, 0, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD),

		gsSPDisplayList(gfx_s_bodyD_0),
		gsSPDisplayList(gfx_s_bodyD_1),

	gsSPTexture(0xffff, 0xffff, 0, 0, G_OFF),

	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPEndDisplayList() 
};

#include "anims/data.inc"
#include "anims/table.inc"
#pragma GCC diagnostic pop
