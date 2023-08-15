.headersize 0x80245000


; mario.c

; for extra animations
.org 0x802509EC ; set_mario_animation
JAL     mario_anim_load_patchable_table ; declared in AI/personalization_helpers.c
.org 0x80250B3C ; set_mario_anim_with_accel
JAL     mario_anim_load_patchable_table ; declared in AI/personalization_helpers.c

.org 0x802523DC ; set_mario_y_vel_based_on_fspeed
; for beta trampoline
JAL     troll_get_additive_y_vel_for_jumps ; declared in AI/personalization_helpers.c

; set_mario_action_airborne, personalize based on red star count
.org 0x8025267C
JAL     triple_jump_set_mario_y_vel_based_on_fspeed ; declared in AI/personalization_helpers.c
.org 0x802527B8
JAL     sideflip_set_mario_y_vel_based_on_fspeed ; declared in AI/personalization_helpers.c

.org 0x80252CF4
.area 0x80252E5C-0x80252CF4
.importobj "Mario/Trolls/mario/set_mario_action.o"
.endarea

.org 0x80254060
.area 0x802542B4-0x80254060
.importobj "Mario/Trolls/mario/update_mario_health.o"
.endarea

.org 0x80254830
.area 0x80254B20-0x80254830
.importobj "Mario/Trolls/mario/execute_mario_action.o"
.endarea

.org 0x80254B20
.area 0x80254F44-0x80254B20
.importobj "Mario/Trolls/mario/init_mario.o"
.endarea



; mario_step.c
.org 0x80255D88 ; perform_ground_step
J       troll_perform_ground_step ; declared in AI/personalization_helpers.c
NOP



; mario_actions_moving.c
.org 0x80265DF8
.area 0x80266038-0x80265DF8
.importobj "Mario/Trolls/mario_actions_moving/push_or_sidle_wall.o"
.endarea


.org 0x80261DB4
J       troll_act_crouching
NOP

.org 0x80262530
J       troll_act_start_crouching
NOP

.org 0x80262650
J       troll_act_stop_crouching
NOP

.org 0x80268168
J       troll_act_crouch_slide
NOP


; mario_actions_airborne.c
.org 0x8026B17C
.area 0x8026B444-0x8026B17C
.importobj "Mario/Trolls/mario_actions_airborne/update_flying.o"
.endarea

; for climbma
.org 0x8026B444
.area 0x8026B6A0-0x8026B444
.importobj "Mario/Trolls/mario_actions_airborne/common_air_action_step.o"
.endarea

.org 0x8026FB04
.area 0x80270110-0x8026FB04
.importobj "Mario/Trolls/mario_actions_airborne/mario_execute_airborne_action.o"
.endarea


; mario_actions_automatic.c
; for climbma
.org 0x802605D0
.area 0x802608B0-0x802605D0
.importobj "Mario/Trolls/mario_actions_automatic/mario_execute_automatic_action.o"
.endarea



; mario_actions_cutscene.c
.org 0x802586CC
.area 0x80258744-0x802586CC
.importobj "Mario/Trolls/mario_actions_cutscene/common_death_handler.o"
.endarea

.org 0x80258184 ; general_star_dance_handler (to implement silver stars)
J       troll_general_star_dance_handler ; implemented in AI/personalization_helpers.c
NOP

.org 0x802596C0 ; act_spawn_spin_airborne
; fixes mario's level entry act crashing on n64 due to float operation in delay slot?
NOP
SUB.S   F18, F10, F16
C.LT.S  F6, F18



; mario_actions_stationary.c
.org 0x80260CB4
.area 0x80260F94-0x80260CB4
.importobj "Mario/Trolls/mario_actions_stationary/act_idle.o"
.endarea

; act_coughing (do not play sound)
.org 0x80261934+0xC
NOP
;LUI     A0, 0x242E
.org 0x80261968+0xC
NOP
;LUI     A0, 0x242E
.org 0x8026199C+0xC
NOP
;LUI     A0, 0x242E

.org 0x80261F70
.area 0x80262080-0x80261F70
.importobj "Mario/Trolls/mario_actions_stationary/act_panting.o"
.endarea

.org 0x80263898
.area 0x80263E60-0x80263898
.importobj "Mario/Trolls/mario_actions_stationary/mario_execute_stationary_action.o"
.endarea



; mario_actions_object.c
.org 0x80275B34
.area 0x80275E78-0x80275B34
.importobj "Mario/Trolls/mario_actions_object/act_holding_bowser.o"
.endarea
