; The purpose of bhv trolls is to edit existing objects' behavior

; Objects edited for difficulty scaling: bosses, Koopa the Quick
; Objects edited for discoloration: coin, amp, bob-omb, goomba

; Game segment
.headersize 0x80245000

; ukiki_act_go_to_cage
.org 0x802BB3D4
JAL     spawn_red_coin_cutscene_star
.orga 0x1203F88
J       spawn_red_coin_cutscene_star

.org 0x802E8AE4
.area 0x802E8ECC-0x802E8AE4
.importobj "Trolls/bhv/amp.inc.c__bhv_circling_amp_init.o"
.endarea

.org 0x802E844C
.area 0x802E84CC-0x802E844C
.importobj "Trolls/bhv/amp.inc.c__check_amp_attack.o"
.endarea

.org 0x802E8618
.area 0x802E885C-0x802E8618
.importobj "Trolls/bhv/amp.inc.c__homing_amp_chase_loop.o"
.endarea

.org 0x802E8D98
.area 0x802E8ECC-0x802E8D98
.importobj "Trolls/bhv/amp.inc.c__circling_amp_idle_loop.o"
.endarea

; Yes Christopher Brutal Agresion, beta trampoline is implemented here!
.org 0x802B2494
.area 0x802B25AC-0x802B2494
.importobj "Trolls/bhv/beta_trampoline.inc.c__bhv_beta_trampoline_spring_loop.o"
.endarea

.org 0x802B25AC
.area 0x802B26A4-0x802B25AC
.importobj "Trolls/bhv/beta_trampoline.inc.c__bhv_beta_trampoline_top_loop.o"
.endarea

; blue fish fix
; (loading) remove gMarioCurrentRoom check
.orga 0x747E4
NOP
NOP
; (unloading) skip gMarioCurrentRoom check (so they never unload)
.orga 0x7487C
j 0x802B98C0
NOP

.org 0x802E6AF8
.area 0x802E6BD4-0x802E6AF8
.importobj "Trolls/bhv/bobomb.inc.c__bobomb_act_explode.o"
.endarea

.org 0x802E6BD4
.area 0x802E6CF0-0x802E6BD4
.importobj "Trolls/bhv/bobomb.inc.c__bobomb_check_interactions.o"
.endarea

.org 0x802E6CF0
.area 0x802E6DC8-0x802E6CF0
.importobj "Trolls/bhv/bobomb.inc.c__bobomb_act_patrol.o"
.endarea

.org 0x802E7180
.area 0x802E7220-0x802E7180
.importobj "Trolls/bhv/bobomb.inc.c__bobomb_held_loop.o"
.endarea

.org 0x802E7220
.area 0x802E7280-0x802E7220
.importobj "Trolls/bhv/bobomb.inc.c__bobomb_dropped_loop.o"
.endarea

.org 0x802E770C
.area 0x802E7814-0x802E770C
.importobj "Trolls/bhv/bobomb.inc.c__bobomb_buddy_act_idle.o"
.endarea

.org 0x802C5414
.area 0x802C5688-0x802C5414
.importobj "Trolls/bhv/boo.inc.c__bhv_boo_in_castle_loop.o"
.endarea

.org 0x802C4B9C ; big_boo_spawn_ghost_hunt_star
; stub to make big boo with key work
JR      RA
NOP

.org 0x802B3C2C
.area 0x802B3CDC-0x802B3C2C
.importobj "Trolls/bhv/bowser.inc.c__bowser_tail_anchor_act_default.o"
.endarea

.org 0x802B4300
.area 0x802B4368-0x802B4300
.importobj "Trolls/bhv/bowser.inc.c__bowser_set_anim_slow_gait.o"
.endarea

.org 0x802B45F4
.area 0x802B473C-0x802B45F4
.importobj "Trolls/bhv/bowser.inc.c__bowser_bitdw_actions.o"
.endarea

.org 0x802B473C
.area 0x802B48D4-0x802B473C
.importobj "Trolls/bhv/bowser.inc.c__bowser_bitfs_actions.o"
.endarea

.org 0x802B48D4
.area 0x802B4A1C-0x802B48D4
.importobj "Trolls/bhv/bowser.inc.c__bowser_bits_action_list.o"
.endarea

