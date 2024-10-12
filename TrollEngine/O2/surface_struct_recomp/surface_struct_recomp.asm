; Game segment
.headersize 0x80245000

; behaviors
.org 0x802C5B54
.area 0x802C5CA8-0x802C5B54
.importobj "O2/surface_struct_recomp/handle_merry_go_round_music.o"
.endarea

.org 0x802ba608
.area 0x802ba7e0-0x802ba608
.importobj "O2/surface_struct_recomp/bhv_rotating_clock_arm_loop.o"
.endarea

.org 0x802bd680
.area 0x802bd8d0-0x802bd680
.importobj "O2/surface_struct_recomp/bhv_koopa_shell_loop.o"
.endarea

.org 0x802ab860
.area 0x802aba40-0x802ab860
.importobj "O2/surface_struct_recomp/bhv_coin_loop.o"
.endarea

.org 0x802b6190
.area 0x802b6568-0x802b6190
.importobj "O2/surface_struct_recomp/bowser_act_jump_onto_stage.o"
.endarea

; camera.c :3
.org 0x80280368
.area 0x802804f4-0x80280368
.importobj "O2/surface_struct_recomp/look_down_slopes.o"
.endarea

.org 0x8028a0f4
.area 0x8028a4ec-0x8028a0f4
.importobj "O2/surface_struct_recomp/is_surf_within_bounding_box.o"
.endarea

.org 0x8028f914
.area 0x8028fc9c-0x8028f914
.importobj "O2/surface_struct_recomp/rotate_camera_around_walls.o"
.endarea

.org 0x802984b4
.area 0x802987b0-0x802984b4
.importobj "O2/surface_struct_recomp/cutscene_enter_painting.o"
.endarea

; mario.c
.org 0x8025177c
.area 0x802518a8-0x8025177c
.importobj "O2/surface_struct_recomp/mario_get_floor_class.o"
.endarea

.org 0x802518a8
.area 0x80251a48-0x802518a8
.importobj "O2/surface_struct_recomp/mario_get_terrain_sound_addend.o"
.endarea

.org 0x80251bd4
.area 0x80251cfc-0x80251bd4
.importobj "O2/surface_struct_recomp/mario_floor_is_slippery.o"
.endarea

.org 0x80251cfc
.area 0x80251e24-0x80251cfc
.importobj "O2/surface_struct_recomp/mario_floor_is_slope.o"
.endarea

.org 0x80251e24
.area 0x80251f24-0x80251e24
.importobj "O2/surface_struct_recomp/mario_floor_is_steep.o"
.endarea

.org 0x80253a60
.area 0x80253d58-0x80253a60
.importobj "O2/surface_struct_recomp/update_mario_geometry_inputs.o"
.endarea

; mario_step.c
.org 0x8032daf0
.area 0x8032db30-0x8032daf0
.importobj "O2/surface_struct_recomp/mario_step_data.o"
.endarea

.org 0x802552fc
.area 0x80255414-0x802552fc
.importobj "O2/surface_struct_recomp/mario_bonk_reflection.o"
.endarea

.org 0x80255414
.area 0x80255654-0x80255414
.importobj "O2/surface_struct_recomp/mario_update_quicksand.o"
.endarea

.org 0x8025570c
.area 0x8025580c-0x8025570c
.importobj "O2/surface_struct_recomp/mario_update_moving_sand.o"
.endarea

.org 0x80255B04
.area 0x80255D88-0x80255B04
.importobj "O2/surface_struct_recomp/perform_ground_quarter_step.o"
.endarea

.org 0x80255ec4
.area 0x802560ac-0x80255ec4
.importobj "O2/surface_struct_recomp/check_ledge_grab.o"
.endarea

.org 0x802560ac
.area 0x802564e0-0x802560ac
.importobj "O2/surface_struct_recomp/perform_air_quarter_step.o"
.endarea

.org 0x802569f8
.area 0x80256b24-0x802569f8
.importobj "O2/surface_struct_recomp/apply_vertical_wind.o"
.endarea

