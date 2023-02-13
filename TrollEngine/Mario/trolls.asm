.headersize 0x80245000

; for extra animations

; set_mario_animation
.org 0x802509EC
JAL     mario_anim_load_patchable_table ; declared in AI/personalization_helpers.c

; set_mario_anim_with_accel
.org 0x80250B3C
JAL     mario_anim_load_patchable_table ; declared in AI/personalization_helpers.c


; for climbma

; common_air_action_step
.org 0x8026B444
.area 0x8026B6A0-0x8026B444
.importobj "Mario/Trolls/common_air_action_step.o"
.endarea

.org 0x802605D0
.area 0x802608B0-0x802605D0
.importobj "Mario/Trolls/mario_execute_automatic_action.o"
.endarea
