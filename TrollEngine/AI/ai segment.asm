; part 1 (0x80367460-0x80378800)
.headersize 0x80367460-0x10C280
.orga 0x10C280 ; rip blue gomba texture
.area 0x113A0,0x01

; absf and roundf
.definelabel absf, 0x80367460
JR         RA
ABS.S      F0, F12
.definelabel roundf, 0x80367468
ROUND.W.S  F0, F12
JR         RA
CVT.S.W    F0, F0

.importobj "AI/objects_collision.o"
.importobj "AI/personalization_helpers.o"
.importobj "AI/post_object_load_pass.o"
.importobj "AI/stats_tracking_debug.o"
.endarea

; part 2 (0x807E0000-0x807EC000)
.headersize 0x007E0000-0x188440
.orga 0x188440
.area 0xB000,0x01

.definelabel topbhv_funcs_start, 0x807E0000
.include "Objects/topbhv/funcs.asm"
.importobj "AI/more_models/more_models_levelscript.o"
.importobj "AI/more_models/more_models_geo.o"
.importobj "AI/more_models/more_models_models.o"
.endarea
; part 3 (0x807EC000-0x807F4000)
.headersize 0x807E0000-0x188440
.orga 0x188440+0xB000
.area 0x9000,0x01

.importobj "AI/stats_tracking.o"
.importobj "AI/audio_trolls.o"
.importobj "AI/frameskip_engine.o"
.importobj "AI/troll_hud.o"
.importobj "AI/yellow_coin_geo.o"
.importobj "AI/save_file.o"

.importobj "Trolls/cameratroll/sCamBeyta.o"

.importobj "AI/extra_dl_pools.o"

; object behaviors
.importobj "Objects/other_bhvs/aiseg_bhv.o"
.include "Objects/other_bhvs/aiseg_bhv_asm.asm"

.importobj "Objects/custom_purple_switch/script.o"
.importobj "Objects/gear_bhv/gear_bhv.o"
.importobj "Objects/blooper_bhv/bhv.o"
.importobj "Objects/beta_boo_key/beta_boo_key.o"

.importobj "Objects/beta_boo_key/boo_with_key.o"
.importobj "Objects/text/text.o"
.endarea


; Fix objects' collision and stuff
; FIND SEGMENT 13 AND SET THE ROM ADDRESS IN THE FILE BEFORE INCLUDING! OTHERWISE SHIT JUST FUCKING BREAKS
;.include "AI/ai objectsfix.asm"