; object_helpers.c
.org 0x8029dbd4
.area 0x8029dcd4-0x8029dbd4
.importobj "O2/surface_struct_recomp/geo_switch_area.o"
.endarea

.org 0x802A07E8
.area 0x802A0AB0-0x802A07E8
.importobj "O2/surface_struct_recomp/cur_obj_move_xz.o"
.endarea

.org 0x802A1D7C
.area 0x802A1F3C-0x802A1D7C
.importobj "O2/surface_struct_recomp/cur_obj_detect_steep_floor.o"
.endarea

.org 0x802A1F3C
.area 0x802A20F4-0x802A1F3C
.importobj "O2/surface_struct_recomp/cur_obj_resolve_wall_collisions.o"
.endarea

.org 0x802a4120
.area 0x802a4210-0x802a4120
.importobj "O2/surface_struct_recomp/cur_obj_update_floor.o"
.endarea

.org 0x802a4f58
.area 0x802a5034-0x802a4f58
.importobj "O2/surface_struct_recomp/cur_obj_align_gfx_with_floor.o"
.endarea

; mario_actions_moving.c
.org 0x802640fc
.area 0x802642b4-0x802640fc
.importobj "O2/surface_struct_recomp/check_ledge_climb_down.o"
.endarea

.org 0x8026440c
.area 0x80264740-0x8026440c
.importobj "O2/surface_struct_recomp/update_sliding_angle.o"
.endarea

.org 0x80264b54
.area 0x80264d80-0x80264b54
.importobj "O2/surface_struct_recomp/apply_slope_accel.o"
.endarea

.org 0x80264e18
.area 0x80265080-0x80264e18
.importobj "O2/surface_struct_recomp/update_shell_speed.o"
.endarea

.org 0x80265244
.area 0x80265458-0x80265244
.importobj "O2/surface_struct_recomp/update_walking_speed.o"
.endarea

.org 0x80265620
.area 0x80265700-0x80265620
.importobj "O2/surface_struct_recomp/begin_braking_action.o"
.endarea

.org 0x80267504
.area 0x80267728-0x80267504
.importobj "O2/surface_struct_recomp/act_riding_shell_ground.o"
.endarea

.org 0x80267ce4
.area 0x80267fa4-0x80267ce4
.importobj "O2/surface_struct_recomp/common_slide_action.o"
.endarea

.org 0x80268dcc
.area 0x80268f78-0x80268dcc
.importobj "O2/surface_struct_recomp/common_landing_action.o"
.endarea

.org 0x80268f78
.area 0x80269108-0x80268f78
.importobj "O2/surface_struct_recomp/common_landing_cancels.o"
.endarea

; mario_actions_airborne.c
.org 0x8026a12c
.area 0x8026a224-0x8026a12c
.importobj "O2/surface_struct_recomp/lava_boost_on_wall.o"
.endarea

.org 0x8026a224
.area 0x8026a400-0x8026a224
.importobj "O2/surface_struct_recomp/check_fall_damage.o"
.endarea

.org 0x8026a62c
.area 0x8026a818-0x8026a62c
.importobj "O2/surface_struct_recomp/check_horizontal_wind.o"
.endarea

.org 0x8026be78
.area 0x8026bf40-0x8026be78
.importobj "O2/surface_struct_recomp/act_long_jump.o"
.endarea

.org 0x8026de98
.area 0x8026e088-0x8026de98
.importobj "O2/surface_struct_recomp/act_butt_slide_air.o"
.endarea

.org 0x8026e088
.area 0x8026e2b4-0x8026e088
.importobj "O2/surface_struct_recomp/act_hold_butt_slide_air.o"
.endarea

.org 0x8026e2b4
.area 0x8026e59c-0x8026e2b4
.importobj "O2/surface_struct_recomp/act_lava_boost.o"
.endarea

.org 0x8026fa18
.area 0x8026fb04-0x8026fa18
.importobj "O2/surface_struct_recomp/check_common_airborne_cancels.o"
.endarea

; mario_actions_automatic.c
.org 0x8025eb50
.area 0x8025ecfc-0x8025eb50
.importobj "O2/surface_struct_recomp/perform_hanging_step.o"
.endarea

