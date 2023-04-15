.headersize 0x8018F240
.orga 0x3D0DC0+0xB400
.area 0xC00
.importobj "Objects/paintings/painting_data.o"
.endarea

.orga 0x3D0DC0+0xF700
.area 0x100
.importobj "Objects/paintings/painting_object_extras.o"
.endarea
.orga 0x3D0DC0+0xF800
.area 0x600
.importobj "Objects/paintings/painting_bhv.o"
.importobj "Objects/paintings/painting_object.o"
.endarea
.definelabel paintings, 0x8056A4FC

; find_floor nullage fix
.headersize 0x80245000
.orga 0x90D0C
.area 0xF0
.importobj "Objects/paintings/geo_painting_update.o"
.endarea


; Engine v2: gTrollWarpDestination code patches
.headersize 0x80245000

; case WARP_OP_STAR_EXIT - moved to interact_star_or_key
.org 0x8024AACC
ADDIU   A1, R0, 0x0020
LUI     AT, 0x8034
SH      A1, 0xB254 (AT)
ADDIU   T8, R0, 0x00F0
SH      T8, 0xB256 (AT)
SH      R0, 0xBACC (AT)
ADDIU   A0, R0, 0x0011
NOP
NOP
NOP
OR      A2, R0, R0
OR      A3, R0, R0
JAL     0x8027B1A0
SW      R0, 0x0010 (SP)

; case WARP_OP_DEATH
.org 0x8024AB0C
LW      T9, 0x0030 (SP)
LB      T0, 0x00AD (T9)
BNEZ    T0, @endif
LUI     AT, 0x8034
ADDIU   T1, R0, 0x0014
SH      T1, 0xB252 (AT)
@endif:
ADDIU   A1, R0, 0x0030
SH      A1, 0xB254 (AT)
ADDIU   T3, R0, 0x00F1
SH      T3, 0xB256 (AT)
ADDIU   A0, R0, 0x0013
NOP
ORI     T8, R0, 0x0001
LUI     AT, 0x8080
SB      T8, 0xFFD0 (AT)
OR      A2, R0, R0
OR      A3, R0, R0
JAL     0x8027B1A0
SW      R0, 0x0010 (SP)

; case WARP_OP_WARP_FLOOR
.org 0x8024AB74
ADDIU   A0, R0, 0x00F3
LUI     AT, 0x8034
JAL     0x8027A9C8
SH      A0, 0xB256 (AT)
BNEZ    V0, @branch1
LUI     AT, 0x8034
ORI     T8, R0, 0x0001
LUI     V0, 0x8080
SB      T8, 0xFFD0 (V0)
LW      T5, 0x0030 (SP)
LB      T6, 0x00AD (T5)
BNEZ    T6, @branch2
NOP
ADDIU   T7, R0, 0x0014
B       @branch1
SH      T7, 0xB252 (AT)
@branch2:
ADDIU   T8, R0, 0x00F1
LUI     AT, 0x8034
SH      T8, 0xB256 (AT)
@branch1:
ADDIU   T9, R0, 0x0014
LUI     AT, 0x8034
SH      T9, 0xB254 (AT)
ADDIU   A0, R0, 0x000B
ADDIU   A1, R0, 0x0014
OR      A2, R0, R0
OR      A3, R0, R0
JAL     0x8027B1A0
SW      R0, 0x0010 (SP)

; initiate_delayed_warp
.headersize 0x80245000
.org 0x8024AEDC
.area 0x27C
.importobj "Objects/paintings/initiate_delayed_warp.o"
.endarea

; init_mario_after_warp
.headersize 0x80245000
.org 0x80249CD8
.area 0x44C
.importobj "Objects/paintings/init_mario_after_warp.o"
.endarea

; function for init_mario_after_warp
.headersize 0x80245000
.org 0x802DB224
.area 0x12C
.importobj "Objects/paintings/offload_init_mario_after_warp.o"
.endarea

; interact_star_or_key
.headersize 0x80245000
.org 0x8024dc28
.area 0x224
.importobj "Objects/paintings/interact_star_or_key.o"
.endarea
