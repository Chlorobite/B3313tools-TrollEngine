.n64
.open "../Bee/b3313 silved.z64", "../Bee/b3313 new.z64", 0
.include "trollfinity.asm" ; the asm behind the trolling

; fixes mario's level entry cutscene crashing on n64
.orga 0x146C0
.word 0x00000000
.word 0x46105481
.word 0x4612303C

.include "AI/ai.asm"

.include "Trolls/areatroll/areatroll.asm"
.include "Trolls/cameratroll/focus_object.asm"
.include "Trolls/global_star_ids/global stars.asm"
.include "Trolls/bhv/bhv.asm"

.include "Objects/blaarg/blarg.asm"
.include "Objects/gombatower/gomba tower.asm"
.include "Objects/mirror/mirror_mario.asm"
.include "Objects/motos/motos.asm"
.include "Objects/paintings/painting_object.asm"
.include "Objects/other_bhvs/1f_bhvscripts.asm"
.include "Objects/topbhv/topbhv.asm"

.include "Objects/GhostPeach.asm"

;.include "O2/o2_nolevelscale.asm"
.include "O2/o2_levelscale.asm"


; more audio size
.orga 0xF0000
.word 0x4000

; sound pools
.orga 0xEE2AC
.word 0x5800
.word 0x8800



.headersize 0x80245000

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


