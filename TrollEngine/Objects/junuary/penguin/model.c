#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "geo_commands_leek.h"
#include "make_const_nonconst.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"
#define ShapeColor_ping_head(R,G,B) {{R/4 ,G/4 ,B/4 ,0 ,R/4 ,G/4 ,B/4 ,0 },{ R, G, B, 0, R, G, B, 0, 40, 40, 40, 0} }
#define ShapeColor_ping_base(R,G,B) {{R/4 ,G/4 ,B/4 ,0 ,R/4 ,G/4 ,B/4 ,0 },{ R, G, B, 0, R, G, B, 0, 40, 40, 40, 0} }



/*-------------------------------------------------------------*/
/*	light data                                                 */
/*-------------------------------------------------------------*/
static Lights1 light_ping_base[]={
	ShapeColor_ping_base(15,15,144),
	ShapeColor_ping_base(206,132,26),
	ShapeColor_ping_base(223,167,52),
	ShapeColor_ping_base(255,255,253),
};

static Lights1 light_ping_head[] = {
	ShapeColor_ping_head( 11, 38,164),
	ShapeColor_ping_head(255,255,255),
	ShapeColor_ping_head(242,213, 28),
};

/*-------------------------------------------------------------*/
/*	texture data                                               */
/*-------------------------------------------------------------*/
static const Texture ping_eye1_txt[] = {
#include "ping_eye1_txt.rgba16.inc.c"
};

static const Texture ping_eye2_txt[] = {
#include "ping_eye2_txt.rgba16.inc.c"
};

static const Texture ping_eye3_txt[] = {
#include "ping_eye3_txt.rgba16.inc.c"
};

static const Texture ping_eye4_txt[] = {
#include "ping_eye4_txt.rgba16.inc.c"
};

static const Texture ping_eye5_txt[] = {
#include "ping_eye5_txt.rgba16.inc.c"
};

static const Texture ping_mouth_txt[] = {
#include "ping_mouth_txt.rgba16.inc.c"
};

/*-------------------------------------------------------------*/
/*	vertex & polygon data                                      */
/*-------------------------------------------------------------*/
static Vtx vtx_ping_base0[]={
	{-1,0,2,0,0,0,1,0,127,0},
	{123,-18,-2,0,0,0,2,-17,125,0},
	{149,20,0,0,0,0,1,13,126,0},
	{21,29,-4,0,0,0,-16,4,-125,0},
	{149,20,0,0,0,0,4,1,-126,0},
	{123,-18,-2,0,0,0,3,0,-127,0},
	{5,-29,-4,0,0,0,-27,5,-123,0},
	{-1,0,2,0,0,0,-55,13,-113,0},
	{21,29,-4,0,0,0,-1,33,122,0},
	{5,-29,-4,0,0,0,0,-28,123,0},
};
static Vtx vtx_ping_base1[]={
	{123,-18,2,0,0,0,2,-17,-125,0},
	{5,-29,4,0,0,0,0,-28,-123,0},
	{-1,0,-2,0,0,0,1,0,-127,0},
	{149,20,0,0,0,0,1,13,-126,0},
	{21,29,4,0,0,0,-1,33,-122,0},
	{-1,0,-2,0,0,0,-55,13,113,0},
	{5,-29,4,0,0,0,-27,5,123,0},
	{21,29,4,0,0,0,-16,4,125,0},
	{123,-18,2,0,0,0,3,0,127,0},
	{149,20,0,0,0,0,4,1,126,0},
};
static Vtx vtx_ping_base2[]={
	{-16,-2,-1,0,0,0,-54,-114,0,0},
	{46,-8,-1,0,0,0,2,-127,0,0},
	{46,16,37,0,0,0,4,-98,80,0},
	{-21,16,-19,0,0,0,-90,-71,-52,0},
	{46,16,-41,0,0,0,4,-98,-80,0},
	{99,16,-16,0,0,0,95,-65,-51,0},
	{94,-1,-1,0,0,0,64,-109,0,0},
	{99,16,12,0,0,0,95,-65,51,0},
	{46,16,37,0,0,0,0,127,0,0},
	{99,16,12,0,0,0,0,127,0,0},
	{46,16,-41,0,0,0,0,127,0,0},
	{-21,16,15,0,0,0,0,127,0,0},
	{-21,16,15,0,0,0,-90,-71,52,0},
	{-21,16,-19,0,0,0,0,127,0,0},
	{99,16,-16,0,0,0,0,127,0,0},
};
static Vtx vtx_ping_base3[]={
	{99,16,-12,0,0,0,95,-65,-51,0},
	{99,16,16,0,0,0,95,-65,51,0},
	{94,-1,1,0,0,0,64,-109,0,0},
	{-21,16,19,0,0,0,-90,-71,52,0},
	{-21,16,-15,0,0,0,-90,-71,-52,0},
	{-16,-2,1,0,0,0,-54,-114,0,0},
	{46,16,-37,0,0,0,4,-98,-80,0},
	{-21,16,-15,0,0,0,0,127,0,0},
	{46,16,41,0,0,0,0,127,0,0},
	{46,16,-37,0,0,0,0,127,0,0},
	{46,16,41,0,0,0,4,-98,80,0},
	{46,-8,1,0,0,0,2,-127,0,0},
	{99,16,-12,0,0,0,0,127,0,0},
	{99,16,16,0,0,0,0,127,0,0},
	{-21,16,19,0,0,0,0,127,0,0},
};
static Vtx vtx_ping_head_0[] = {
	{     59,     73,      0,     0,  -438,   308,    52,  115,    0,  255}, 
	{     68,     57,     41,     0,  1246,    32,    30,   95,   77,  255}, 
	{    110,     47,     26,     0,   620, -1424,    98,   53,   58,  255}, 
	{     18,     67,     36,     0,  1018,  1712,   -64,   95,   53,  255}, 
	{    110,     47,    -25,     0,   620, -1424,    98,   53,  -59,  255}, 
	{     68,     57,    -40,     0,  1246,    32,    30,   95,  -77,  255}, 
	{     18,     67,    -35,     0,  1018,  1712,   -54,   95,  -64,  255}, 
};

