; T0 = gCurrentObject
LUI     T0, 0x8036
LW      T0, 0x1160 (T0)
; T1 = bparams 1, 2 as a u16
LHU     T1, 0x188 (T0)
; T2 = T1 * 10
SLL     T2, T1, 2
ADDU    T2, T2, T1
SLL     T2, T2, 1
; F4 = (f32)T2
MTC1    T2, F4
NOP
CVT.S.W F4, F4
; F6 = levelScaleV
LUI     AT, ((levelScaleV + 0x8000) >> 16)
LWC1    F6, (levelScaleV & 0xFFFF) (AT)
NOP
; F4 *= F6
MUL.S   F4, F4, F6
NOP
; gCurrentObject->hitboxHeight = F4
SWC1    F4, 0x1FC (T0)
; return
JR      RA
NOP
