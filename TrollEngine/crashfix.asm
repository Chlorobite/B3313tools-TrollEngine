.definelabel set_vblank_handler, 0x80246b14
.definelabel gGameVblankQueue, 0x8033b010
.definelabel gGameVblankHandler, 0x8033b048
.definelabel thread5_game_loop, 0x80248af0

.headersize 0x8056C000-(0x3D0DC0+0xC000)
.orga 0x3D0DC0+0xC000
.importobj "AI/crash_screen.o"

.headersize 0x80245000

.org thread5_game_loop+0x28 // initialization code
.area 0x80248B40-(thread5_game_loop+0x28)
jal     crash_screen_init
nop
lui     $a1, 0x8034
addiu   $a2, $a1, (gGameVblankQueue - 0x80340000)
addiu   $a1, $a1, (gGameVblankHandler - 0x80340000)
addiu   $a0, $zero, 2
jal     set_vblank_handler
addiu   $a3, $zero, 1
jal     segmented_to_virtual
lui     $a0, 0x1000
.endarea

// insert into ram code coming soon
