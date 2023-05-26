.n64
.open "../Bee/b3313 silved.z64", "../Bee/b3313 new.z64", 0
.include "trollfinity.asm" ; the asm behind the trolling
.definelabel osPiStartDma, 0x80324460


.include "AI/ai.asm" ; no way there is a personaliatoin real ai ai chatgpt real stable diffusion

.include "Mario/mario.asm" ; M is real

; expand area IDs from 8 to 33
.include "Trolls/areatroll/areatroll.asm"
; focus object behavior (0x1F000500), and beta lobby stuff
.include "Trolls/cameratroll/focus_object.asm"
; global star IDs: any star ID >=16 will directly correspond to a specific star ID in the save file,
; as opposed to referencing the current level's stars
.include "Trolls/global_star_ids/global stars.asm"
; a big fat list of behavior functions
.include "Trolls/bhv/bhv.asm"

; See object docs
.include "Objects/blaarg/blarg.asm"
.include "Objects/gombatower/gomba tower.asm"
.include "Objects/mirror/mirror_mario.asm"
.include "Objects/motos/motos.asm"
.include "Objects/paintings/painting_object.asm"
.include "Objects/other_bhvs/1f_bhvscripts.asm"
.include "Objects/topbhv/topbhv.asm"
.include "Objects/GhostPeach.asm"

; "O2" is a project to optimize some hot game functions (mainly collision) so it doesn't lagma like shit
; then level scale happened, so it was split into 2
; no level scale mode probably doesn't work so don't bother, unless level scale is to be removed for any reason
; (e.g. another minihack)
;.include "O2/o2_nolevelscale.asm"
.include "O2/o2_levelscale.asm"

; 8056C000.. is crash screen code, do not overwrite!!!
; I don't have the original armips for this :( so the funny binary data from previous rom will have to do
.include "crashfix.asm"


.orga 0xEFFFC
; total audio heap size (there is unused stack space after)
.word 0x80200600-0x801ce000
; init pool size
.word 0x3800

; sound_init_main_pools
; do not allocate space in audio pools, instead, allocate 807F4000-807F7800
.orga 0xD2138 ;80317138
LUI     A1, 0x807F
ORI     A1, A1, 0x4000
; remove ADDU and SUBU since pointless now
.orga 0xD2168 ;80317168
OR      A1, T6, R0
JAL     0x803170B4
OR      A2, T7, R0

; sound pools
.orga 0xEE2AC
.word 0x8000 ; permanent note partition
.word 0xB330 ; temporary note partition



.headersize 0x80245000

; fix setup_game_memory not setting A3=0 (side) for load_segment for segment 2, causing random crashes on start
.org 0x80248A8C
LUI     A1, 0x0011
ADDIU   A2, A1, 0x8A40
ADDIU   A1, A1, 0x8A10
ADDIU   A0, R0, 0x0010
JAl     0x8027868C
OR      A3, R0, R0
LUI     A1, 0x0080
ADDIU   A1, A1, 0x3156
LUI     A2, 0x0082
ADDIU   A2, A2, 0xD164
ADDIU   A0, R0, 0x0002
JAl     0x8027868C
OR      A3, R0, R0

; WHAT THE FUCK IS THIS SCUTTLEBUG DOING IN BEEIE GET THE FUCK OUT
.org 0x80294FFC
LW      T6, 0xDF60 (T6)
LUI     T7, 0x8034

; browser level fix
.org 0x8028752C
; Instead of demo, check for browser's existence, and skip the entire thing if no browser
LUI     T3, 0x8033
LW      T3, 0xDF30 (T3)
BEQ     T3, R0, 0x802876EC
NOP
; Make the other 2 browser levels use this check as well
.org 0x80287578
B       0x8028752C
NOP
.org 0x8028758C
B       0x8028752C
NOP

; sefty
.org 0x8024694c
.area 0x68
.importobj "handle_dp_complete.o"
.endarea

; fixes a shitty rom mangler function that caused crash on game over
.orga 0x1203700
ADDIU   SP, SP, -0x18
SW      RA, 0x0014 (SP)
SLL     A0, A0, 4
SW      A0, 0x0010 (SP)
JAL     0x80277F50
LUI     A0, 0x1900
LW      A0, 0x0010 (SP)
LW      T1, 0x5FFC (V0)
ADD     V0, V0, A0
LUI     A0, 0x8042
LUI     T2, 0x4BC9
ORI     T2, T2, 0x189A
BNE     T1, T2, @nodma
LUI     AT, 0xF000
LW      A1, 0x5F00 (V0)
LW      A2, 0x5F04 (V0)
AND     T0, A1, AT
BNE     T0, R0, @nodma
AND     T0, A2, AT
BNE     T0, R0, @nodma
SLT     T0, A1, A2
BEQ     T0, R0, @nodma
NOP
JAL     0x80278504
NOP
@nodma:
;the function sets T6 for load_area, but load_area doesn't need it anymore
;uncomment in case this is not the case anymore
;LUI     T6, 0x8033
;LW      T6, 0xDDCC (T6)
LW      RA, 0x0014 (SP)
JR      RA
ADDIU   SP, SP, 0x18
.close 


