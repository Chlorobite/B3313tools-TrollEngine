; Some macros for Fast3D commands relating to 2D drawing

.macro G_NOOP
	.word 0x00000000
	.word 0x00000000
.endmacro

.macro G_RDPLOADSYNC
	.word 0xE6000000
	.word 0x00000000
.endmacro

.macro G_RDPPIPESYNC
	.word 0xE7000000
	.word 0x00000000
.endmacro

.macro G_RDPTILESYNC
	.word 0xE8000000
	.word 0x00000000
.endmacro

.macro G_RDPFULLSYNC
	.word 0xE9000000
	.word 0x00000000
.endmacro

.macro G_ENDDL
	.word 0xB8000000
	.word 0x00000000
.endmacro

.definelabel G_CYC_1CYCLE, 0
.definelabel G_CYC_2CYCLE, 1
.definelabel G_CYC_COPY, 2
.definelabel G_CYC_FILL, 3

.macro G_SET_CYCLE_TYPE, cycleType
	.word 0xBA001402
	.word ( cycleType << 20 )
.endmacro

.macro G_SETFILLCOLOR, colour
	.word 0xF7000000
	.word colour
.endmacro

; color: RGBA5551 format (red/green/blue from 0 to 31, alpha is 0 or 1)
.macro G_SETFILLCOLOR_RGBA5551, red, green, blue, alpha
	.word 0xF7000000
	.halfword ( (( red & 0x1F ) << 11) | (( green & 0x1F ) << 6) | (( blue & 0x1F ) << 1) | ( alpha & 1 ) )
	.halfword ( (( red & 0x1F ) << 11) | (( green & 0x1F ) << 6) | (( blue & 0x1F ) << 1) | ( alpha & 1 ) )
.endmacro

; (x1,y1) = upper-left corner
; (x2,y2) = lower-right corner
.macro G_FILLRECT, x1, y1, x2, y2
	.word ( 0xF6000000 | ( ( x2 & 0xFFF ) << 0xC ) | ( y2 & 0xFFF ) )
	.word ( ( ( x1 & 0xFFF ) << 0xC ) | ( y1 & 0xFFF ) )
.endmacro
