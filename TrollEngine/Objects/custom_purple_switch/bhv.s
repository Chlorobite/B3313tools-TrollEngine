SHAKE_POS_SMALL equ 1
SOUND_GENERAL2_PURPLE_SWITCH equ 0x803EC080
MODEL_PURPLE_SWITCH equ 0xCF
o_platform equ 0x214
MARIO_NO_PURPLE_SWITCH equ 0x00002000

; o constants
PURPLE_SWITCH_BP_NO_TICK equ                        0x0
PURPLE_SWITCH_BP_ANIMATES equ                       0x1
PURPLE_SWITCH_BP_REVEAL_HIDDEN equ                  0x2

PURPLE_SWITCH_ACT_IDLE equ                          0x0
PURPLE_SWITCH_ACT_PRESSED equ                       0x1
PURPLE_SWITCH_ACT_TICKING equ                       0x2
PURPLE_SWITCH_ACT_UNPRESSED equ                     0x3
PURPLE_SWITCH_ACT_WAIT_FOR_MARIO_TO_GET_OFF equ     0x4


SCALE_AXIS_X equ 0x01
SCALE_AXIS_Y equ 0x02
SCALE_AXIS_Z equ 0x04

; o fields
oAction equ 0x14C

.headersize 0x8018F240
.orga 0x3D0DC0+0xC000
.area 0x27c
bhvPurpleSwitchThatDisappearsAShip:
.word 0x00090000
.word 0x11010001
.word 0x2A000000, 0x0800C7A8
.word 08000000
.word 0x0C000000, bhv_custom_purple_switch_loop
.word 0x0C000000, load_object_collision_model
.word 09000000


.importobj "Objects/custom_purple_switch/script.o"
.endarea