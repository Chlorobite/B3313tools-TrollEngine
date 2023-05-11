; note this is the gears intended bhv so id rather use this one than the rm one

; sounds
SOUND_GENERAL_BIG_CLOCK equ 0x30170080

; object constants
ACT_GEAR_SPIN equ       0x0
ACT_GEAR_STOP equ       0x1

; object fields
oTimer  equ 0x154
oAngleVelYaw equ 0x118

.headersize 0x8018F240
.orga 0x3D0DC0+0xB240
.area 0x27c
.importobj "Objects/gear_bhv/gear_bhv.o"
.endarea