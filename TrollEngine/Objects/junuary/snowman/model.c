#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "geo_commands_leek.h"
#include "make_const_nonconst.h"

// Snowman

// Unreferenced light group
static const Lights1 snowman_lights_unused1 = gdSPDefLights1(
    0x35/2, 0x32/2, 0xa2/2,
    0x35, 0x32, 0xa2, 0x28, 0x28, 0x28
);

// Unreferenced light group
static const Lights1 snowman_lights_unused3 = gdSPDefLights1(
    0x23/2, 0x1c/2, 0x12/2,
    0x23, 0x1c, 0x12, 0x28, 0x28, 0x28
);

// 0x05009470
ALIGNED8 static const u8 snowman_seg5_texture_05009470[] = {
#include "mr_blizzard_left_side.rgba16.inc.c"
};

// 0x0500A470
ALIGNED8 static const u8 snowman_seg5_texture_0500A470[] = {
#include "mr_blizzard_right_side.rgba16.inc.c"
};

// 0x0500B470
ALIGNED8 static const u8 snowman_seg5_texture_0500B470[] = {
#include "mr_blizzard_eye.rgba16.inc.c"
};

// 0x0500BC70
ALIGNED8 static const u8 snowman_seg5_texture_0500BC70[] = {
#include "mr_blizzard_mouth.rgba16.inc.c"
};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"
// 0x0500C488
static const Vtx snowman_seg5_vertex_0500C488[] = {
	{-24,1,-6,0,0,0,-19,105,68,0},
	{74,14,14,0,0,0,-17,125,6,0},
	{74,16,-24,0,0,0,-17,125,6,0},
	{-24,1,-6,0,0,0,-14,5,-125,255},
	{74,16,-24,0,0,0,-10,-57,-112,255},
	{75,-17,-6,0,0,0,-10,-57,-112,255},
	{-24,1,-6,0,0,0,-13,-112,57,255},
	{75,-17,-6,0,0,0,-12,-68,105,255},
	{74,14,14,0,0,0,-12,-68,105,255},
};

// 0x0500C4C8 - 0x0500C500
const Gfx snowman_seg5_dl_0500C4C8[] = {
    gsSPLight(&snowman_lights_unused3.l, 1),
    gsSPLight(&snowman_lights_unused3.a, 2),
	gsSPVertex(&snowman_seg5_vertex_0500C488[0],9,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(3,4,5,0),
	gsSP1Triangle(6,7,8,0),
    gsSPEndDisplayList(),
};

// 0x0500C500 - 0x0500C530
const Gfx snowman_seg5_dl_0500C500[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPDisplayList(snowman_seg5_dl_0500C4C8),
    gsSPEndDisplayList(),
};

// 0x0500C530
static const Vtx snowman_seg5_vertex_0500C530[] = {
    {{{     0,     76,      0}, 0, {   990,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -76,    -76,      0}, 0, {     0,   2012}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,    -76,      0}, 0, {   990,   2012}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -76,     76,      0}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
};

// 0x0500C570
static const Vtx snowman_seg5_vertex_0500C570[] = {
    {{{    76,     76,      0}, 0, {   990,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,    -76,      0}, 0, {     0,   2012}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    76,    -76,      0}, 0, {   990,   2012}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,     76,      0}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
};

// 0x0500C5B0 - 0x0500C5E8
const Gfx snowman_seg5_dl_0500C5B0[] = {
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, snowman_seg5_texture_05009470),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 64 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPVertex(snowman_seg5_vertex_0500C530, 4, 0),
    gsSP2Triangles( 0,  1,  2, 0x0,  0,  3,  1, 0x0),
    gsSPEndDisplayList(),
};

// 0x0500C5E8 - 0x0500C620
const Gfx snowman_seg5_dl_0500C5E8[] = {
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, snowman_seg5_texture_0500A470),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 64 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPVertex(snowman_seg5_vertex_0500C570, 4, 0),
    gsSP2Triangles( 0,  1,  2, 0x0,  0,  3,  1, 0x0),
    gsSPEndDisplayList(),
};

// 0x0500C620 - 0x0500C698
const Gfx snowman_seg5_dl_0500C620[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (64 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPDisplayList(snowman_seg5_dl_0500C5B0),
    gsSPDisplayList(snowman_seg5_dl_0500C5E8),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

// 0x0500C698
static const Vtx snowman_seg5_vertex_0500C698[] = {
    {{{    69,    191,   -132}, 0, {  -370,   -440}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -63,    230,      0}, 0, {  1472,   1328}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    69,    230,      0}, 0, {  1472,   -440}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -63,    191,   -132}, 0, {  -370,   1328}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    69,    230,      0}, 0, {  -370,   -440}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -63,    191,    133}, 0, {  1472,   1328}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    69,    191,    133}, 0, {  1472,   -440}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -63,    230,      0}, 0, {  -370,   1328}, {0xff, 0xff, 0xff, 0xff}}},
};

