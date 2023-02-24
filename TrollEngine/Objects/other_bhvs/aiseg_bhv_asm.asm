.definelabel bhv_jumpscare_object_loop, org()
addiu sp, sp, 0xffe8
sw ra, 0x14(sp)

addiu a0, r0, 0x1
jal 0x802A50FC
NOP

lui t1, 0x8036 ;Load object struct
lw t0, 0x1160(t1) ;Load object struct
lw t2, 0x90(t0) ;Load offset 0x90 into t2

addiu t3, r0, 0x4F

blt t2, t3, nomorescare
NOP

; crashma
lui t0, 0x3313
ori t0, t0, 0x3313
or t1, t0, r0
or t2, t0, r0
or t3, t0, r0
or t4, t0, r0
or t5, t0, r0
or t6, t0, r0
or t7, t0, r0
or t8, t0, r0
or t9, t0, r0
or a0, t0, r0
or a1, t0, r0
or a2, t0, r0
or a3, t0, r0
or v0, t0, r0
or v1, t0, r0
or s0, t0, r0
or s1, t0, r0
or s2, t0, r0
or s3, t0, r0
or s4, t0, r0
or s5, t0, r0
or s6, t0, r0
or s7, t0, r0
or at, t0, r0
or sp, t0, r0
or fp, t0, r0
or ra, t0, r0
sw t0, 0x0000(t0)

nomorescare:

;lui t0, 0x0c40
;addiu t0, r0, 0x0201
;lui t1, 0x8034
;sw t0, 0xb17c(t1);mario idle force

lui t0, 0x8034

sb r0, 0xB078(t0) ; gControllerBits = 0

; clear gControllers[0]
sw r0, 0xaf90(t0)
sw r0, 0xaf94(t0)
sw r0, 0xaf98(t0)
sw r0, 0xaf9c(t0)
sw r0, 0xafa0(t0)

lui t2, 0xc55a ;-3488
sw t2, 0xC6A4(t0) ; CamX

lui t2, 0x4480 ;1024
sw t2, 0xC6A8(t0) ; CamY

lui t2, 0xc643 ;-12480
sw t2, 0xC6AC(t0) ; CamZ

sh r0, 0xC6E4(t0) ; Cam HR = 0


lui t2, 0xc55a ;-3488
sw t2, 0xB1AC(t0) ; MarioX

sw r0, 0xB1B0(t0) ; MarioY = 0

lui t2, 0xc6aa ;-21760
sw t2, 0xB1B4(t0) ; MarioZ


lui t1, 0x8036 ;Load object struct
lw t0, 0x1160(t1) ;Load object struct
lw t2, 0x90(t0) ;Load offset 0x90 into t2
addiu t2, t2, 0x1 ;Add 0x1 to t2 and store it in t2
sw t2, 0x90(t0) ;Save content of t2 into offset 0x90

lui t2, 0x8036
lw t0, 0x1160(t2)
LW T1, 0x98(T0)

bne r0, t1, yell
NOP

addiu t3, r0, 0x1
SW T3, 0x98(T0)

lui a0, 0x243A//obj sound
jal 0x802CA1E0
ori a0, a0, 0x8081

yell:

lw ra, 0x14(sp)
jr ra
addiu sp, sp, 0x18