.org 0x802B4A3C
.area 0x802B4AF4-0x802B4A3C
.importobj "Trolls/bhv/bowser.inc.c__bowser_bits_actions.o"
.endarea

.org 0x802B4D14
.area 0x802B4F00-0x802B4D14
.importobj "Trolls/bhv/bowser.inc.c__bowser_act_walk_to_mario.o"
.endarea

.org 0x802B5218
.area 0x802B53F4-0x802B5218
.importobj "Trolls/bhv/bowser.inc.c__bowser_act_hit_mine.o"
.endarea

.org 0x802B5C40
.area 0x802B5F6C-0x802B5C40
.importobj "Trolls/bhv/bowser.inc.c__bowser_act_charge_mario.o"
.endarea

.org 0x802B5F6C
.area 0x802B5FEC-0x802B5F6C
.importobj "Trolls/bhv/bowser.inc.c__bowser_check_hit_mine.o"
.endarea

.org 0x802B65D0
.area 0x802B6670-0x802B65D0
.importobj "Trolls/bhv/bowser.inc.c__bowser_spawn_collectable.o"
.endarea

.org 0x802B71E4
.area 0x802B72D4-0x802B71E4
.importobj "Trolls/bhv/bowser.inc.c__bowser_free_update.o"
.endarea

.org 0x802B7878
.area 0x802B798C-0x802B7878
.importobj "Trolls/bhv/bowser.inc.c__bhv_bowser_init.o"
.endarea

.org 0x802B8434
.area 0x802B84AC-0x802B8434
.importobj "Trolls/bhv/bowser_flame.inc.c__bowser_flame_should_despawn.o"
.endarea

.org 0x802B8D68
.area 0x802B8E7C-0x802B8D68
.importobj "Trolls/bhv/bowser_flame.inc.c__bhv_blue_bowser_flame_init.o"
.endarea

.org 0x802B8E7C
.area 0x802B9034-0x802B8E7C
.importobj "Trolls/bhv/bowser_flame.inc.c__bhv_blue_bowser_flame_loop.o"
.endarea

.org 0x802B921C
.area 0x802B935C-0x802B921C
.importobj "Trolls/bhv/bowser_flame.inc.c__bhv_blue_flames_group_loop.o"
.endarea

.org 0x802A6EE4
.area 0x802A7020-0x802A6EE4
.importobj "Trolls/bhv/capswitch.inc.c__cap_switch_act_0.o"
.endarea

.org 0x802A7020
.area 0x802A708C-0x802A7020
.importobj "Trolls/bhv/capswitch.inc.c__cap_switch_act_1.o"
.endarea

.org 0x802A708C
.area 0x802A7160-0x802A708C
.importobj "Trolls/bhv/capswitch.inc.c__cap_switch_act_2.o"
.endarea

.org 0x802C60AC
J       toyal_bhv_castle_floor_trap_init ; not enough space; implemented in AI/personalization_helpers.c
NOP

.org 0x802ECFAC
.area 0x802ED10C-0x802ECFAC
.importobj "Trolls/bhv/celebration_star.c__bhv_celebration_star_init.o"
.endarea

.org 0x802ED10C
.area 0x802ED28C-0x802ED10C
.importobj "Trolls/bhv/celebration_star.c__celeb_star_act_spin_around_mario.o"
.endarea

.org 0x802ED28C
.area 0x802ED39C-0x802ED28C
.importobj "Trolls/bhv/celebration_star.c__celeb_star_act_face_camera.o"
.endarea

.org 0x80300778
.area 0x803008A8-0x80300778
.importobj "Trolls/bhv/chain_chomp.inc.c__chain_chomp_released_break_gate.o"
.endarea

.org 0x803008A8
.area 0x803008EC-0x803008A8
J       troll_chain_chomp_released_jump_away
NOP
.endarea

.org 0x802AB5C8
.area 0x802AB650-0x802AB5C8
.importobj "Trolls/bhv/coin.inc.c__bhv_coin_sparkles_init.o"
.endarea

.org 0x802AB650
.area 0x802AB70C-0x802AB650
.importobj "Trolls/bhv/coin.inc.c__bhv_yellow_coin_init.o"
.endarea

