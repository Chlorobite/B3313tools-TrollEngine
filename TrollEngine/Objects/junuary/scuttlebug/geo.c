#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "geo_commands_leek.h"
#include "make_const_nonconst.h"

extern Gfx light_sourcex[];
extern Gfx RCP_walker_F_walk0[];
extern Gfx RCP_walker_F_walk1[];
extern Gfx RCP_walker_F_walk2[];
extern Gfx RCP_walker_F_walk6[];
extern Gfx RCP_walker_F_walk14[];
extern Gfx RCP_walker_F_walk15[];

const GeoLayout scuttlebug_geo[] = {
   // idiotic padding ahahdh
   0x01000000,
   
hmsShadow(100,150,1)
hmsBegin()
  hmsScale(0.25f)
  hmsBegin()
    GEO_RENDER_RANGE(-2048, 3313),
    hmsBegin()
      hmsJoint(RM_SURF,light_sourcex,0,0,0)  /* chn8(39) */
      hmsBegin()
         hmsJoint(RM_SURF,NULL,0,0,0)  /* jnt8_1(38) */
         hmsBegin()
            hmsJoint(RM_SURF,NULL,-1,113,112)  /* chn1(37) */
            hmsBegin()
               hmsJoint(RM_SURF,NULL,0,0,0)  /* jnt1_1(27) */
               hmsBegin()
                  hmsJoint(RM_SURF,NULL,198,0,0)  /* jnt1_2(23) */
                  hmsBegin()
                     hmsJoint(RM_SURF,NULL,195,0,0)  /* jnt1_3(17) */
                     hmsBegin()
                        hmsJoint(RM_SURF,RCP_walker_F_walk0,0,0,0)  /* w_footA(0) */
                     hmsEnd()
                     hmsJoint(RM_SURF,RCP_walker_F_walk1,0,0,0)  /* w_legA2(1) */
                  hmsEnd()
                  hmsJoint(RM_SURF,RCP_walker_F_walk2,0,0,0)  /* w_legA1(2) */
               hmsEnd()
            hmsEnd()
            hmsJoint(RM_SURF,NULL,-1,109,-116)  /* chn3(36) */
            hmsBegin()
               hmsJoint(RM_SURF,NULL,0,0,0)  /* jnt3_1(28) */
               hmsBegin()
                  hmsJoint(RM_SURF,NULL,198,0,0)  /* jnt3_2(24) */
                  hmsBegin()
                     hmsJoint(RM_SURF,NULL,195,0,0)  /* jnt3_3(18) */
                     hmsBegin()
                        hmsJoint(RM_SURF,RCP_walker_F_walk0,0,0,0)  /* w_footB(3) */
                     hmsEnd()
                     hmsJoint(RM_SURF,RCP_walker_F_walk1,0,0,0)  /* w_legB2(4) */
                  hmsEnd()
                  hmsJoint(RM_SURF,RCP_walker_F_walk2,0,0,0)  /* w_legB1(5) */
               hmsEnd()
            hmsEnd()
            hmsJoint(RM_SURF,RCP_walker_F_walk15,0,0,0)  /* w_body(35) */
            hmsJoint(RM_SURF,NULL,-29,-57,28)  /* chn9(34) */
            hmsBegin()
               hmsJoint(RM_SURF,NULL,0,0,0)  /* jnt9_1(19) */
               hmsBegin()
                  hmsJoint(RM_SURF,NULL,158,0,0)  /* eff9(15) */
                  hmsBegin()
                     hmsJoint(RM_SURF,RCP_walker_F_walk6,0,0,0)  /* w_eyeL(6) */
                  hmsEnd()
               hmsEnd()
            hmsEnd()
            hmsJoint(RM_SURF,NULL,-29,55,28)  /* chn10(33) */
            hmsBegin()
               hmsJoint(RM_SURF,NULL,0,0,0)  /* jnt10_1(20) */
               hmsBegin()
                  hmsJoint(RM_SURF,NULL,157,0,0)  /* eff10(16) */
                  hmsBegin()
                     hmsJoint(RM_SURF,RCP_walker_F_walk6,0,0,0)  /* w_eyeR(7) */
                  hmsEnd()
               hmsEnd()
            hmsEnd()
            hmsJoint(RM_SURF,NULL,-1,-116,-111)  /* chn12(32) */
            hmsBegin()
               hmsJoint(RM_SURF,NULL,0,0,0)  /* jnt12_1(29) */
               hmsBegin()
                  hmsJoint(RM_SURF,NULL,198,0,0)  /* jnt12_2(25) */
                  hmsBegin()
                     hmsJoint(RM_SURF,NULL,195,0,0)  /* jnt12_3(21) */
                     hmsBegin()
                        hmsJoint(RM_SURF,RCP_walker_F_walk0,0,0,0)  /* w_footC(8) */
                     hmsEnd()
                     hmsJoint(RM_SURF,RCP_walker_F_walk1,0,0,0)  /* w_legC2(9) */
                  hmsEnd()
                  hmsJoint(RM_SURF,RCP_walker_F_walk2,0,0,0)  /* w_legC1(10) */
               hmsEnd()
            hmsEnd()
            hmsJoint(RM_SURF,NULL,-1,-116,112)  /* chn5(31) */
            hmsBegin()
               hmsJoint(RM_SURF,NULL,0,0,0)  /* jnt5_1(30) */
               hmsBegin()
                  hmsJoint(RM_SURF,NULL,195,0,0)  /* jnt5_2(26) */
                  hmsBegin()
                     hmsJoint(RM_SURF,NULL,199,0,0)  /* jnt5_3(22) */
                     hmsBegin()
                        hmsJoint(RM_SURF,RCP_walker_F_walk0,0,0,0)  /* w_footD(11) */
                     hmsEnd()
                     hmsJoint(RM_SURF,RCP_walker_F_walk1,0,0,0)  /* w_legD2(12) */
                  hmsEnd()
                  hmsJoint(RM_SURF,RCP_walker_F_walk2,0,0,0)  /* w_legD1(13) */
               hmsEnd()
            hmsEnd()
            hmsJoint(RM_SURF,RCP_walker_F_walk14,0,0,0)  /* w_eyebrow(14) */
 hmsExit()
};
