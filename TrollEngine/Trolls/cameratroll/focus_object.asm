.headersize 0x8018F240
.orga 0x3D0DC0+0x500
.area 0x100,0x00


@beh:
.word 0x00040000
.word 0x08000000
.word 0x0c000000, @trollfunc
.word 0x09000000

@trollfunc:
LUI   T0, 0x8033
LW    T0, 0xDDCC (T0) ; area
LW    T0, 0x0024 (T0) ; camera
LUI   T1, 0x8036
LW    T1, 0x1160 (T1) ; current object
LBU   T2, 0x0188 (T1) ; T2 = bparam1
LW    T9, 0x00A0 (T1) ; oPosX...
SW    T9, 0x0028 (T0) ; ...to areaCenX
LW    T9, 0x00A8 (T1) ; oPosZ...
; DDD mode?
ORI   T8, R0, 0x0001
BNE   T2, T8, @endif1
SW    T9, 0x002C (T0) ; ...to areaCenZ
; DDD mode, change camera type based on camera position
LUI   AT, 0x45A0
MTC1  AT, F6
LWC1  F4, 0x000C (T0) ; camera position z or smth
C.LT.S F4, F6 ; position < 5120 ?
NOP
BC1F  @ddd_pos_check_endif
ORI   T9, R0, 0x0004 ; >=5120 (cave) : CAMERA_MODE_CLOSE...
ORI   T9, R0, 0x0001 ; < 5120 (spawn): CAMERA_MODE_RADIAL...
@ddd_pos_check_endif:
LUI   AT, 0x8034
SB    T9, 0xC6D5 (AT) ; ...to current camera mode
@endif1:
; inside castle mode?
ORI   T9, R0, 0x0002
BNE   T2, T9, @endif2
NOP
; inside castle mode
J bhv_camtrollobj_insidecastle
NOP
@endif2:
JR    RA
NOP

.endarea

; sCameraTriggers
.orga 0xE9CB0
.word 0, 0, 0, 0
.word 0, 0, 0, 0
.word 0, 0, 0, 0
.word 0, 0, 0, 0
.word 0, 0, 0, 0
.word 0, 0, 0, 0
.word 0, 0, 0, 0
.word 0, 0, 0, 0
.word 0, 0, 0, 0
.word 0, 0, 0, 0

; update_fixed_camera fix for nathaniel uninitialized variables everytime
.org 0x8028244C
ADD.S   F8, F16, F4
NOP

; update_fixed_camera
.org 0x80282520
.word 0x11C00008 ; restores a branch that was presumably missing

; beta 60 fov in inside castle\
.headersize 0x80245000
.org 0x8029a8d0
.area 0x8029a9a4-0x8029a8d0,0x00
.importobj "Trolls/cameratroll/approach_fov_45.o"
.endarea