.org 0x802AB7A4
.area 0x802AB860-0x802AB7A4
.importobj "Trolls/bhv/coin.inc.c__bhv_coin_init.o"
.endarea

.org 0x802ABA40
.area 0x802ABC04-0x802ABA40
.importobj "Trolls/bhv/coin.inc.c__bhv_coin_formation_spawn_loop.o"
.endarea

; overflows; useless object tbh
;.org 0x802B9E94
;.area 0x802B9EFC-0x802B9E94
;.importobj "Trolls/bhv/ddd_warp.inc.c__bhv_ddd_warp_loop.o"
;.endarea

; door_animation_and_reset
.org 0x802AC910
J       troll_door_animation_and_reset ; not enough space; implemented in AI/personalization_helpers.c
NOP

.org 0x802ACAC8
.area 0x802ACC3C-0x802ACAC8
.importobj "Trolls/bhv/door.inc.c__bhv_door_loop.o"
.endarea

.org 0x802ACC3C
.area 0x802ACE80-0x802ACC3C
.importobj "Trolls/bhv/door.inc.c__bhv_door_init.o"
.endarea

.org 0x8030AE9C
.area 0x8030B0B8-0x8030AE9C
.importobj "Trolls/bhv/dorrie.inc.c__dorrie_act_move.o"
.endarea

.org 0x8030B2F4
.area 0x8030B658-0x8030B2F4
.importobj "Trolls/bhv/dorrie.inc.c__bhv_dorrie_update.o"
.endarea

.org 0x802ADDF8
.area 0x802ADF6C-0x802ADDF8
.importobj "Trolls/bhv/elevator.inc.c__bhv_elevator_init.o"
.endarea

.org 0x8030E9E0
.area 0x8030EA9C-0x8030E9E0
.importobj "Trolls/bhv/eyerok.inc.c__eyerok_boss_act_die.o"
.endarea

; some romanger tweak shuffled the eyerok code around, so return to vanilla
.org 0x8030ECF0
.area 0x8030F590-0x8030ECF0
.incbin "Trolls/bhv/eyerokdeeznuts.bin"
.endarea

.org 0x802FF584
.area 0x802FF600-0x802FF584
.importobj "Trolls/bhv/goomba.inc.c__mark_goomba_as_dead.o"
.endarea

.org 0x802FF600
.area 0x802FF868-0x802FF600
.importobj "Trolls/bhv/goomba.inc.c__goomba_act_walk.o"
.endarea

.org 0x802FF96C
.area 0x802FFB38-0x802FF96C
.importobj "Trolls/bhv/goomba.inc.c__bhv_goomba_update.o"
.endarea

.org 0x802C26F8
.area 0x802C2930-0x802C26F8
.importobj "Trolls/bhv/grill_door.inc.c__bhv_openable_grill_loop.o"
.endarea

.org 0x802F3014
.area 0x802F30F0-0x802F3014
.importobj "Trolls/bhv/hidden_star.inc.c__bhv_hidden_star_init.o"
.endarea

.org 0x802F30F0
.area 0x802F31BC-0x802F30F0
.importobj "Trolls/bhv/hidden_star.inc.c__bhv_hidden_star_loop.o"
.endarea

.org 0x802F31BC
.area 0x802F328C-0x802F31BC
.importobj "Trolls/bhv/hidden_star.inc.c__bhv_hidden_star_trigger_loop.o"
.endarea

.org 0x802F328C
.area 0x802F336C-0x802F328C
.importobj "Trolls/bhv/hidden_star.inc.c__bhv_bowser_course_red_coin_star_loop.o"
.endarea

.org 0x802A7264
.area 0x802A7384-0x802A7264
.importobj "Trolls/bhv/king_bobomb.inc.c__king_bobomb_act_0.o"
.endarea

.org 0x802A73D8
.area 0x802A7598-0x802A73D8
.importobj "Trolls/bhv/king_bobomb.inc.c__king_bobomb_act_2.o"
.endarea

.org 0x802A7598
.area 0x802A7804-0x802A7598
.importobj "Trolls/bhv/king_bobomb.inc.c__king_bobomb_act_3.o"
.endarea

