#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "geo_commands_leek.h"
#include "make_const_nonconst.h"

extern Gfx RCP_omurobo_anim_run0[];
extern Gfx RCP_omurobo_anim_run1[];
extern Gfx RCP_omurobo_anim_run2[];
extern Gfx RCP_omurobo_anim_run3[];
extern Gfx RCP_omurobo_anim_run4[];
extern Gfx RCP_omurobo_anim_run5[];
extern Gfx RCP_omurobo_anim_run8[];
extern Gfx RCP_omurobo_anim_run9[];

extern Gfx *MotosProc1(s32 run, UNUSED struct GraphNode *node, Mat4 mtx);

const GeoLayout heave_ho_geo[] = {
   // padding shit aaaa
   0x05000000, 0x05000000, 0x01000000,
   
 hmsShadow(100,150,1)
 hmsBegin()
 hmsScale(0.25f)
    hmsBegin()
      hmsJoint(RM_SURF,NULL,0,0,0)
      hmsBegin()
         hmsJoint(RM_SURF,RCP_omurobo_anim_run9,0,0,0)  /* ,omu_bar,(14) */
         hmsBegin()
            hmsJoint(RM_SURF,NULL,189,-28,0)  /* ,chn4,(13) */
            hmsBegin()
               hmsJoint(RM_SURF,RCP_omurobo_anim_run8,0,0,0)  /* ,omu_ita,(10) */
               hmsJoint(RM_SURF,RCP_omurobo_anim_run5,0,0,0)  /* ,omu_Larm,(6) */
               hmsJoint(RM_SURF,RCP_omurobo_anim_run0,0,0,0)  /* ,omu_Rarm,(0) */
	           hmsBegin()
				   hmsCProg(0, MotosProc1)
    	       hmsEnd()
            hmsEnd()
            hmsJoint(RM_SURF,NULL,175,0,-84)  /* ,chn6,(12) */
            hmsBegin()
               hmsJoint(RM_SURF,RCP_omurobo_anim_run4,0,0,0)  /* ,sphere2,(8) */
               hmsBegin()
                  hmsJoint(RM_SURF,RCP_omurobo_anim_run4,168,0,0)  /* ,sphere1,(7) */
                  hmsJoint(RM_SURF,NULL,168,0,0)  /* ,eff6,(5) */
               hmsEnd()
            hmsEnd()
            hmsJoint(RM_SURF,NULL,180,-12,0)  /* ,chn7,(11) */
            hmsBegin()
               hmsJoint(RM_SURF,RCP_omurobo_anim_run1,0,0,0)  /* ,omu_body,(1) */
            hmsEnd()
            hmsJoint(RM_SURF,NULL,50,-36,92)  /* ,chn8,(9) */
            hmsBegin()
               hmsJoint(RM_SURF,RCP_omurobo_anim_run3,0,0,0)  /* ,omu_hatabar,(3) */
               hmsBegin()
                  hmsJoint(RM_SURF,RCP_omurobo_anim_run2,196,0,0)  /* ,omu_hata,(2) */
               hmsEnd()
            hmsEnd()
         hmsEnd()
         hmsJoint(RM_SURF,RCP_omurobo_anim_run4,0,0,0)  /* ,sphere3,(4) */
      hmsEnd()
   hmsEnd()
 hmsEnd()
 hmsExit()
};
