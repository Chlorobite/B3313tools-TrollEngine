#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "geo_commands_leek.h"
#include "make_const_nonconst.h"

extern Gfx RCP_wanwan_anim0[];
extern Gfx RCP_wanwan_anim1[];
extern Gfx RCP_wanwan_anim2[];
extern Gfx RCP_wanwan_anim3[];
extern Gfx RCP_wanwan_anim4[];

static const GeoLayout placeholder_for_alignment[] = {0, 0, 0};

// 0x0D0005EC
const GeoLayout chain_chomp_geo[] = {
 hmsShadow(200,150,1)
 hmsBegin()
 hmsScale(0.25f)
    hmsBegin()
      hmsJoint(RM_SURF,NULL,0,0,0)					/* ,XXX, */
      hmsBegin()
         hmsJoint(RM_SURF,RCP_wanwan_anim2,0,0,0)  /* ,wan_bodyB,(2) */
         hmsJoint(RM_SURF,RCP_wanwan_anim0,0,0,0)  /* ,wan_toothB,(0) */
      hmsEnd()
      hmsJoint(RM_SURF,NULL,0,0,0)  /* ,chn2,(5) */
      hmsBegin()
         hmsJoint(RM_SURF,RCP_wanwan_anim4,0,0,0)  /* ,wan_bodyA,(4) */
         hmsJoint(RM_SURF,RCP_wanwan_anim3,0,0,0)  /* ,wan_toothA,(3) */
         hmsJoint(LAYER_ALPHA,RCP_wanwan_anim1,0,0,0)  /* ,wan_eye,(1) */
      hmsEnd()
   hmsEnd()
 hmsEnd()
 hmsEnd()
 hmsExit()
};
