#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "geo_commands_leek.h"
#include "make_const_nonconst.h"

extern Gfx amp_seg8_dl_08003910[];
extern Gfx amp_seg8_dl_080039D0[];
extern Gfx amp_seg8_dl_08002D70[];
extern Gfx amp_seg8_dl_08002C88[];


// 0x0F000020
const GeoLayout amp_geo[] = {
   // padding troll
   0x00000000, 0x00000000,
   
   GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0xC8, 100),
   GEO_OPEN_NODE(),
      GEO_SCALE(0x00, 16384),
      GEO_OPEN_NODE(),
         GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
         GEO_OPEN_NODE(),
            GEO_ANIMATED_PART(LAYER_ALPHA, 0, 0, 0, amp_seg8_dl_08002C88),
            GEO_OPEN_NODE(),
               GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
               GEO_OPEN_NODE(),
                  GEO_SWITCH_CASE(2, geo_switch_anim_state),
                  GEO_OPEN_NODE(),
                     GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
                     GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, amp_seg8_dl_08003910),
                  GEO_CLOSE_NODE(),
               GEO_CLOSE_NODE(),
               GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
               GEO_OPEN_NODE(),
                  GEO_SWITCH_CASE(2, geo_switch_anim_state),
                  GEO_OPEN_NODE(),
                     GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
                     GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, amp_seg8_dl_08003910),
                  GEO_CLOSE_NODE(),
               GEO_CLOSE_NODE(),
               GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
               GEO_OPEN_NODE(),
                  GEO_SWITCH_CASE(2, geo_switch_anim_state),
                  GEO_OPEN_NODE(),
                     GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
                     GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, amp_seg8_dl_08003910),
                  GEO_CLOSE_NODE(),
               GEO_CLOSE_NODE(),
               GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
               GEO_OPEN_NODE(),
                  GEO_SWITCH_CASE(2, geo_switch_anim_state),
                  GEO_OPEN_NODE(),
                     GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
                     GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, amp_seg8_dl_08003910),
                  GEO_CLOSE_NODE(),
               GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
            GEO_ANIMATED_PART(LAYER_ALPHA, 0, 0, 0, amp_seg8_dl_08002D70),
            GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
            GEO_OPEN_NODE(),
               GEO_OPEN_NODE(),
                  GEO_DISPLAY_LIST(LAYER_OPAQUE, amp_seg8_dl_080039D0),
               GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