static Vtx vtx_ping_head_1[] = {
	{     18,     67,    -35,     0,   962,    68,   -54,   95,  -64,  255}, 
	{     17,    123,      0,     0,   198,  1208,    12,  126,   -1,  255}, 
	{     59,     73,      0,     0,    52,   -24,    52,  115,    0,  255}, 
	{     17,    123,      0,     0,   468,  1066,    12,  126,   -1,  255}, 
	{     18,     67,     36,     0,   990,    54,   -64,   95,   53,  255}, 
	{     59,     73,      0,     0,   -28,    54,    52,  115,    0,  255}, 
};

static Vtx vtx_ping_head_2[] = {
	{     18,     67,     36,     0,     0,     0,   -64,   95,   53,  255}, 
	{     17,    123,      0,     0,     0,     0,    12,  126,   -1,  255}, 
	{     18,     67,    -35,     0,     0,     0,   -54,   95,  -64,  255}, 
	{     19,     46,     59,     0,     0,     0,   -50,   45,  106,  255}, 
	{     -3,    -18,     36,     0,     0,     0,  -118,  -24,   38,  255}, 
	{     21,    -24,     61,     0,     0,     0,   -22,  -50,  114,  255}, 
	{     -3,    -18,    -35,     0,     0,     0,  -114,  -28,  -46,  255}, 
	{     22,    -67,      0,     0,     0,     0,   -50, -116,    0,  255}, 
	{     -5,     39,    -19,     0,     0,     0,  -118,   39,  -21,  255}, 
	{     -5,     39,     20,     0,     0,     0,  -116,   38,   31,  255}, 
	{     18,     67,     36,     0,     0,     0,   -64,   95,   53,  255}, 
	{     18,     67,    -35,     0,     0,     0,   -54,   95,  -64,  255}, 
	{     19,     46,    -58,     0,     0,     0,   -50,   45, -106,  255}, 
	{     80,     32,     52,     0,     0,     0,    40,   28,  116,  255}, 
	{     68,     57,     41,     0,     0,     0,    30,   95,   77,  255}, 
	{     21,    -24,    -60,     0,     0,     0,   -22,  -49, -114,  255}, 
	{     80,     32,    -51,     0,     0,     0,    40,   28, -116,  255}, 
	{     68,     57,    -40,     0,     0,     0,    30,   95,  -77,  255}, 
	{    110,     47,    -25,     0,     0,     0,    98,   53,  -59,  255}, 
	{     79,    -48,    -28,     0,     0,     0,    65,  -95,  -53,  255}, 
	{     22,    -67,      0,     0,     0,     0,   -50, -116,    0,  255}, 
	{     21,    -24,    -60,     0,     0,     0,   -22,  -49, -114,  255}, 
	{     80,     32,    -51,     0,     0,     0,    40,   28, -116,  255}, 
	{     81,    -13,    -51,     0,     0,     0,    78,  -30,  -95,  255}, 
	{    110,     47,    -25,     0,     0,     0,    98,   53,  -59,  255}, 
	{    105,    -12,      0,     0,     0,     0,   120,  -40,    0,  255}, 
	{     79,    -48,     29,     0,     0,     0,    65,  -95,   52,  255}, 
	{     21,    -24,     61,     0,     0,     0,   -22,  -50,  114,  255}, 
	{     81,    -13,     52,     0,     0,     0,    78,  -30,   95,  255}, 
	{    110,     47,     26,     0,     0,     0,    98,   53,   58,  255}, 
	{     80,     32,     52,     0,     0,     0,    40,   28,  116,  255}, 
	{     68,     57,     41,     0,     0,     0,    30,   95,   77,  255}, 
	{     59,     73,      0,     0,     0,     0,    52,  115,    0,  255}, 
};
static Vtx vtx_ping_base5[]={
	{-23,130,24,0,0,0,-47,96,67,0},
	{23,130,24,0,0,0,47,109,42,0},
	{-21,130,-28,0,0,0,-44,108,-49,0},
	{65,-15,-36,0,0,0,116,-19,-47,0},
	{52,36,-53,0,0,0,103,25,-68,0},
	{56,82,19,0,0,0,119,42,8,0},
	{-56,82,19,0,0,0,-119,42,8,0},
	{-52,36,-53,0,0,0,-103,25,-68,0},
	{-65,-15,-36,0,0,0,-116,-19,-47,0},
	{-23,97,-55,0,0,0,-67,50,-94,0},
	{23,97,-55,0,0,0,47,53,-104,0},
	{21,130,-28,0,0,0,67,98,-43,0},
	{-35,-53,-47,0,0,0,-66,-107,-13,0},
	{-65,-4,26,0,0,0,-112,-35,46,0},
	{31,4,-72,0,0,0,53,23,-112,0},
	{0,-45,-103,0,0,0,0,-47,-117,0},
	{-31,4,-72,0,0,0,-44,23,-116,0},
	{31,4,-72,0,0,0,53,23,-112,0},
	{56,82,19,0,0,0,119,42,8,0},
	{65,-4,26,0,0,0,112,-35,46,0},
	{65,-15,-36,0,0,0,116,-19,-47,0},
	{-65,-15,-36,0,0,0,-116,-19,-47,0},
	{-65,-4,26,0,0,0,-112,-35,46,0},
	{-56,82,19,0,0,0,-119,42,8,0},
	{35,-53,-47,0,0,0,55,-113,-11,0},
	{-35,-53,-47,0,0,0,-66,-107,-13,0},
	{-52,36,-53,0,0,0,-103,25,-68,0},
	{-23,97,-55,0,0,0,-67,50,-94,0},
	{23,97,-55,0,0,0,47,53,-104,0},
	{52,36,-53,0,0,0,103,25,-68,0},
	{-21,130,-28,0,0,0,-44,108,-49,0},
	{21,130,-28,0,0,0,67,98,-43,0},
	{23,97,-55,0,0,0,47,53,-104,0},
	{23,130,24,0,0,0,47,109,42,0},
	{24,89,53,0,0,0,43,38,112,0},
	{36,-18,57,0,0,0,42,-56,105,0},
	{65,-4,26,0,0,0,112,-35,46,0},
	{-23,130,24,0,0,0,-47,96,67,0},
	{-24,89,53,0,0,0,-60,35,105,0},
	{-65,-4,26,0,0,0,-112,-35,46,0},
	{-56,82,19,0,0,0,-119,42,8,0},
	{23,130,24,0,0,0,47,109,42,0},
	{56,82,19,0,0,0,119,42,8,0},
	{-36,-18,57,0,0,0,-42,-33,114,0},
	{-28,-53,22,0,0,0,-36,-112,46,0},
	{28,-53,22,0,0,0,54,-106,43,0},
	{35,-53,-47,0,0,0,55,-113,-11,0},
	{-35,-53,-47,0,0,0,-66,-107,-13,0},
};
const Gfx RCP_ping_base0[]={
	gsSPLight((&light_ping_base[0].l[0]),1),
	gsSPLight((&light_ping_base[0].a),2),
	gsSPVertex(&vtx_ping_base0[0],10,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(3,4,5,0),
	gsSP1Triangle(5,6,3,0),
	gsSP1Triangle(3,6,7,0),
	gsSP1Triangle(8,0,2,0),
	gsSP1Triangle(0,9,1,0),
	gsSPEndDisplayList()
};
const Gfx RCP_ping_base1[]={
	gsSPLight((&light_ping_base[0].l[0]),1),
	gsSPLight((&light_ping_base[0].a),2),
	gsSPVertex(&vtx_ping_base1[0],10,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(3,2,4,0),
	gsSP1Triangle(5,6,7,0),
	gsSP1Triangle(7,6,8,0),
	gsSP1Triangle(8,9,7,0),
	gsSP1Triangle(3,0,2,0),
	gsSPEndDisplayList()
};
const Gfx RCP_ping_base2[]={
	gsSPLight((&light_ping_base[1].l[0]),1),
	gsSPLight((&light_ping_base[1].a),2),
	gsSPVertex(&vtx_ping_base2[0],15,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(0,3,4,0),
	gsSP1Triangle(4,5,6,0),
	gsSP1Triangle(6,7,2,0),
	gsSP1Triangle(1,4,6,0),
	gsSP1Triangle(1,6,2,0),
	gsSP1Triangle(8,9,10,0),
	gsSP1Triangle(1,0,4,0),
	gsSP1Triangle(8,10,11,0),
	gsSP1Triangle(2,12,0,0),
	gsSP1Triangle(0,12,3,0),
	gsSP1Triangle(6,5,7,0),
	gsSP1Triangle(10,13,11,0),
	gsSP1Triangle(9,14,10,0),
	gsSPEndDisplayList()
};
const Gfx RCP_ping_base3[]={
	gsSPLight((&light_ping_base[1].l[0]),1),
	gsSPLight((&light_ping_base[1].a),2),
	gsSPVertex(&vtx_ping_base3[0],15,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(3,4,5,0),
	gsSP1Triangle(5,4,6,0),
	gsSP1Triangle(7,8,9,0),
	gsSP1Triangle(10,5,11,0),
	gsSP1Triangle(8,12,9,0),
	gsSP1Triangle(6,2,11,0),
	gsSP1Triangle(2,10,11,0),
	gsSP1Triangle(6,0,2,0),
	gsSP1Triangle(2,1,10,0),
	gsSP1Triangle(10,3,5,0),
	gsSP1Triangle(6,11,5,0),
	gsSP1Triangle(8,13,12,0),
	gsSP1Triangle(7,14,8,0),
	gsSPEndDisplayList()
};
const Gfx RCP_ping_base5[]={
	gsSPLight((&light_ping_base[0].l[0]),1),
	gsSPLight((&light_ping_base[0].a),2),
	gsSPVertex(&vtx_ping_base5[0],15,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(3,4,5,0),
	gsSP1Triangle(6,7,8,0),
	gsSP1Triangle(2,9,6,0),
	gsSP1Triangle(5,10,11,0),
	gsSP1Triangle(4,10,5,0),
	gsSP1Triangle(8,12,13,0),
	gsSP1Triangle(5,11,1,0),
	gsSP1Triangle(6,0,2,0),
	gsSP1Triangle(10,9,2,0),
	gsSP1Triangle(3,14,4,0),
	gsSPVertex(&vtx_ping_base5[15],15,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(3,4,5,0),
	gsSP1Triangle(6,7,8,0),
	gsSP1Triangle(9,10,0,0),
	gsSP1Triangle(11,8,12,0),
	gsSP1Triangle(9,5,4,0),
	gsSP1Triangle(2,13,14,0),
	gsSP1Triangle(1,11,12,0),
	gsSP1Triangle(13,2,1,0),
	gsSP1Triangle(6,11,1,0),
	gsSP1Triangle(0,2,5,0),
	gsSP1Triangle(0,10,6,0),
	gsSP1Triangle(6,1,0,0),
	gsSP1Triangle(5,9,0,0),
	gsSP1Triangle(1,12,13,0),
	gsSPVertex(&vtx_ping_base5[30],4,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(3,1,0,0),
	gsSPLight((&light_ping_base[3].l[0]),1),
	gsSPLight((&light_ping_base[3].a),2),
	gsSPVertex(&vtx_ping_base5[34],14,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(3,4,0,0),
	gsSP1Triangle(5,4,6,0),
	gsSP1Triangle(7,0,8,0),
	gsSP1Triangle(6,4,3,0),
	gsSP1Triangle(9,10,1,0),
	gsSP1Triangle(10,11,1,0),
	gsSP1Triangle(1,0,9,0),
	gsSP1Triangle(9,5,10,0),
	gsSP1Triangle(12,10,13,0),
	gsSP1Triangle(2,1,11,0),
	gsSP1Triangle(2,11,12,0),
	gsSP1Triangle(0,4,9,0),
	gsSP1Triangle(2,8,0,0),
	gsSP1Triangle(12,11,10,0),
	gsSP1Triangle(0,7,3,0),
	gsSP1Triangle(5,9,4,0),
	gsSP1Triangle(13,10,5,0),
	gsSPEndDisplayList()
};

static Gfx gfx_ping_head_1[] = {
	gs_Tani_LoadTextureImage2(ping_mouth_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPLight((&light_ping_head[1].l[0]),1 ),
	gsSPLight((&light_ping_head[1].a   ),2 ),
	gsSPVertex(&vtx_ping_head_1[0],  6, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 4, 5, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_ping_head_2[] = {
	gsSPLight((&light_ping_head[2].l[0]),1 ),
	gsSPLight((&light_ping_head[2].a   ),2 ),
	gsSPVertex(&vtx_ping_head_2[0],  3, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSPLight((&light_ping_head[0].l[0]),1 ),
	gsSPLight((&light_ping_head[0].a   ),2 ),
	gsSPVertex(&vtx_ping_head_2[3], 16, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 4, 1, 0),
	gsSP1Triangle( 3, 1, 5, 0),
	gsSP1Triangle( 4, 2, 1, 0),
	gsSP1Triangle( 1, 6, 5, 0),
	gsSP1Triangle( 0, 6, 1, 0),
	gsSP1Triangle( 0, 7, 6, 0),
	gsSP1Triangle( 5, 6, 7, 0),
	gsSP1Triangle( 8, 9, 5, 0),
	gsSP1Triangle( 7, 8, 5, 0),
	gsSP1Triangle( 3, 5, 9, 0),
	gsSP1Triangle( 0, 2,10, 0),
	gsSP1Triangle(11, 7, 0, 0),
	gsSP1Triangle( 0,10,11, 0),
	gsSP1Triangle(12, 3, 9, 0),
	gsSP1Triangle(12, 4, 3, 0),
	gsSP1Triangle(13,14,15, 0),
	gsSP1Triangle(12, 9,13, 0),
	gsSP1Triangle( 9,14,13, 0),
	gsSP1Triangle( 9, 8,14, 0),
	gsSPVertex(&vtx_ping_head_2[19], 14, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 2, 3, 4, 0),
	gsSP1Triangle( 2, 4, 0, 0),
	gsSP1Triangle( 5, 6, 4, 0),
	gsSP1Triangle( 6, 0, 4, 0),
	gsSP1Triangle( 4, 3, 5, 0),
	gsSP1Triangle( 1, 0, 7, 0),
	gsSP1Triangle( 0, 6, 7, 0),
	gsSP1Triangle( 8, 1, 7, 0),
	gsSP1Triangle( 9, 7, 6, 0),
	gsSP1Triangle( 7, 9, 8, 0),
	gsSP1Triangle( 9,10,11, 0),
	gsSP1Triangle( 9, 6,10, 0),
	gsSP1Triangle( 8, 9,11, 0),
	gsSP1Triangle( 5,10, 6, 0),
	gsSP1Triangle(10,12,11, 0),
	gsSP1Triangle( 5,13,10, 0),
	gsSPEndDisplayList() 
};

/* ========================================================================================	
		: ping_head each eye and its common gfx.
=========================================================================================== */
static Gfx gfx_ping_head_0_common[] = {
	gsSPLight((&light_ping_head[0].l[0]),1 ),
	gsSPLight((&light_ping_head[0].a   ),2 ),
	gsSPVertex(&vtx_ping_head_0[0],  7, 0), 
	gsSP1Triangle( 0, 1, 2, 0),
	gsSP1Triangle( 3, 1, 0, 0),
	gsSP1Triangle( 4, 5, 0, 0),
	gsSP1Triangle( 0, 5, 6, 0),
	gsSPEndDisplayList() 
};

static Gfx gfx_ping_head_0_1[] = {
	gs_Tani_LoadTextureImage2(ping_eye1_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPDisplayList(gfx_ping_head_0_common),
	gsSPEndDisplayList() 
};

static Gfx gfx_ping_head_0_2[] = {
	gs_Tani_LoadTextureImage2(ping_eye2_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPDisplayList(gfx_ping_head_0_common),
	gsSPEndDisplayList() 
};

static Gfx gfx_ping_head_0_3[] = {
	gs_Tani_LoadTextureImage2(ping_eye3_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPDisplayList(gfx_ping_head_0_common),
	gsSPEndDisplayList() 
};

static Gfx gfx_ping_head_0_4[] = {
	gs_Tani_LoadTextureImage2(ping_eye4_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPDisplayList(gfx_ping_head_0_common),
	gsSPEndDisplayList() 
};

static Gfx gfx_ping_head_0_5[] = {
	gs_Tani_LoadTextureImage2(ping_eye5_txt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, 7),
	gsSPDisplayList(gfx_ping_head_0_common),
	gsSPEndDisplayList() 
};

/* ========================================================================================	
		: ping_head initisalize 2 gfxs.
=========================================================================================== */
static Gfx gfx_ping_head0_initA[] = {
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_BLENDRGBA, G_CC_BLENDRGBA),
	gsDPSetTile(G_IM_FMT_RGBA,G_IM_SIZ_16b, 0, 0, 7, 0,  0,0,0, 0,0,0),
	gsSPTexture(0xffff,0xffff, 0, 0, G_ON),
	gs_Tani_SetUpTileDescrip(G_IM_FMT_RGBA,G_IM_SIZ_16b, 32, 32,  0, 0, G_TX_CLAMP|G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_CLAMP|G_TX_NOMIRROR, 5, G_TX_NOLOD),
	gsSPEndDisplayList() 
};

static Gfx gfx_ping_head0_initB[] = {
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_MODULATERGB, G_CC_MODULATERGB),
	gs_Tani_SetUpTileDescrip(G_IM_FMT_RGBA,G_IM_SIZ_16b, 32, 32,  0, 0, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD, G_TX_WRAP|G_TX_NOMIRROR, 5, G_TX_NOLOD),
	gsSPDisplayList(gfx_ping_head_1),
	gsSPTexture(0xffff, 0xffff, 0, 0, G_OFF),
	gsDPPipeSync(),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPDisplayList(gfx_ping_head_2),
	gsSPEndDisplayList() 
};

/* ========================================================================================	
		: ping_head main 5 gfxs.
=========================================================================================== */
const Gfx RCP_ping_head_eye1[] = {
	gsSPDisplayList(gfx_ping_head0_initA),
	gsSPDisplayList(gfx_ping_head_0_1),
	gsSPDisplayList(gfx_ping_head0_initB),
	gsSPEndDisplayList() 
};

const Gfx RCP_ping_head_eye2[] = {
	gsSPDisplayList(gfx_ping_head0_initA),
	gsSPDisplayList(gfx_ping_head_0_2),
	gsSPDisplayList(gfx_ping_head0_initB),
	gsSPEndDisplayList() 
};

const Gfx RCP_ping_head_eye3[] = {
	gsSPDisplayList(gfx_ping_head0_initA),
	gsSPDisplayList(gfx_ping_head_0_3),
	gsSPDisplayList(gfx_ping_head0_initB),
	gsSPEndDisplayList() 
};

const Gfx RCP_ping_head_eye4[] = {
	gsSPDisplayList(gfx_ping_head0_initA),
	gsSPDisplayList(gfx_ping_head_0_4),
	gsSPDisplayList(gfx_ping_head0_initB),
	gsSPEndDisplayList() 
};

const Gfx RCP_ping_head_eye5[] = {
	gsSPDisplayList(gfx_ping_head0_initA),
	gsSPDisplayList(gfx_ping_head_0_5),
	gsSPDisplayList(gfx_ping_head0_initB),
	gsSPEndDisplayList() 
};
#pragma GCC diagnostic pop
