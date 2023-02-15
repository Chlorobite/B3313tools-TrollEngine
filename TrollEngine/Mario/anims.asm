; Animation data gaming
.orga 0x7CD000
.area 0x7D7000-0x7CD000

.headersize 0-orga() ; the pointers are relative to the start of the anim
.definelabel @anim_d1_start, orga()
.importobj "Mario/Anims/anim_D1_climbing.o"
.definelabel @anim_d1_size, orga()-@anim_d1_start

.headersize 0-orga()
.definelabel @anim_d2_start, orga()
.importobj "Mario/Anims/anim_D2_climbing_down.o"
.definelabel @anim_d2_size, orga()-@anim_d2_start

.headersize 0-orga()
.definelabel @anim_d3_start, orga()
.importobj "Mario/Anims/anim_D3_climbing_left.o"
.definelabel @anim_d3_size, orga()-@anim_d3_start

.headersize 0-orga()
.definelabel @anim_d4_start, orga()
.importobj "Mario/Anims/anim_D4_climbing_right.o"
.definelabel @anim_d4_size, orga()-@anim_d4_start

.headersize 0-orga()
.definelabel @anim_d5_start, orga()
.importobj "Mario/Anims/anim_D5_squatkick.o"
.definelabel @anim_d5_size, orga()-@anim_d5_start

.endarea

; Animation patchable table: the continuation
; place this in some unused space in ram, let's say uhhh lvl_init_or_update why not
; (this location depends on frameskip engine to work)
.headersize 0x80245000
.org 0x8024BCE0
.area 0x8024BD5C-0x8024BCE0

.definelabel mario_patchable_table_TWO, org()
.word @anim_d1_start, @anim_d1_size
.word @anim_d2_start, @anim_d2_size
.word @anim_d3_start, @anim_d3_size
.word @anim_d4_start, @anim_d4_size
.word @anim_d5_start, @anim_d5_size
; wiping old data, nothing to see here
.word 0, 0
.word 0, 0

.endarea
