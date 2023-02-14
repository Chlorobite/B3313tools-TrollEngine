; The purpose of O2 

; Engine segment
.headersize 0x80283280

; surface_collision.c
.org 0x80380690
.area 0x80380DE8-0x80380690
.importobj "O2/surface_collision/find_wall_collisions_from_list.o"
.endarea

.org 0x803814B8
.area 0x8038156C-0x803814B8
.importobj "O2/surface_collision/find_floor_height_and_data.o"
.endarea

; surface_load.c
.org 0x803825FC
.area 0x8038283C-0x803825FC
.importobj "O2/surface_load/add_surface_to_cell.o"
.endarea

.org 0x803828FC
.area 0x80382990-0x803828FC
.importobj "O2/surface_load/lower_cell_index.o"
.endarea

.org 0x80382990
.area 0x80382A2C-0x80382990
.importobj "O2/surface_load/upper_cell_index.o"
.endarea

.org 0x80382B7C
.area 0x80382F84-0x80382B7C
.importobj "O2/surface_load/read_surface_data.o"
.endarea

.org 0x80383828
.area 0x803839CC-0x80383828
.importobj "O2/surface_load/load_object_surfaces.o"
.endarea

; behavior_script.c
.org 0x80383BB0
.area 0x80383CB4-0x80383BB0
.importobj "O2/behavior_script/random_u16.o"
.endarea




; Game segment
.headersize 0x80245000

; interaction.c
.org 0x8024CC7C
.area 0x8024CE08-0x8024CC7C
.importobj "O2/interaction/mario_check_object_grab.o"
.endarea

; camera.c
.org 0x8027FC18
.area 0x8027FE20-0x8027FC18
.importobj "O2/camera/calc_y_to_curr_floor.o"
.endarea

.org 0x8028FC9C
.area 0x8028FE24-0x8028FC9C
.importobj "O2/camera/find_mario_floor_and_ceil.o"
.endarea

.org 0x8028A6BC
.area 0x8028A7EC-0x8028A6BC
.importobj "O2/camera/is_range_behind_surface.o"
.endarea

;.org 0x80283AF8
;.area 0x80284CB8-0x80283AF8
;.importobj "O2/camera/update_default_camera.o"
;.endarea

; geo_misc.c
.org 0x802D2210
.area 0x802D22C4-0x802D2210
.importobj "O2/geo_misc/make_vertex.o"
.endarea

.org 0x802D22C4
.area 0x802D2360-0x802D22C4
.include "O2/geo_misc/round_float.asm"
.endarea

; shadow.c
.org 0x802CD388
.area 0x802CD444-0x802CD388
.importobj "O2/shadow/scale_shadow_with_distance.o"
.endarea

.org 0x802CD48C
.area 0x802CD614-0x802CD48C
.importobj "O2/shadow/dim_shadow_with_distance.o"
.endarea

;.org 0x802CD938
;.area 0x802CD988-0x802CD938
.definelabel get_texture_coords_9_vertices, 0x802CD938
;.importobj "O2/shadow/get_texture_coords_9_vertices.o"
;.endarea

;.org 0x802CD988
;.area 0x802CD9EC-0x802CD988
.definelabel get_texture_coords_4_vertices, 0x802CD988
;.importobj "O2/shadow/get_texture_coords_4_vertices.o"
;.endarea

.org 0x802CD9EC
.area 0x802CDB20-0x802CD9EC
.importobj "O2/shadow/make_shadow_vertex_at_xyz.o"
.endarea

.org 0x802CDC40
.area 0x802CDE94-0x802CDC40
.importobj "O2/shadow/calculate_vertex_xyz.o"
.endarea

.org 0x802CE128
.area 0x802CE2BC-0x802CE128
.importobj "O2/shadow/add_shadow_to_display_list.o"
.endarea

.org 0x802CF34C
.area 0x802CF5B0-0x802CF34C
.importobj "O2/shadow/create_shadow_below_xyz.o"
.endarea
; make_shadow_vertex: this calculation is redundant now, and breaks bowser's shadow. skip it
.org 0x802CDFDC
B       0x802CE0B4

; object_collision.c
.org 0x802C8FE4
.area 0x802C91EC-0x802C8FE4
.importobj "O2/object_collision/detect_object_hitbox_overlap.o"
.endarea

; memory.c
.org 0x80278AB8
.area 0x80278B28-0x80278AB8
.importobj "O2/memory/alloc_only_pool_alloc.o"
.endarea

.org 0x80278F2C
.area 0x80278FA0-0x80278F2C
.importobj "O2/memory/alloc_display_list.o"
.endarea

; rendering_graph_node.c
.org 0x8027CA70
.area 0x8027CF38-0x8027CA70
.importobj "O2/rendering_graph_node/geo_process_animated_part.o"
.endarea

.org 0x8027D6FC
.area 0x8027DA10-0x8027D6FC
.importobj "O2/rendering_graph_node/geo_process_object.o"
.endarea