.org 0x802A7A60
.area 0x802A7B1C-0x802A7A60
.importobj "Trolls/bhv/king_bobomb.inc.c__king_bobomb_act_7.o"
.endarea

.org 0x802FD068
.area 0x802FD3E4-0x802FD068
.importobj "Trolls/bhv/koopa.inc.c__koopa_the_quick_act_race.o"
.endarea

.org 0x802A6C20
.area 0x802A6C74-0x802A6C20
.include "Trolls/bhv/pole.inc.c__bhv_pole_init.s"
.endarea

; bhv_snow_leaf_particle_spawn_init
.org 0x802B09C8
B       0x802B0A8C ; always spawn leaves

.org 0x80312B80
.area 0x80312D0C-0x80312B80
.importobj "Trolls/bhv/skeeter.inc.c__skeeter_act_idle.o"
.endarea

.org 0x802AAF48
.area 0x802AAFFC-0x802AAF48
.importobj "Trolls/bhv/sparkle_spawn_star.inc.c__bhv_spawned_star_init.o"
.endarea

.org 0x802AB558
.area 0x802AB5C8-0x802AB558
.importobj "Trolls/bhv/sparkle_spawn_star.inc.c__bhv_spawn_star_no_level_exit.o"
.endarea

.org 0x802F24F4
.area 0x802F25B0-0x802F24F4
.importobj "Trolls/bhv/spawn_star.inc.c__bhv_collect_star_init.o"
.endarea

.org 0x802F2AA0
.area 0x802F2B88-0x802F2AA0
.importobj "Trolls/bhv/spawn_star.inc.c__spawn_star.o"
.endarea

; spawn_red_coin_cutscene_star
.org 0x802F2BF4 ; red coins and silvers already have object position, don't fall for magnets
JAL     troll_spawn_star_bypass_magnets
.org 0x802F2C0C ; set collect personalization AI flag for the star
LHU     T6, 0x01B0 (T7)
ORI     T6, T6, 0x0002
SH      T6, 0x01B0 (T7)
LW      RA, 0x0014 (SP)
JR      RA
ADDIU   SP, SP, 0x20

.org 0x802F2C24
.area 0x802F2C84-0x802F2C24
.importobj "Trolls/bhv/spawn_star.inc.c__spawn_no_exit_star.o"
.endarea

.org 0x802F2C84
.area 0x802F2D8C-0x802F2C84
.importobj "Trolls/bhv/spawn_star.inc.c__bhv_hidden_red_coin_star_init.o"
.endarea

.org 0x802F2D8C
.area 0x802F2E6C-0x802F2D8C
.importobj "Trolls/bhv/spawn_star.inc.c__bhv_hidden_red_coin_star_loop.o"
.endarea

.org 0x802B19D8
.area 0x802B1AE0-0x802B19D8
.importobj "Trolls/bhv/switch_hidden_objects.inc.c__hidden_unbreakable_box_actions.o"
.endarea

.org 0x802A8CDC
.area 0x802A8D48-0x802A8CDC
.importobj "Trolls/bhv/water_objs.inc.c__bhv_fish_group_loop.o"
.endarea

.org 0x802C2A24
.area 0x802C2CE8-0x802C2A24
.importobj "Trolls/bhv/wdw_water_level.inc.c__bhv_water_level_diamond_loop.o"
.endarea

.org 0x802C7858
.area 0x802C7998-0x802C7858
.importobj "Trolls/bhv/whomp.inc.c__whomp_die.o"
.endarea

.org 0x802C710C
.area 0x802C7254-0x802C710C
.importobj "Trolls/bhv/whomp.inc.c__king_whomp_chase.o"
.endarea

.org 0x802C76D4
.area 0x802C7858-0x802C76D4
.importobj "Trolls/bhv/whomp.inc.c__whomp_on_ground_general.o"
.endarea

.org 0x80301F70
.area 0x80302024-0x80301F70
.importobj "Trolls/bhv/wiggler.inc.c__wiggler_act_shrink.o"
.endarea

.org 0x80301940
.area 0x80301C88-0x80301940
.importobj "Trolls/bhv/wiggler.inc.c__wiggler_act_walk.o"
.endarea