.headersize 0x80245000
; arrays used by behaviors
; sSlidingPlatform2CollisionData
.org 0x80332B34
.word bits_seg7_collision_0701A9A0 & 0x00FFFFFF, bits_seg7_collision_0701AA0C & 0x00FFFFFF, bitfs_seg7_collision_07015714 & 0x00FFFFFF, bitfs_seg7_collision_07015768 & 0x00FFFFFF, rr_seg7_collision_070295F8 & 0x00FFFFFF, rr_seg7_collision_0702967C & 0x00FFFFFF, 0, bitdw_seg7_collision_0700F688 & 0x00FFFFFF
; sTumblingBridgeParams
.org 0x803302AC+0*0x0C+0x08
.word wf_seg7_collision_tumbling_bridge & 0x00FFFFFF
.org 0x803302AC+1*0x0C+0x08
.word bbh_seg7_collision_07026B1C & 0x00FFFFFF
.org 0x803302AC+2*0x0C+0x08
.word lll_seg7_collision_0701D21C & 0x00FFFFFF
.org 0x803302AC+3*0x0C+0x08
.word bitfs_seg7_collision_07015288 & 0x00FFFFFF
; sTTCRotatingSolidCollisionModels
.org 0x80332A70
.word ttc_seg7_collision_07014F70 & 0x00FFFFFF, ttc_seg7_collision_07015008 & 0x00FFFFFF
; sTTCPitBlockCollisionModels
.org 0x80332AB8
.word ttc_seg7_collision_07015754 & 0x00FFFFFF, ttc_seg7_collision_070157D8 & 0x00FFFFFF
; sTTCCogCollisionModels
.org 0x80332AA8
.word ttc_seg7_collision_07015584 & 0x00FFFFFF, ttc_seg7_collision_07015650 & 0x00FFFFFF
; sSeesawPlatformCollisionModels
.org 0x80332A00
.word bitdw_seg7_collision_0700F70C & 0x00FFFFFF, bits_seg7_collision_0701ADD8 & 0x00FFFFFF, bits_seg7_collision_0701AE5C & 0x00FFFFFF, bob_seg7_collision_bridge & 0x00FFFFFF, bitfs_seg7_collision_07015928 & 0x00FFFFFF, rr_seg7_collision_07029750 & 0x00FFFFFF, rr_seg7_collision_07029858 & 0x00FFFFFF, vcutm_seg7_collision_0700AC44 & 0x00FFFFFF
; sOctagonalPlatformCollision
.org 0x80332B54
.word bits_seg7_collision_0701AA84 & 0x00FFFFFF, rr_seg7_collision_07029508 & 0x00FFFFFF
; sWFRotatingPlatformData
.org 0x803301A8+0*0x0C+0x04
.word wf_seg7_collision_rotating_platform & 0x00FFFFFF
.org 0x803301A8+1*0x0C+0x04
.word wdw_seg7_collision_070186B4 & 0x00FFFFFF
; sFerrisWheelProperties
.org 0x80332A20+0*0x0C
.word bits_seg7_collision_0701ACAC & 0x00FFFFFF, bits_seg7_collision_0701AC28 & 0x00FFFFFF
.org 0x80332A20+1*0x0C
.word bitdw_seg7_collision_0700F7F0 & 0x00FFFFFF, bitdw_seg7_collision_0700F898 & 0x00FFFFFF
; sTTCTreadmillCollisionModels
.org 0x80332A8C
.word ttc_seg7_collision_070152B4 & 0x00FFFFFF, ttc_seg7_collision_070153E0 & 0x00FFFFFF
; sActivatedBackAndForthPlatformCollisionModels
.org 0x80332BE4
.word bits_seg7_collision_0701AD54 & 0x00FFFFFF, bitfs_seg7_collision_070157E0 & 0x00FFFFFF, bitfs_seg7_collision_07015124 & 0x00FFFFFF
; next are the staircases (D_80331A54) which might not happen ever since they also require 5 models each :tol:
; gOpenableGrills
.org 0x80330C48+0*0x08+0x04
.word bob_seg7_collision_gate & 0x00FFFFFF
.org 0x80330C48+1*0x08+0x04
.word hmc_seg7_collision_0702B65C & 0x00FFFFFF
; sPlatformOnTrackCollisionModels
.org 0x803329CC
.word rr_seg7_collision_07029038 & 0x00FFFFFF, ccm_seg7_collision_070163F8 & 0x00FFFFFF, 0x0800D710, bitfs_seg7_collision_070157E0 & 0x00FFFFFF
; sBowserFallingPlatform might also not happen



; Code to load stuff, including the AI segments
.orga 0x3A24 ; 80248A24
LUI     A1, 0x8034
SW      V0, 0xB060 (A1)
LW      A1, 0xB060 (A1)
JAL     0x80277EE0
ADDIU   A0, R0, 0x0011
LUI     A0, 0x8034
LUI     A1, 0x004F
LW      A2, 0xB060 (A0)
ADDIU   A1, A1, 0xC000
JAL     0x80279028
ADDIU   A0, A0, 0xB080
; RAM dest
LUI     A0, 0x8036
ORI     A0, A0, 0x7460
; ROM start
LUI     A1, 0x0010
ORI     A1, A1, 0xC280
; ROM end
LUI     A2, ((0x0010C280+0x0001139F) >> 16)
JAL     0x80278504
ORI     A2, A2, ((0x0010C280+0x0001139F) & 0xFFFF)
; RAM dest
LUI     A0, 0x807E
; ROM start
LUI     A1, ((0x00188440) >> 16)
ORI     A1, A1, ((0x00188440) & 0xFFFF)
; ROM end
LUI     A2, ((0x00188440+0x13FFF) >> 16)
JAL     0x80278504
ORI     A2, A2, ((0x00188440+0x13FFF) & 0xFFFF)
JAL     troll_setup
NOP
NOP
; 80248A90