.org 0x8025ecfc
.area 0x8025eed0-0x8025ecfc
.importobj "O2/surface_struct_recomp/update_hang_moving.o"
.endarea

.org 0x8025ef58
.area 0x8025f0b4-0x8025ef58
.importobj "O2/surface_struct_recomp/act_start_hanging.o"
.endarea

.org 0x8025f0b4
.area 0x8025f1e4-0x8025f0b4
.importobj "O2/surface_struct_recomp/act_hanging.o"
.endarea

.org 0x8025f1e4
.area 0x8025f384-0x8025f1e4
.importobj "O2/surface_struct_recomp/act_hang_moving.o"
.endarea

.org 0x8025f6c0
.area 0x8025f970-0x8025f6c0
.importobj "O2/surface_struct_recomp/act_ledge_grab.o"
.endarea

; mario_actions_stationary.c
.org 0x802608b0
.area 0x80260aac-0x802608b0
.importobj "O2/surface_struct_recomp/check_common_idle_cancels.o"
.endarea

.org 0x80260aac
.area 0x80260cb4-0x80260aac
.importobj "O2/surface_struct_recomp/check_common_hold_idle_cancels.o"
.endarea

.org 0x802635e8
.area 0x80263784-0x802635e8
.importobj "O2/surface_struct_recomp/act_first_person.o"
.endarea

; mario_actions_submerged.c
.org 0x80270500
.area 0x80270918-0x80270500
.importobj "O2/surface_struct_recomp/apply_water_current.o"
.endarea

.org 0x80273618
.area 0x802737F4-0x80273618
.importobj "O2/surface_struct_recomp/update_metal_water_walking_speed.o"
.endarea

; etc
.org 0x802e41a4
.area 0x802e42e0-0x802e41a4
.importobj "O2/surface_struct_recomp/turn_obj_away_from_steep_floor.o"
.endarea

.org 0x802e445c
.area 0x802e4814-0x802e445c
.importobj "O2/surface_struct_recomp/calc_new_obj_vel_and_pos_y.o"
.endarea

.org 0x802e4814
.area 0x802e4cec-0x802e4814
.importobj "O2/surface_struct_recomp/calc_new_obj_vel_and_pos_y_underwater.o"
.endarea

.org 0x802e5a80
.area 0x802e5b18-0x802e5a80
.importobj "O2/surface_struct_recomp/obj_check_floor_death.o"
.endarea

.org 0x802e0120
.area 0x802e048c-0x802e0120
.importobj "O2/surface_struct_recomp/envfx_set_lava_bubble_position.o"
.endarea

.org 0x802c89f0
.area 0x802c8b4c-0x802c89f0
.importobj "O2/surface_struct_recomp/update_mario_platform.o"
.endarea

.org 0x802507fc
.area 0x80250940-0x802507fc
.importobj "O2/surface_struct_recomp/mario_handle_special_floors.o"
.endarea

.org 0x8025a9ac
.area 0x8025ae0c-0x8025a9ac
.importobj "O2/surface_struct_recomp/act_squished.o"
.endarea

.org 0x8025D70C
.area 0x8025D798-0x8025D70C
.importobj "O2/surface_struct_recomp/check_for_instant_quicksand.o"
.endarea

.org 0x8024a7b4
.area 0x8024a85c-0x8024a7b4
.importobj "O2/surface_struct_recomp/get_painting_warp_node.o"
.endarea

.org 0x802e405c
.area 0x802e41a4-0x802e405c
.importobj "O2/surface_struct_recomp/obj_find_wall.o"
.endarea


; Engine segment
.headersize 0x80283280

.org 0x803824F8
.area 0x80382590-0x803824F8
.importobj "O2/surface_struct_recomp/alloc_surface.o"
.endarea

.org 0x80382A2C
.area 0x80382B7C-0x80382A2C
.importobj "O2/surface_struct_recomp/add_surface.o"
.endarea

.org 0x80383068
.area 0x803831D0-0x80383068
.importobj "O2/surface_struct_recomp/load_static_surfaces.o"
.endarea
