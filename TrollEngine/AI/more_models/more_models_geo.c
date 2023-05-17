#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#define hmsShadow(a,b,c) GEO_SHADOW(c,b,a),
#define hmsBegin() GEO_OPEN_NODE(),
#define hmsTrans(x,y,z) GEO_TRANSLATE(0x00, x,y,z),
#define hmsScale(s) GEO_SCALE(0x00, (u32)(s * 65536)),
#define hmsJoint(l,dl,x,y,z) GEO_ANIMATED_PART(l, x, y, z, dl),
#define hmsEnd() GEO_CLOSE_NODE(),
#define hmsExit() GEO_END(),
#define hmsBboard(x,y,z) GEO_BILLBOARD(),
#define hmsGfx(l,dl) GEO_DISPLAY_LIST(l, dl),
#define hmsGroup() GEO_NODE_START(),
#define hmsCProg(arg,func) GEO_ASM(arg, func),
#define hmsHeader(radius) GEO_CULLING_RADIUS(radius),
#define hmsSelect(count, function) GEO_SWITCH_CASE(count, function),
#define   RM_SURF                         1
#define   RM_SPRITE                       4

/********************************************************************************
	RCP_HmsItemStar [ New New Version ]
														[ Nob 19, 1995 ]
 ********************************************************************************/

extern Gfx RCP_sstar1[];
extern Gfx RCP_sstar2[];
extern Gfx RCP_sstar3[];
extern Gfx RCP_sstar4[];
extern Gfx RCP_sstar5[];
extern Gfx RCP_sstar6[];
extern Gfx RCP_sstar7[];
extern Gfx RCP_sstar8[];

extern Gfx custom_door_1_dl_near[];
extern Gfx custom_door_1_dl_far[];
extern Gfx custom_door_2_dl_near[];
extern Gfx custom_door_2_dl_far[];
extern Gfx custom_door_3_dl_near[];
extern Gfx custom_door_3_dl_far[];
extern Gfx custom_door_4_dl_near[];
extern Gfx custom_door_4_dl_far[];

extern Gfx small_key_seg5_dl_05006A08[];


/********************************************************************************/
/*	Hierarchy data of star coin.												*/
/********************************************************************************/
const GeoLayout silver_star_geo[] = {
	hmsShadow(100, 155, 1)
	hmsBegin()
		hmsSelect(16, geo_switch_anim_state)
		hmsBegin()
			hmsGfx(RM_SPRITE, RCP_sstar1)
			hmsGfx(RM_SPRITE, RCP_sstar2)
			hmsGfx(RM_SPRITE, RCP_sstar3)
			hmsGfx(RM_SPRITE, RCP_sstar4)
			hmsGfx(RM_SPRITE, RCP_sstar5)
			hmsGfx(RM_SPRITE, RCP_sstar6)
			hmsGfx(RM_SPRITE, RCP_sstar7)
			hmsGfx(RM_SPRITE, RCP_sstar8)
	hmsExit()
};


// 0x160003A8
const GeoLayout custom_door_1_geo[] = {
   GEO_TRANSLATE_NODE(0x00, 78, 0, 0),
   GEO_OPEN_NODE(),
      GEO_SCALE(0x00, 16384),
      GEO_OPEN_NODE(),
         GEO_ANIMATED_PART(LAYER_OPAQUE, -300, 0, 0, NULL),
         GEO_OPEN_NODE(),
            GEO_OPEN_NODE(),
               GEO_RENDER_RANGE(-2048, 3500),
               GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, custom_door_1_dl_near),
               GEO_CLOSE_NODE(),
               GEO_RENDER_RANGE(3500, 32767),
               GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, custom_door_1_dl_far),
   GEO_END(),
};

// 0x160003A8
const GeoLayout custom_door_2_geo[] = {
   GEO_TRANSLATE_NODE(0x00, 78, 0, 0),
   GEO_OPEN_NODE(),
      GEO_SCALE(0x00, 16384),
      GEO_OPEN_NODE(),
         GEO_ANIMATED_PART(LAYER_OPAQUE, -300, 0, 0, NULL),
         GEO_OPEN_NODE(),
            GEO_OPEN_NODE(),
               GEO_RENDER_RANGE(-2048, 3500),
               GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, custom_door_2_dl_near),
               GEO_CLOSE_NODE(),
               GEO_RENDER_RANGE(3500, 32767),
               GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, custom_door_2_dl_far),
   GEO_END(),
};

// 0x160003A8
const GeoLayout custom_door_3_geo[] = {
   GEO_TRANSLATE_NODE(0x00, 78, 0, 0),
   GEO_OPEN_NODE(),
      GEO_SCALE(0x00, 16384),
      GEO_OPEN_NODE(),
         GEO_ANIMATED_PART(LAYER_OPAQUE, -300, 0, 0, NULL),
         GEO_OPEN_NODE(),
            GEO_OPEN_NODE(),
               GEO_RENDER_RANGE(-2048, 3500),
               GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, custom_door_3_dl_near),
               GEO_CLOSE_NODE(),
               GEO_RENDER_RANGE(3500, 32767),
               GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, custom_door_3_dl_far),
   GEO_END(),
};

// 0x160003A8
const GeoLayout custom_door_4_geo[] = {
   GEO_TRANSLATE_NODE(0x00, 78, 0, 0),
   GEO_OPEN_NODE(),
      GEO_SCALE(0x00, 16384),
      GEO_OPEN_NODE(),
         GEO_ANIMATED_PART(LAYER_OPAQUE, -300, 0, 0, NULL),
         GEO_OPEN_NODE(),
            GEO_OPEN_NODE(),
               GEO_RENDER_RANGE(-2048, 3500),
               GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, custom_door_4_dl_near),
               GEO_CLOSE_NODE(),
               GEO_RENDER_RANGE(3500, 32767),
               GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, custom_door_4_dl_far),
   GEO_END(),
};

const GeoLayout small_key_geoB[] = {
   GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x96, 80),
   GEO_OPEN_NODE(),
      GEO_SCALE(0x00, 16384),
      GEO_OPEN_NODE(),
         GEO_ROTATE_WITH_DL(LAYER_OPAQUE, 0, 0x4000, 0, 0x05006A08),
   GEO_END(),
};
