.headersize 0x807F7800-0x7d7800

; load 7d7800-7e0000 to 807F7800
;.orga 0xfb9d4
;load:
;SW RA, 0x0010 (SP)
;LUI T5, 0x8039
;LW T6, 0xBE28 (T5)
;addiu t7, t6, 0x0010
;sw t7, 0xbe28 (T5)
;LW A0, 0x0004 (T6)
;LW A1, 0x0008 (T6)
;JAL 0x802786F0
;LW A2, 0x000C (T6)


;lui a0, 0x807F
;ori a0, a0, 0x7800
;lui a1, 0x007D
;ori a1, a1, 0x7800
;lui a2, 0x007D
;jal 0x80278504
;ori a2, a2, 0xC800
;lw ra, 0x0010 (SP)
;jr ra
;nop


; dust is spawn o, 0xFF, 0x007F8100
; Ad5E8 jal 7f8180

; BEHAVIOR 007D8130

; iterate all DLs starting at 80420000
	; start of DL is marked with E7 00 00 00 00 00 00 00
	; end of DL is marked with B8 00 00 00 00 00 00 00
; while active, add 0xF2 20 00 00 + o->oTimer&F * 0x10000 to pointer
; to deactivate, subtract the last added number (store somewhere in struct)

.orga 0x7d7800
.area 0x7d8200-0x7d7800
.importobj "Objects/corrupt/kazeshit.o"
.endarea


.orga 0x7d8200
b:
addiu sp, sp, 0xffe8
sw ra, 0x14 (SP)
lui t0, 0x8036
lw t0, 0x1160(t0)
lw t2, 0x014c (t0)
beq t2, r0, reset
nop
action:
lw t2, 0x0154 (t0)
lui a0, 0x0001
addiu t3, r0, 0x0100
bne t2, t3, nosub
nop
lui a1, 0xFF00
sw r0, 0x0154 (t0)
nosub:
bne t2, r0, noadd
nop
lui a1, 0xF200
add a0, a1, a0
noadd:
jal corrupt_entryPoint
nop
lui t0, 0x8036
lw t0, 0x1160(t0)
lui at, 0xf220
lw t2, 0x0154 (t0)
addiu t2, t2, 0x1
sll t2, t2, 0x10
lui t3, 0xf200
or t2, t3, t2
sw t2, 0x00f0 (t0)

beq r0, r0, end
nop
reset:
lw t2, 0x0154 (t0)
bne t2, r0, end
nop
lui a0, 0x8034
jal 0x8027ae44
lbu a0, 0xb24a (a0)
;lw a0, 0x00f0 (t0)
;jal corrupt_entryPoint
;sub a0, r0, a0
end:

lui t0, 0x8036
lw t0, 0x1160(t0)
lwc1 f2, 0x015c (t0)
lbu t3, 0x0189 (t0)
addiu t3, t3, 0x1
sll t3, t3, 0x8
mtc1 t3, f4
nop
nop
cvt.s.w f4, f4
nop
nop
nop
c.lt.s f2, f4
sw r0, 0x014c (t0)
bc1f normal
addiu at, r0, 0x1
sw at, 0x014c (t0)
normal:
lw ra, 0x14 (SP)
jr ra
addiu sp, sp, 0x0018
