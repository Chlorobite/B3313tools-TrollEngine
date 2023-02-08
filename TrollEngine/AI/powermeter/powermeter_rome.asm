; random free spaces in rom, hopefully nothing breaks
.definelabel sonk_start, 0x1C4300 ; size: (9*64*64*2)
.definelabel broll2_start, 0x201410 ; size: (9*64*64*2)
.definelabel broll1_start, 0x16D900 ; size: (9*64*64*2)
.definelabel vanilla_start, 0x145F00 ; size: (8*32*32*2)
.definelabel hud_numbers_start, vanilla_start+(8*32*32*2) ; size: (2*10*16*16*2)
.definelabel hud_stars_start, 0x13BA00 ; size: (2*16*16*2)
.definelabel hud_crosses_start, hud_stars_start+(2*16*16*2) ; size: (3*16*16*2)
.definelabel tex_stars_start, hud_crosses_start+(3*16*16*2) ; size: (2*32*32*8)

.orga tex_stars_start
.area (2*32*32*8*2),0x00
.incbin "AI/powermeter/star/star_yellow.bin"
.incbin "AI/powermeter/star/star_red.bin"
.endarea
.orga hud_crosses_start
.area (3*16*16*2),0x00
.incbin "AI/powermeter/hud/brollcross.bin"
.incbin "AI/powermeter/hud/sonkcross.bin"
.incbin "AI/powermeter/hud/vanillacross.bin"
.endarea
.orga hud_stars_start
.area (2*16*16*2),0x00
.incbin "AI/powermeter/hud/brollstar.bin"
.incbin "AI/powermeter/hud/sonkstar.bin"
.endarea
.orga hud_numbers_start
.area (2*10*16*16*2),0x00
.incbin "AI/powermeter/hud/betanum.bin"
.incbin "AI/powermeter/hud/vanillanum.bin"
.endarea
.orga vanilla_start
.area (8*32*32*2),0x00
.importobj "AI/powermeter/vanilla/vanilla.o"
.endarea
.orga broll1_start
.area (9*64*64*2),0x00
.importobj "AI/powermeter/broll/broll.o"
.endarea
.orga broll2_start
.area (9*64*64*2),0x00
.importobj "AI/powermeter/broll2/broll2.o"
.endarea
.orga sonk_start
.area (9*64*64*2),0x00
.importobj "AI/powermeter/sonk/sonk.o"
.endarea

; restore vanilla power meter left/right textures
.orga 0xAB2450+0x233E0
.importobj "AI/powermeter/vanilla/vanilla_sides.o"

; undo the vanilla power meter's transparency, as we use realbeta for broll and sonk now
.orga 0xADB8E0
.word 0xFCFFFFFF, 0xFFFCF279, 0xB900031D, 0x0F0A7008
