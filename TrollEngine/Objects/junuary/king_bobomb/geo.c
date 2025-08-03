#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "geo_commands_leek.h"
#include "make_const_nonconst.h"

extern Gfx RCP_bom_king_carry3[];
extern Gfx RCP_bom_king_carry5[];
extern Gfx RCP_bom_king_carry6[];
extern Gfx RCP_bom_king_carry8[];
extern Gfx RCP_bom_king_carry10[];
extern Gfx RCP_bom_king_carry11[];

// 0x0C000000
const GeoLayout king_bobomb_geo[] = {
 hmsShadow(100,150,1)
 hmsBegin()
 hmsScale(0.25f)
    hmsBegin()
      hmsJoint(RM_SURF,NULL,0,0,0)		/* ,xxx,(xx) */
      hmsBegin()
         hmsJoint(RM_SURF,NULL,0,0,0)  /* ,jnt5_1,(28) */
         hmsBegin()
            hmsJoint(RM_SURF,RCP_bom_king_carry11,0,0,0)  /* ,king_body,(27) */
            hmsJoint(RM_SURF,NULL,0,-243,0)  /* ,chn2,(26) */
            hmsBegin()
               hmsJoint(RM_SURF,NULL,0,0,0)  /* ,jnt2_1,(16) */
               hmsBegin()
                  hmsJoint(RM_SURF,NULL,488,0,0)  /* ,jnt2_2,(12) */
                  hmsBegin()
                     hmsJoint(RM_SURF,NULL,110,0,0)  /* ,jnt2_3,(8) */
                     hmsBegin()
                        hmsJoint(RM_SURF,NULL,0,0,0)  /* ,king_Lfoot,(0) */
                     hmsEnd()
                     hmsJoint(RM_SURF,NULL,0,0,0)  /* ,king_Lleg,(1) */
                  hmsEnd()
               hmsEnd()
            hmsEnd()
            hmsJoint(RM_SPRITE,RCP_bom_king_carry10,0,0,0)  /* ,king_eye,(25) */
            hmsJoint(RM_SURF,RCP_bom_king_carry3,-700,0,0)  /* ,king_hige,(24) */
            hmsJoint(RM_SURF,RCP_bom_king_carry8,0,0,0)  /* ,king_crown,(23) */
            hmsJoint(RM_SURF,NULL,0,243,0)  /* ,chn4,(22) */
            hmsBegin()
               hmsJoint(RM_SURF,NULL,0,0,0)  /* ,jnt4_1,(17) */
               hmsBegin()
                  hmsJoint(RM_SURF,NULL,488,0,0)  /* ,jnt4_2,(13) */
                  hmsBegin()
                     hmsJoint(RM_SURF,NULL,110,0,0)  /* ,jnt4_3,(9) */
                     hmsBegin()
                        hmsJoint(RM_SURF,NULL,0,0,0)  /* ,king_Rfoot,(2) */
                     hmsEnd()
                     hmsJoint(RM_SURF,NULL,0,0,0)  /* ,king_Rleg,(3) */
                  hmsEnd()
               hmsEnd()
            hmsEnd()
            hmsJoint(RM_SURF,NULL,0,-1,36)  /* ,chn6,(21) */
            hmsBegin()
               hmsJoint(RM_SURF,NULL,0,0,0)  /* ,jnt6_1,(18) */
               hmsBegin()
                  hmsJoint(RM_SURF,NULL,548,0,0)  /* ,jnt6_2,(14) */
                  hmsBegin()
                     hmsJoint(RM_SURF,RCP_bom_king_carry6,0,0,0)  /* ,king_Larm,(10) */
                     hmsJoint(RM_SURF,NULL,189,0,0)  /* ,eff6,(6) */
                     hmsBegin()
                        hmsJoint(RM_SURF,RCP_bom_king_carry5,0,0,0)  /* ,king_Lhand,(4) */
		  				hmsCProg(0, geo_update_held_mario_pos)						
                     hmsEnd()
                  hmsEnd()
               hmsEnd()
            hmsEnd()
            hmsJoint(RM_SURF,NULL,0,1,36)  /* ,chn8,(20) */
            hmsBegin()
               hmsJoint(RM_SURF,NULL,0,0,0)  /* ,jnt8_1,(19) */
               hmsBegin()
                  hmsJoint(RM_SURF,NULL,548,0,0)  /* ,jnt8_2,(15) */
                  hmsBegin()
                     hmsJoint(RM_SURF,RCP_bom_king_carry6,0,0,0)  /* ,king_Rarm,(11) */
                     hmsJoint(RM_SURF,NULL,189,0,0)  /* ,eff8,(7) */
                     hmsBegin()
                        hmsJoint(RM_SURF,RCP_bom_king_carry5,0,0,0)  /* ,king_Rhand,(5) */
                     hmsEnd()
                  hmsEnd()
               hmsEnd()
            hmsEnd()
         hmsEnd()
      hmsEnd()
   hmsEnd()
 hmsEnd()
 hmsEnd()
 hmsExit()
};
