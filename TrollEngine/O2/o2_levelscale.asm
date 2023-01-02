.include "O2/o2_generic.asm"


; Engine segment
.headersize 0x80283280

; surface_collision.c
;.org 0x80381038
;.area 0x80381260-0x80381038
;.importobj "O2/surface_collision/scale/find_ceil_from_list.o"
;.endarea

;.org 0x8038156C
;.area 0x80381790-0x8038156C
;.importobj "O2/surface_collision/scale/find_floor_from_list.o"
;.endarea

.org 0x80381F08
.area 0x80382490-0x80381F08
.importobj "O2/surface_collision/scale/find_ceil_from_list.o"
.importobj "O2/surface_collision/scale/find_floor_from_list.o"
.endarea

.org 0x80380E8C
.area 0x80381038-0x80380E8C
.importobj "O2/surface_collision/scale/find_wall_collisions.o"
.endarea

.org 0x80381264
.area 0x80381470-0x80381264
.importobj "O2/surface_collision/scale/find_ceil.o"
.endarea

.org 0x80381900
.area 0x80381BA0-0x80381900
.importobj "O2/surface_collision/scale/find_floor.o"
.endarea


.org 0x80381BA0
.area 0x80381D3C-0x80381BA0
.importobj "O2/surface_collision/scale/find_water_level.o"
.endarea

.org 0x80381D3C
.area 0x80381EC8-0x80381D3C
.importobj "O2/surface_collision/scale/find_poison_gas_level.o"
.endarea

; surface_load.c
.org 0x80383228
.area 0x80383340-0x80383228
.importobj "O2/surface_load/scale/load_environmental_regions.o"
.endarea

.org 0x80383614
.area 0x80383828-0x80383614
.importobj "O2/surface_load/scale/transform_object_vertices.o"
.endarea



; Game segment
.headersize 0x80245000

; camera.c
.org 0x802826A0
.area 0x80282C0C-0x802826A0
.importobj "O2/camera/scale/update_boss_fight_camera.o"
.endarea

.org 0x80288f5c
.area 0x80289198-0x80288f5c
.importobj "O2/camera/scale/collide_with_walls.o"
.endarea

.org 0x8028a4ec
.area 0x8028a6bc-0x8028a4ec
.importobj "O2/camera/scale/is_behind_surface.o"
.endarea

; rendering_graph_node.c
.org 0x8027B904
.area 0x8027BA00-0x8027B904
.importobj "O2/rendering_graph_node/scale/geo_append_display_list.o"
.endarea

; external.c
.org 0x803208EC
.area 0x80320D70-0x803208EC
.importobj "O2/audio_external/scale/process_level_music_dynamics.o"
.endarea

; shadow.c
.org 0x802CD6C4
.area 0x802CD938-0x802CD6C4
.importobj "O2/shadow/scale/init_shadow.o"
.endarea
