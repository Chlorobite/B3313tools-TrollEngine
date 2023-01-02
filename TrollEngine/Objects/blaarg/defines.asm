/*
This file should not contain any actual code or data to be written to the ROM.
Instead, this file is to be used for label definitions or macros to be used in
other asm files.
*/

.include "Objects/blaarg/behaviour-script-macros.asm"
.include "Objects/blaarg/labels.asm"
.include "Objects/blaarg/fast-3d.asm"

.include "Objects/blaarg/globals-ref.asm"

g_play_mode equ 0x8033B238 ; short

get_mario_dist_from_home equ 0x802A15AC

beh_explosion equ 0x13003510
beh_bobomb equ 0x13003174

PLAY_MODE_PAUSED equ 2