// 0x0500C718 - 0x0500C760
const Gfx snowman_seg5_dl_0500C718[] = {
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, snowman_seg5_texture_0500B470),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPVertex(snowman_seg5_vertex_0500C698, 8, 0),
    gsSP2Triangles( 0,  1,  2, 0x0,  0,  3,  1, 0x0),
    gsSP2Triangles( 4,  5,  6, 0x0,  4,  7,  5, 0x0),
    gsSPEndDisplayList(),
};

// 0x0500C760 - 0x0500C7D0
const Gfx snowman_seg5_dl_0500C760[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPDisplayList(snowman_seg5_dl_0500C718),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

static Vtx vtx_snowman_ball_standby0[]={
	{96,15,-36,0,0,0,15,41,-118,0},
	{-3,44,-15,0,0,0,-44,71,-94,0},
	{32,81,26,0,0,0,-5,90,-89,0},
	{95,-31,-36,0,0,0,32,-34,-117,255},
	{-5,3,-29,0,0,0,-11,9,-126,255},
	{3,-44,-25,0,0,0,-10,-63,-109,255},
	{103,-63,-17,0,0,0,51,-54,-102,255},
	{115,-101,10,0,0,0,27,-73,-99,255},
	{68,-56,35,0,0,0,5,-23,124,255},
	{57,-20,35,0,0,0,2,-8,126,255},
	{6,0,27,0,0,0,-94,-13,83,255},
	{105,62,-3,0,0,0,20,74,-100,255},
	{32,81,26,0,0,0,-50,20,114,255},
	{127,-61,8,0,0,0,36,-22,119,255},
	{127,-61,8,0,0,0,80,-48,-85,255},
	{35,-86,11,0,0,0,-64,-56,93,255},
	{115,-101,10,0,0,0,21,-42,117,0},
	{68,-56,35,0,0,0,5,-23,124,0},
	{127,-61,8,0,0,0,36,-22,119,255},
	{206,-42,24,0,0,0,2,-39,120,255},
	{57,-20,35,0,0,0,2,-8,126,255},
	{130,89,31,0,0,0,0,3,127,255},
	{213,41,28,0,0,0,7,-2,126,255},
	{32,81,26,0,0,0,-5,90,-89,255},
	{130,89,31,0,0,0,19,92,-84,255},
	{105,62,-3,0,0,0,20,74,-100,255},
	{115,-101,10,0,0,0,27,-73,-99,255},
	{35,-86,11,0,0,0,-17,-90,-87,255},
	{3,-44,-25,0,0,0,-10,-63,-109,255},
	{127,-61,8,0,0,0,80,-48,-85,255},
	{103,-63,-17,0,0,0,51,-54,-102,0},
	{95,-31,-36,0,0,0,32,-34,-117,0},
	{68,-56,35,0,0,0,5,-23,124,255},
	{6,0,27,0,0,0,-94,-13,83,255},
	{35,-86,11,0,0,0,-64,-56,93,255},
	{57,-20,35,0,0,0,2,-8,126,255},
	{130,89,31,0,0,0,0,3,127,255},
	{32,81,26,0,0,0,-50,20,114,255},
	{-3,44,-15,0,0,0,-105,35,60,255},
	{3,-44,-25,0,0,0,-117,-32,34,255},
	{-5,3,-29,0,0,0,-123,-16,23,255},
	{-3,44,-15,0,0,0,-118,10,43,255},
	{213,41,28,0,0,0,50,52,-104,255},
	{105,62,-3,0,0,0,20,74,-100,255},
	{130,89,31,0,0,0,19,92,-84,255},
	{96,15,-36,0,0,0,15,41,-118,255},
	{105,62,-3,0,0,0,20,74,-100,0},
	{213,41,28,0,0,0,50,52,-104,0},
	{206,-42,24,0,0,0,56,-22,-111,255},
	{95,-31,-36,0,0,0,32,-34,-117,255},
	{127,-61,8,0,0,0,80,-48,-85,255},
};
#pragma GCC diagnostic pop

const Gfx snowman_seg5_dl_0500CAA8[]={
	gsDPPipeSync(),
	gsSPLight(&snowman_lights_unused1.l, 1),
	gsSPLight(&snowman_lights_unused1.a, 2),
	gsSPVertex(&vtx_snowman_ball_standby0[0],15,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(0,3,4,0),
	gsSP1Triangle(5,6,7,0),
	gsSP1Triangle(8,9,10,0),
	gsSP1Triangle(2,11,0,0),
	gsSP1Triangle(12,10,9,0),
	gsSP1Triangle(0,4,1,0),
	gsSP1Triangle(9,8,13,0),
	gsSP1Triangle(5,4,3,0),
	gsSP1Triangle(3,6,5,0),
	gsSP1Triangle(14,7,6,0),
	gsSPVertex(&vtx_snowman_ball_standby0[15],14,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(3,4,5,0),
	gsSP1Triangle(6,5,4,0),
	gsSP1Triangle(6,4,7,0),
	gsSP1Triangle(8,9,10,0),
	gsSP1Triangle(11,12,13,0),
	gsSP1Triangle(2,1,3,0),
	gsSPVertex(&vtx_snowman_ball_standby0[29],16,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(3,4,5,0),
	gsSP1Triangle(6,7,8,0),
	gsSP1Triangle(8,9,4,0),
	gsSP1Triangle(4,10,5,0),
	gsSP1Triangle(11,10,4,0),
	gsSP1Triangle(4,12,11,0),
	gsSP1Triangle(13,14,15,0),
	gsSPVertex(&vtx_snowman_ball_standby0[45],6,0),
	gsSP1Triangle(0,1,2,0),
	gsSP1Triangle(0,2,3,0),
	gsSP1Triangle(0,3,4,0),
	gsSP1Triangle(4,3,5,0),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsSPEndDisplayList()
};

// 0x0500CB08
static const Vtx snowman_seg5_vertex_0500CB08[] = {
    {{{     0,     57,      0}, 0, {   990,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -57,    -57,      0}, 0, {     0,   2012}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,    -57,      0}, 0, {   990,   2012}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -57,     57,      0}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
};

// 0x0500CB48
static const Vtx snowman_seg5_vertex_0500CB48[] = {
    {{{    57,     57,      0}, 0, {   990,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,    -57,      0}, 0, {     0,   2012}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    57,    -57,      0}, 0, {   990,   2012}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,     57,      0}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
};

// 0x0500CB88 - 0x0500CBC0
const Gfx snowman_seg5_dl_0500CB88[] = {
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, snowman_seg5_texture_05009470),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 64 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPVertex(snowman_seg5_vertex_0500CB08, 4, 0),
    gsSP2Triangles( 0,  1,  2, 0x0,  0,  3,  1, 0x0),
    gsSPEndDisplayList(),
};

// 0x0500CBC0 - 0x0500CBF8
const Gfx snowman_seg5_dl_0500CBC0[] = {
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, snowman_seg5_texture_0500A470),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 64 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPVertex(snowman_seg5_vertex_0500CB48, 4, 0),
    gsSP2Triangles( 0,  1,  2, 0x0,  0,  3,  1, 0x0),
    gsSPEndDisplayList(),
};

// 0x0500CBF8 - 0x0500CC70
const Gfx snowman_seg5_dl_0500CBF8[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (64 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPDisplayList(snowman_seg5_dl_0500CB88),
    gsSPDisplayList(snowman_seg5_dl_0500CBC0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

// 0x0500CC70
static const Vtx snowman_seg5_vertex_0500CC70[] = {
    {{{   -90,    210,    -56}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{  -134,    182,    -56}, 0, {     0,    990}, {0xff, 0xff, 0xff, 0xff}}},
    {{{  -134,    182,     57}, 0, {   990,    990}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -90,    210,     57}, 0, {   990,      0}, {0xff, 0xff, 0xff, 0xff}}},
};

// 0x0500CCB0 - 0x0500CCE8
const Gfx snowman_seg5_dl_0500CCB0[] = {
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, snowman_seg5_texture_0500BC70),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 32 * 32 - 1, CALC_DXT(32, G_IM_SIZ_16b_BYTES)),
    gsSPVertex(snowman_seg5_vertex_0500CC70, 4, 0),
    gsSP2Triangles( 0,  1,  2, 0x0,  0,  2,  3, 0x0),
    gsSPEndDisplayList(),
};

// 0x0500CCE8 - 0x0500CD58
const Gfx snowman_seg5_dl_0500CCE8[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (32 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPDisplayList(snowman_seg5_dl_0500CCB0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};
