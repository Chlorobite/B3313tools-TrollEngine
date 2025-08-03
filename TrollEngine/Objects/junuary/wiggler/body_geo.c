#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "geo_commands_leek.h"
#include "make_const_nonconst.h"

extern Gfx RCP_hana_body_walk6[];
extern Gfx wiggler_seg5_dl_0500BE98[];
extern Gfx wiggler_seg5_dl_0500BE10[];
extern Gfx wiggler_seg5_dl_0500BCB8[];
extern Gfx wiggler_seg5_dl_0500C100[];
extern Gfx wiggler_seg5_dl_0500C078[];
extern Gfx wiggler_seg5_dl_0500BF20[];

// 0x0500C778
const GeoLayout wiggler_body_geo[] = {
  // padding
  0x0500C6DC, 0x00000000,
  
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x96, 0x64),
    GEO_OPEN_NODE(),
      GEO_SCALE(0, 16384),
      GEO_OPEN_NODE(),
        GEO_ANIMATED_PART(1, 0, 0, 0, NULL),
        GEO_OPEN_NODE(),
         GEO_ANIMATED_PART(1, 0, 0, 0, RCP_hana_body_walk6),
          GEO_OPEN_NODE(),
            GEO_ANIMATED_PART(1, -31, 0, 51, NULL),
            GEO_OPEN_NODE(),
              GEO_ANIMATED_PART(1, 0, 0, 0, wiggler_seg5_dl_0500BE98),
              GEO_OPEN_NODE(),
                GEO_ANIMATED_PART(1, 39, 0, 0, wiggler_seg5_dl_0500BE10),
                GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(1, 41, 0, 0, wiggler_seg5_dl_0500BCB8),
                GEO_CLOSE_NODE(),
              GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
            GEO_ANIMATED_PART(1, -31, 0, -49, NULL),
            GEO_OPEN_NODE(),
              GEO_ANIMATED_PART(1, 0, 0, 0, wiggler_seg5_dl_0500C100),
              GEO_OPEN_NODE(),
                GEO_ANIMATED_PART(1, 39, 0, 0, wiggler_seg5_dl_0500C078),
                GEO_OPEN_NODE(),
                  GEO_ANIMATED_PART(1, 41, 0, 0, wiggler_seg5_dl_0500BF20),
                GEO_CLOSE_NODE(),
              GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
          GEO_CLOSE_NODE(),
        GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};
