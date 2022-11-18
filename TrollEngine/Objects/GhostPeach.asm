;ghost peach

.definelabel GetHDist, 0x8029E27C ; a0 obj 1, a1 obj 2
.definelabel PlaySound, 0x802CA190 ; a0 sound and properties

.headersize 0x8018F240
.orga 0x3D0DC0+0x600

;behavior script
@@beh:
.word 0x00040000
.word 0x1101004b
.word 0x27260000, 0x0501C504
.word 0x28010000
.word 0x08000000
.word 0x0c000000, @gpe
.word 0x09000000
@gpe:

addiu sp, sp, 0xffe0
sw ra, 0x001c(sp)
sw s0, 0x0018(sp)
sw s1, 0x0014(sp)

lui s0, 0x8036
lw s1, 0x1158(s0)
lw s0, 0x1160(s0)

lb t0, 0x018b(s0)
bne t0, r0, @@MoteRoom
nop
jal @Ghost
nop
beq r0, r0, @@idle
nop
@@MoteRoom:
jal @motepeach
nop



@@idle:
lw s1, 0x0014(sp)
lw s0, 0x0018(sp)
lw ra, 0x001c(sp)
jr ra
addiu sp, sp, 0x0020

;peach in mote room
@motepeach:
addiu sp, sp, 0xffe0
sw ra, 0x001c(sp)
lui t0, 0x807d
lh t0, 0x0000(t0)
sll t0, t0, 0x1f
sra t0, t0, 0x1f
sb t0, 0x017f(s0)
bne t0, r0, @@nos
lui a0, 0x4004
jal PlaySound
ori a0, a0, 0xFF01
@@nos:
lw ra, 0x001c(sp)
jr ra
addiu sp, sp, 0x0020

@Ghost:
addiu sp, sp, 0xffe0
sw ra, 0x001c(sp)
lw t0, 0x0154(s0)
bne t0, r0, @@setup
addiu t0, r0, 0x00AA
sb t0, 0x017f(s0) ; transparency


@@setup:
or a0, s0, r0
jal GetHDist
or a1, s1, r0
lui t0, 0x442f
mtc1 t0, f4
lwc1 f6, 0x00f4(s0)
c.le.s f0, f4
lui t0, 0x4000
bc1f @@none
mtc1 t0, f8
lb t0, 0x0188(s0)
beq t0, r0, @@ic1
addiu t1, r0, 0x0001
beq t0, t1, @@ic2
nop
beq r0, r0, @@none
nop


@@ic1:
lw t0, 0x0154(s0)
slti t0, t0, 0x0020
bne t0, r0, @@none
lbu t0, 0x017f(s0)
addiu t0, t0, 0xFFFe
slt t1, t0, r0
bnel t1, r0, @@none
sw r0, 0x0074(s0)
beq r0, r0, @@none
sb t0, 0x017f(s0)
@@ic2:
lbu t0, 0x017f(s0)
addiu t0, t0, 0xFFFE
slt t1, t0, r0
bnel t1, r0, @@none
sw r0, 0x0074(s0)
sb t0, 0x017f(s0)
add.s f6, f6, f8
swc1 f6, 0x00f4(s0)



@@none:
lwc1 f0, 0x00f4(s0)
lwc1 f2, 0x00a4(s0)
add.s f0, f0, f2
swc1 f0, 0x00a4(s0)
lw ra, 0x001c(sp)
jr ra
addiu sp, sp, 0x0020
LogWheels:
