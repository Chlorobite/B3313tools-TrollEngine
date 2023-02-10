.include "AI/ai segment.asm"

; hud trolls
.include "AI/trolls/hud/hud.asm"

; more hud trolls
.include "AI/powermeter/powermeter_rome.asm"

; model trolls
.include "AI/more_models/more_models.asm"

; coin moment
.orga 0x218EE0
.importobj "AI/yellow_coin_geolayout.o"

; skeeter moment
; HOLY FUCKING SHIT
; WE WERE PLACING THESE BYTES THIS WHOLE TIME
; FUCKFUCKFUCKFUCK
; do not uncomment, unless you want to absolutely make sure the skeeter has the lava immunity flag (it should already have it),
; even then, please find segment 13 and fix the rom address
;.orga 0x31EB13C
;.word 0x1101A049 ; set lava immunity flag

; dudaw troll (allows adding condition to beta heal)
.orga 0x120F764
JAL     troll_betaheal ; AI/personalization_helpers.c
NOP
NOP
NOP

; Game segment
.headersize 0x80245000

; mario trolled
.org 0x802523DC
JAL     troll_get_additive_y_vel_for_jumps

; mario cough trolled (do not play sound)
.org 0x80261934+0xC
NOP
;LUI     A0, 0x242E
.org 0x80261968+0xC
NOP
;LUI     A0, 0x242E
.org 0x8026199C+0xC
NOP
;LUI     A0, 0x242E

; elevator data
.org 0x803302F0
.halfword -4348, 0, 0
.halfword -3808, -118, 1
.halfword -2585, 2281, 1

; nop exit particles
.org 0x8025982C
NOP
.org 0x802598AC
NOP
.org 0x80259EAC
NOP

; fast travel trolled
.org 0x8024B67C
JAL     troll_initiate_warp

; fast travel trolled again
.org 0x802DCBA4
BEQ     T5, R0, 0x802DCC20
LUI     T5, ((personalizationFlags + 0x8000) >> 16)
LBU     T5, (personalizationFlags & 0xFFFF) (T5)
ANDI    T5, T5, 0x0080
BNE     T5, R0, 0x802DCC20

; fast travel text trolled
.org 0x802DB120
JAL     troll_print_generic_string

; real beta: disable music fade out on warp
.org 0x8024AEAC
NOP

; fixma camera
.org 0x80286AD8
JAL     0x8028EEB0

; lvl_init_or_update moment
.org 0x8024BCD8
J       troll_lvl_init_or_update
NOP

; unfuck my shit code (mario luigi swap), so that it can be called by the troll double doors
.org 0x8029AD90
; first check the L
LUI     A0, 0x8033
LW      A0, 0xD5E4 (A0)
LHU     A0, 0x0012 (A0)
ANDI    A0, 0x0020
BEQ     A0, R0, 0x8029ADF8
NOP
; THEN push RA
LUI     AT, 0x8080
ADDIU   SP, SP, -0x18
SW      RA, 0x0014 (SP)
; playma?
LUI     A0, 0x701E
JAL     0x802CA190
ORI     A0, A0, 0xFF81

; door warp stuff
.org 0x8024AD00
;AT=0x80340000
;T8=m (SP+0x30)
LW      T9, 0x001C (T8) ; m->actionArg
LW      T1, 0x0080 (T8) ; m->usedObj
LW      S0, 0x0188 (T1) ; m->usedObj->oBehParams
SRL     T0, S0, 7 ; bparam3=1 becomes a value of 2
ANDI    T0, T0, 0x02
XOR     T9, T9, T0
SW      T9, 0xB258 (AT) ; sDelayedWarpArg
NOP
NOP

.org 0x8028c2c8
.area 0x4D8
.importobj "AI/trolls/get_cutscene_from_mario_status.o"
.endarea

.org 0x8032daa0
.area 0x40
.importobj "AI/trolls/sTerrainSounds.o"
.endarea

; should_get_stuck_in_ground
.org 0x8026A494
JR      RA
ORI     V0, R0, 0x0000

; cutscene_door_move_behind_mario tomfoolery
.org 0x802991F8
JAL     troll_swap_mario
SW      A0, 0x0030 (SP)
JAL     0x8029051C
LW      A0, 0x0030 (SP)
JAL     0x8028CDEC
ADDIU   A0, SP, 0x0022
LW      A0, 0x0030 (SP)
OR      A1, R0, R0
LUI     A2, 0x42FA
OR      A3, R0, R0
JAL     0x8028C9CC
SW      R0, 0x0010 (SP)
LUI     T6, 0x8033
LW      T6, 0xDF60 (T6)
LH      T8, 0x0022 (SP)
LUI     A0, 0x8034
LH      T7, 0x0012 (T6)
ADDIU   A0, A0, 0xCA7C
; etc

; thread5_game_loop troll
.org 0x80248B98
LUI     AT, 0x8034
LW      T7, 0xB078 (AT)
BEQ     T7, R0, 0x80248BB0
NOP
JAL     0x80323A60
ADDIU   A0, AT, 0xAF78
JAL     get_frame_count
NOP
JAL     0x802494D8
NOP
JAL     0x80247FDC
NOP
JAL     0x80248638
NOP
JAL     0x803805C8
LW      A0, 0x0024 (SP)
JAL     0x80248090
SW      V0, 0x0024 (SP)

; memory.c
.org 0x80278498
.area 0x80278504-0x80278498
.importobj "AI/trolls/main_pool_pop_state.o"
.endarea

.org 0x8027868C
.area 0x802786F0-0x8027868C
.importobj "AI/trolls/load_segment.o"
.endarea

.org 0x802787D8
.area 0x802788B4-0x802787D8
.importobj "AI/trolls/load_segment_decompress.o"
.endarea

; level_update.c
.org 0x8024A374
J       troll_check_instant_warp
NOP

.org 0x8024B13C
.area 0x8024B390-0x8024B13C
.importobj "AI/trolls/update_hud_values.o"
.endarea

; interaction.c
.org 0x8024D998
.area 0x8024DAAC-0x8024D998
.importobj "AI/trolls/take_damage_and_knock_back.o"
.endarea

.org 0x8024DB2C
.area 0x8024DBF0-0x8024DB2C
.importobj "AI/trolls/interact_coin.o"
.endarea

.org 0x8024E420
.area 0x8024E6EC-0x8024E420
.importobj "AI/trolls/interact_door.o"
.endarea

.org 0x8024FF04
.area 0x80250098-0x8024FF04
.importobj "AI/trolls/check_read_sign.o"
.endarea

.org 0x80250098
.area 0x80250198-0x80250098
.importobj "AI/trolls/check_npc_talk.o"
.endarea

; mario.c
.org 0x8025267C
JAL     triple_jump_set_mario_y_vel_based_on_fspeed
.org 0x802527B8
JAL     sideflip_set_mario_y_vel_based_on_fspeed

.org 0x80252CF4
.area 0x80252E5C-0x80252CF4
.importobj "AI/trolls/set_mario_action.o"
.endarea

.org 0x80254830
.area 0x80254B20-0x80254830
.importobj "AI/trolls/execute_mario_action.o"
.endarea

.org 0x80254B20
.area 0x80254F44-0x80254B20
.importobj "AI/trolls/init_mario.o"
.endarea

.org 0x80255d88
.area 0x80255ec4-0x80255d88
.importobj "AI/trolls/perform_ground_step.o"
.endarea

.org 0x80265df8
.area 0x80266038-0x80265df8
.importobj "AI/trolls/push_or_sidle_wall.o"
.endarea

.org 0x8026b17c
.area 0x8026b444-0x8026b17c
.importobj "Trolls/Mario/update_flying.o"
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

; mario_misc.c
.org 0x802766B4
.area 0x802767B8-0x802766B4
.importobj "AI/trolls/toad_message_talking.o"
.endarea

.org 0x80276910
.area 0x80276AA0-0x80276910
.importobj "AI/trolls/bhv_toad_message_init.o"
.endarea

; area.c
.org 0x8027B0C0
.area 0x8027B164-0x8027B0C0
.importobj "AI/trolls/change_area.o"
.endarea

; camera.c
.org 0x80283AF8
.area 0x80284CB8-0x80283AF8
.import "AI/trolls/mode_default_camera.bin"
.endarea

.org 0x80287BE0
.area 0x80287CB8-0x80287BE0
.importobj "AI/trolls/create_camera.o"
.endarea

.org 0x802892D8
.area 0x8028935C-0x802892D8
.importobj "AI/trolls/is_within_100_units_of_mario.o"
.endarea

.org 0x80289F88
.area 0x8028A080-0x80289F88
.importobj "AI/trolls/clamp_positions_and_find_yaw.o"
.endarea

.org 0x8028EEB0
.area 0x8028F670-0x8028EEB0
.importobj "AI/trolls/camera_course_processing.o"
.endarea

; level_update.c
; level_trigger_warp
.org 0x8024AB88
LUI     AT, 0x8034

.org 0x8024ABC0
ORI     A0, R0, 0x000B
ORI     A1, R0, 0x0014
SH      A1, 0xB254 (AT)
OR      A2, R0, R0
OR      A3, R0, R0
JAL     0x8027B1A0
SW      R0, 0x0010 (SP)
JAL     on_death_barrier_warp
NOP

; object_list_processor.c
.org 0x8029CFB0
.area 0x8029D1E8-0x8029CFB0
.importobj "AI/trolls/spawn_objects_from_info.o"
.endarea

; rendering_graph_node.c
.org 0x8027BF58
.area 0x8027C114-0x8027BF58
.importobj "AI/trolls/geo_process_camera.o"
.endarea

.org 0x8027CF38
.area 0x8027D0B8-0x8027CF38
.importobj "AI/trolls/geo_set_animation_globals.o"
.endarea

.org 0x8027DA10
.area 0x8027DA84-0x8027DA10
.importobj "AI/trolls/geo_process_object_parent.o"
.endarea

; load_area
.org 0x8027AF30
JAL     postObjectLoadPass
NOP

; render_game
.org 0x8027B3B4
.area 0x8027B6C0-0x8027B3B4
.importobj "AI/trolls/render_game.o"
.endarea

; object_helpers.c
.org 0x802A4960
.area 0x802A4BE4-0x802A4960
.importobj "AI/trolls/cur_obj_update_dialog.o"
.endarea

.org 0x802A1A18
.area 0x802A1B34-0x802A1A18
.importobj "AI/trolls/obj_spawn_loot_coins.o"
.endarea

.org 0x802A2C5C
.area 0x802A2ED4-0x802A2C5C
.importobj "AI/trolls/cur_obj_follow_path.o"
.endarea

.org 0x802A3B40
.area 0x802A3C18-0x802A3B40
.importobj "AI/trolls/cur_obj_scale_over_time.o"
.endarea

.org 0x802A958C
.area 0x802A9708-0x802A958C
.importobj "AI/trolls/common_anchor_mario_behavior.o"
.endarea

; screen_transition.c
.org 0x802CB5C0
.area 0x802CB640-0x802CB5C0
.importobj "AI/trolls/set_and_reset_transition_fade_timer.o"
.endarea

; skybox.c
; create_skybox_facing_camera
; instead of the JRB star check, the skybox darkens at night mode, if the background wasn't swapped for another
.org 0x802CFF48
LUI     AT, ((nightMode + 0x8000) >> 16)
LBU     T7, (nightMode & 0xFFFF) (AT)
BEQ     T7, R0, 0x802CFF7C
LBU     T7, (overrideBgId & 0xFFFF) (AT)
ADDIU   T7, T7, -0xFF
BNE     T7, R0, 0x802CFF7C
NOP
B       0x802CFF78
NOP

; save_file.c
.org 0x802794A0
.area 0x8027951C-0x802794A0
.importobj "AI/trolls/save_main_menu_data.o"
.endarea

.org 0x80279840
.area 0x802798FC-0x80279840
.importobj "AI/trolls/save_file_do_save.o"
.endarea

.org 0x802799DC
.area 0x80279BC8-0x802799DC
.importobj "AI/trolls/save_file_load_all.o"
.endarea

.org 0x8027A010
.area 0x8027A0A8-0x8027A010
.importobj "AI/trolls/save_file_get_total_star_count.o"
.endarea

; mov texture
.org 0x802D0254
.area 0x802D0484-0x802D0254
.importobj "AI/trolls/movtex_make_quad_vertex.o"
.endarea

; obj_behaviors_2.c
.org 0x802FB3DC
.area 0x802FB518-0x802FB3DC
.importobj "AI/trolls/obj_die_if_above_lava_and_health_non_positive.o"
.endarea

; load.c
.org 0x803190F4
.area 0x80319220-0x803190F4
.importobj "AI/trolls/load_banks_immediate.o"
.endarea

.org 0x8031B830
.area 0x8031B940-0x8031B830
.incbin "AI/trolls/sequence_channel_init.bin"
.endarea

.org 0x8031D08C
.area 0x8031D9EC-0x8031D08C
.importobj "AI/trolls/sequence_channel_process_script.o"
.endarea

.org 0x8031D9EC
.area 0x8031E240-0x8031D9EC
.importobj "AI/trolls/sequence_player_process_sequence.o"
.endarea

; external.c
.org 0x80322230
.area 0x8032231C-0x80322230
.importobj "AI/trolls/sound_reset.o"
.endarea

; sound_init.c
.org 0x80249178
.area 0x8024922C-0x80249178
.importobj "AI/trolls/set_background_music.o"
.endarea


; Engine segment
.headersize 0x80283280

; behavior_script.c
.org 0x803854CC
.area 0x8038556C-0x803854CC
.importobj "AI/trolls/bhv_cmd_begin.o"
.endarea

; geo_layout.c
.org 0x8037DE94
.area 0x8037DEF8-0x8037DE94
.importobj "AI/trolls/geo_layout_cmd_node_background.o"
.endarea

; level_script.c
.org 0x8037EE48
.area 0x8037EEA8-0x8037EE48
.importobj "AI/trolls/level_script/level_cmd_init_level.o"
.endarea

.org 0x8037F010
.area 0x8037F130-0x8037F010
.importobj "AI/trolls/level_script/level_cmd_begin_area.o"
.endarea

.org 0x8037F164
.area 0x8037F214-0x8037F164
.importobj "AI/trolls/level_script/level_cmd_load_model_from_dl.o"
.endarea

.org 0x8037F214
.area 0x8037F2A4-0x8037F214
.importobj "AI/trolls/level_script/level_cmd_load_model_from_geo.o"
.endarea

.org 0x8037F790
.area 0x8037F920-0x8037F790
.importobj "AI/trolls/level_script/level_cmd_create_instant_warp.o"
.endarea

; surface_load.c
.org 0x803839CC
.area 0x80383B70-0x803839CC
.importobj "AI/trolls/load_object_collision_model.o"
.endarea
