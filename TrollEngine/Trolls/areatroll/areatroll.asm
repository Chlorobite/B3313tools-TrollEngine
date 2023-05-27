; The purpose of areatroll is to unlock more than 8 areas (34)

; Game segment
.headersize 0x80245000

; area.c
.org 0x8027AB04
.area 0x8027AD74-0x8027AB04
.importobj "Trolls/areatroll/clear_areas.o"
.endarea

.org 0x8027AD74
.area 0x8027AE44-0x8027AD74
.importobj "Trolls/areatroll/clear_area_graph_nodes.o"
.endarea

; load_area manually edited because rom mangler
.org 0x8027AE50
LUI     T6, 0x8033
LW      T6, 0xDDCC (T6)
BNE     T6, R0, 0x8027AF38
LW      T7, 0x0018 (SP)
LUI     T9, (gAreaDataNew >> 16)
ORI     T9, T9, (gAreaDataNew & 0xFFFF)
.org 0x8027AE78
LW      T9, 0x0004 (T9)
BEQ     T9, R0, 0x8027AF38
.org 0x8027AE84
JAL     0x80403700 ; moved the load here so it only loads when new area
NOP
LW      T0, 0x0018 (SP)
LUI     T2, (gAreaDataNew >> 16)
ORI     T2, T2, (gAreaDataNew & 0xFFFF)
SLL     T1, T0, 4
SUBU    T1, T1, T0
SLL     T1, T1, 2
ADDU    T3, T1, T2
LUI     AT, 0x8033
SW      T3, 0xDDCC (AT)
LW      T4, 0xDDCC (AT)
LUI     T0, 0x8034
LB      T5, 0x0000 (T4)
SH      T5, 0xBACA (T0)
LW      T6, 0xDDCC (AT)


.org 0x8032DDC8 ; struct Area *gAreas (vanilla: = gAreaData)
.word gAreaDataNew


; Engine segment
.headersize 0x80283280

; level_script.c
.org 0x8037EF70
.area 0x8037F010-0x8037EF70
.importobj "Trolls/areatroll/level_cmd_free_level_pool.o"
.endarea


