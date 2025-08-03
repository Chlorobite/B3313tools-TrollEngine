#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "geo_commands_leek.h"
#include "make_const_nonconst.h"

extern Gfx RCP_hana_body_walk6[];
extern Gfx RCP_hana_head_walk10[];
extern Gfx RCP_hana_head_walk6[];
extern Gfx wiggler_seg5_dl_0500E518[];
extern Gfx wiggler_seg5_dl_0500E430[];
extern Gfx wiggler_seg5_dl_0500E2F8[];
extern Gfx wiggler_seg5_dl_0500E060[];
extern Gfx wiggler_seg5_dl_0500DFD8[];
extern Gfx wiggler_seg5_dl_0500DE80[];
extern Gfx wiggler_seg5_dl_0500DDF8[];
extern Gfx wiggler_seg5_dl_0500DD70[];
extern Gfx wiggler_seg5_dl_0500DC18[];


// 0x0C000030
const GeoLayout wiggler_head_geo[] = {
   GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x96, 100),
   GEO_OPEN_NODE(),
      GEO_SCALE(0x00, 16384),
      GEO_OPEN_NODE(),
         GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
         GEO_OPEN_NODE(),
          GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, RCP_hana_body_walk6),
            GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 157, 0, 0, RCP_hana_head_walk10),
               GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 143, 0, 0, RCP_hana_head_walk6),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 143, 0, 0, NULL),
               GEO_CLOSE_NODE(),
               GEO_ANIMATED_PART(LAYER_ALPHA, 157, 0, 0, wiggler_seg5_dl_0500E518),
               GEO_ANIMATED_PART(LAYER_ALPHA, 157, 0, 0, wiggler_seg5_dl_0500E430),
               GEO_ANIMATED_PART(LAYER_ALPHA, 157, 0, 0, wiggler_seg5_dl_0500E2F8),
               GEO_ANIMATED_PART(LAYER_OPAQUE, -16, 23, 51, NULL),
               GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, wiggler_seg5_dl_0500DDF8),
                  GEO_OPEN_NODE(),
                     GEO_ANIMATED_PART(LAYER_OPAQUE, 39, 0, 0, wiggler_seg5_dl_0500DD70),
                     GEO_OPEN_NODE(),
                        GEO_ANIMATED_PART(LAYER_OPAQUE, 41, 0, 0, wiggler_seg5_dl_0500DC18),
                     GEO_CLOSE_NODE(),
                  GEO_CLOSE_NODE(),
               GEO_CLOSE_NODE(),
               GEO_ANIMATED_PART(LAYER_OPAQUE, -16, 23, -49, NULL),
               GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, wiggler_seg5_dl_0500E060),
                  GEO_OPEN_NODE(),
                     GEO_ANIMATED_PART(LAYER_OPAQUE, 39, 0, 0, wiggler_seg5_dl_0500DFD8),
                     GEO_OPEN_NODE(),
                        GEO_ANIMATED_PART(LAYER_OPAQUE, 41, 0, 0, wiggler_seg5_dl_0500DE80),
                     GEO_CLOSE_NODE(),
                  GEO_CLOSE_NODE(),
               GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
