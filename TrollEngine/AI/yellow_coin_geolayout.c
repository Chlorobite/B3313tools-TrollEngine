#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "AI/personalized_geo.h"

#include "make_const_nonconst.h"

#include "actors/common1.h"

const GeoLayout yellow_coin_geo[] = {
   // funny alignment moment
   //GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0xB4, 50),
   0x00B40032,
   GEO_OPEN_NODE(),
      GEO_ASM(0, geo_yellow_coin),
   GEO_END(),
};
