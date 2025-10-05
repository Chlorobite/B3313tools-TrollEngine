#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "geo_commands_leek.h"
#include "make_const_nonconst.h"

extern const Gfx wf_seg7_dl_0700E760[];
extern const Gfx wf_seg7_dl_0700E9B8[];
extern const Gfx wf_seg7_dl_plankeyes[];

const GeoLayout wf_geo_000A58[] = {
   GEO_CULLING_RADIUS(1500),
   GEO_OPEN_NODE(),
      GEO_RENDER_RANGE(-2048, 5000),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_OPAQUE, wf_seg7_dl_0700E760),
         GEO_DISPLAY_LIST(LAYER_ALPHA, wf_seg7_dl_plankeyes),
      GEO_CLOSE_NODE(),
      GEO_RENDER_RANGE(5000, 32767),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_OPAQUE, wf_seg7_dl_0700E9B8),
         GEO_DISPLAY_LIST(LAYER_ALPHA, wf_seg7_dl_plankeyes),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
