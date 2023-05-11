; me when spin is rel turkey sandwirch

; object constants
CELEB_STAR_ACT_SPIN_AROUND_MARIO equ 0x0
CELEB_STAR_ACT_FACE_CAMERA equ 0x1

; object fields
oCelebStarUnkF4 equ 0x0F4
oCelebStarDiameterOfRotation equ 0x108

.headersize 0x8018F240
.orga 0x3D0DC0+0x3100
.area 0x44c
.importobj "Objects/star_spin_bhv/bhv.o"
.endarea