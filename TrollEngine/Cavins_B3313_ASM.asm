/* Boss object struct conventions:

0xF8: Interaction Flags A
0xF9: Interaction Flags B
0xFA: Interaction Flags C
0xFB: Interaction Flags D

0xFC: Action Variables

#define INT_GROUND_POUND_OR_TWIRL (1 << 0) // 0x01
#define INT_PUNCH                 (1 << 1) // 0x02
#define INT_KICK                  (1 << 2) // 0x04
#define INT_TRIP                  (1 << 3) // 0x08
#define INT_SLIDE_KICK            (1 << 4) // 0x10
#define INT_FAST_ATTACK_OR_SHELL  (1 << 5) // 0x20
#define INT_HIT_FROM_ABOVE        (1 << 6) // 0x40
#define INT_HIT_FROM_BELOW        (1 << 7) // 0x80

0x100: Target X
0x104: Target Y
0x108: Target Z

0x10A: Target pitch
0x10C: Target yaw

0x148: Action timer

0x1A4: Float variable?
0x1A8: Float variable?
0x1AC: Float variable?
0x1B0: Float variable?


Important functions:
JAL 0x8028AAD8		// Calc yaw
???

JAL 0x802E5160		// Move using fvel and yaw
OR A0, R0, S0

JAL 0x8029F070		// cur obj move using vel
NOP

JAL 0x802A25B4		// cur obj move using vel and gravity
NOP




*/

.orga 0x01203800
.area 0x1000

// Goomboss

.dw 0x00040000		// 0x403800
.dw 0x110120D1		// Set obj flags
.dd 0x272600000801DA4C	// Set animation model
.dw 0x28000000		// Animate
.dd 0x2300000000E00190	// Collision cylinder
.dd 0x2E00000000E00190	// Hurtbox cylinder
.dw 0x103E0002		// Damage to Mario
.dw 0x32000258		// Scale object
.dw 0x10050000		// Set collision timer
.dw 0x1E000000		// Drop to ground
.dw 0x08000000
.dd 0x0C00000080405100	// cavin_goomboss
.dw 0x09000000

// Goomboss minion

.dw 0x00040000		// 0x403844
.dw 0x110120C1		// Set obj flags
.dd 0x272600000801DA4C	// Set animation model
.dw 0x28000000		// Animate
.dd 0x23000000004800A0	// Collision cylinder
.dd 0x2E000000004800A0	// Hurtbox cylinder
.dw 0x103E0001		// Damage to Mario
.dw 0x32000096		// Scale object
.dw 0x10050000		// Set collision timer
.dw 0x1E000000		// Drop to ground
.dw 0x2D000000		// Set home
.dw 0x08000000
.dd 0x0C00000080405108	// cavin_goomboss_minion
.dw 0x09000000

// Toxic damage floor

.dw 0x00040000		// 0x40388C
.dw 0x08000000
.dd 0x0C00000080405110	// cavin_toxic_floor
.dw 0x09000000

// Water Rescue

.dw 0x00040000		// 0x4038A0
.dd 0x23000000004800A0	// Collision cylinder
.dd 0x272600000801DA4C	// Set animation model
.dw 0x28000000		// Animate
.dw 0x08000000
.dd 0x0C00000080405118	// cavin_water_rescue_start
.dw 0x09000000

// Stop Music in Radius

.dw 0x00040000		// 0x4038C8
.dw 0x11010040		// Set obj flags
.dw 0x08000000
.dd 0x0C00000080405120	// cavin_stop_music
.dw 0x09000000

// Break box give star

.dw 0x00020000		// 0x4038E0
.dw 0x11010449
.dw 0x1E000000		// Drop to ground
.dw 0x2D000000
.dd 0x0C000000802F5CD4	// init function
.dw 0x2D000000		// Declare home
.dw 0x08000000
.dw 0x10050000
.dd 0x0C000000802F6228	// box function
.dd 0x0C00000080405128	// cavin_break_box_star
.dw 0x09000000




/* Goomboss
variables:

0xFC: Minion count
0xFD: Minion timer

0x1A4: Temp storage
0x1A8: Turn amount

*/
cavin_goomboss_start:
ADDIU SP, SP, 0xFFE0
SW RA, 0x1C (SP)
SW S0, 0x10 (SP)
SW S1, 0x14 (SP)
SW S2, 0x18 (SP)
LUI S0, 0x8036
JAL 0x802A064C		// current object get floor height and data
LW S0, 0x1160 (S0)
SW V0, 0x1C0 (S0)	// Store floor struct
LW S1, 0x184 (S0)	// Load damage counter
LW S2, 0x150 (S0)	// Load subaction

LW T0, 0x14C (S0)	// Load action
ADDIU AT, R0, 0x1
BEQ T0, AT, cavin_goomboss_die
ADDIU AT, AT, 0x1
BEQ T0, AT, cavin_goomboss_walk
ADDIU AT, AT, 0x1
BEQ T0, AT, cavin_goomboss_damaged

LUI AT, 0x0001
ORI AT, AT, 0x8000
SW AT, 0x48 (S0)	// Store anim accel
SW R0, 0x184 (S0)	// Reset damage counter
ORI AT, R0, 0x2		// Set action to 2
SW AT, 0x14C (S0)	// Store action
LUI AT, 0x41A0		// 20.0f
SW AT, 0xB8 (S0)	// Store velH
ADDIU AT, R0, 0xFF60
BEQZ R0, cavin_goomboss_common
SW AT, 0x1A8 (S0)	// Store turn increment

cavin_goomboss_summon:
LBU T4, 0xFC (S0)	// Load minion count
ADDIU AT, T4, 0xFFFD	// Max minions of 3
BGEZ AT, cavin_goomboss_common
LBU T5, 0xFD (S0)	// Load minion timer
ADDIU T5, T5, 0x1
ADDIU AT, T5, 0xFF6A	// -96, 5 seconds
BLTZ AT, cavin_goomboss_common
SB T5, 0xFD (S0)	// Store minion timer
ADDIU T4, T4, 0x1
SB T4, 0xFC (S0)	// Store minion count
SB R0, 0xFD (S0)	// Reset minion timer
OR A0, R0, S0
ORI A1, R0, 0xC0	// model_goomba
LUI A2, 0x8040
JAL 0x8029EDCC		// Spawn child
ADDIU A2, A2, 0x3844
LUI AT, 0xC28C		// -70.0f
SW AT, 0xA0 (V0)	// Store child X pos
SW AT, 0xA8 (V0)	// Store child Z pos
LUI AT, 0xC2C8		// 100.0f
SW AT, 0xA4 (V0)	// Store child Y pos
LUI AT, 0xBFC0		// -1.5f
SW AT, 0xE4 (V0)	// Store gravity
LUI AT, 0x4220		// 40.0f
SW AT, 0xB0 (V0)	// Store vel Y
JAL 0x80383BB0		// Call RNG
SW V0, 0x1A4 (S0)
LW AT, 0x1A4 (S0)
SW V0, 0xC8 (AT)	// Store random move yaw
BEQZ R0, cavin_goomboss_common
SW V0, 0xD4 (AT)	// Store random face yaw


cavin_goomboss_die:
JAL 0x8024927C		// Fadeout level music
ORI A0, R0, 0x20
ORI A0, R0, 0x20
JAL 0x802A3D40		// Shake Y
ORI A1, R0, 0x10
BEQZ V0, cavin_goomboss_common
NOP
LWC1 F12, 0xA0 (S0)	// Pos X
LUI AT, 0x442F		// 700.0f
MTC1 AT, F14		// pos Y
JAL 0x802F2B88		// Spawn star
LW A2, 0xA8 (S0)	// Pos Z
JAL 0x802A37AC		// Default Particle function
NOP
LUI A0, 0x5061
JAL 0x802CA1E0		// Sound Magic
ORI A0, A0, 0xFF81
BEQZ R0, cavin_goomboss_end
SW R0, 0x74 (S0)	// Despawn boss


cavin_goomboss_walk:
LW T1, 0xC8 (S0)	// Load move angle
LW AT, 0x1A8 (S0)	// Load turn increment
ADDU T1, T1, AT
SW T1, 0xC8 (S0)	// Store move angle
JAL 0x802E5160		// Move using fvel and yaw
OR A0, R0, S0
BEQZ R0, cavin_goomboss_summon
NOP


cavin_goomboss_damaged:
ORI A0, R0, 0x10
JAL 0x802A3D40		// Shake Y
ORI A1, R0, 0x10
LWC1 F0, 0x30 (S0)	// Load scale Y
LUI AT, 0x3F81
ORI AT, AT, 0x0625	// 1.008f
MTC1 AT, F1
MUL.S F2, F1, F0
SWC1 F2, 0x2C (S0)
SWC1 F2, 0x30 (S0)
BEQZ V0, cavin_goomboss_common
SWC1 F2, 0x34 (S0)

LW T1, 0x1A8 (S0)	// Speedup on damage
SRL AT, T1, 1
ADDU T1, T1, AT
SW T1, 0x1A8 (S0)
LUI AT, 0x3FC0		// 1.5f
MTC1 AT, F1
LWC1 F0, 0xB8 (S0)
MUL.S F2, F1, F0
SWC1 F2, 0xB8 (S0)

LUI AT, 0x3FA0		// 1.25f
MTC1 AT, F0
LWC1 F1, 0x1F8 (S0)
LWC1 F2, 0x1FC (S0)
LWC1 F3, 0x200 (S0)
LWC1 F4, 0x204 (S0)
MUL.S F1, F1, F0
MUL.S F2, F2, F0
MUL.S F3, F3, F0
MUL.S F4, F4, F0
SWC1 F1, 0x1F8 (S0)
SWC1 F2, 0x1FC (S0)
SWC1 F3, 0x200 (S0)
SWC1 F4, 0x204 (S0)

ORI AT, R0, 0x3
BNE AT, S1, @@logic_branch_1
ORI AT, R0, 0x2		// Set action to 2
ORI AT, R0, 0x1		// Set action to 1
@@logic_branch_1:
BEQZ R0, cavin_goomboss_common
SW AT, 0x14C (S0)	// Store action


cavin_goomboss_common:
JAL 0x80405218		// cavin_handle_boss_i_frames
NOP
JAL 0x80405210		// cavin_check_boss_collision
NOP
BEQZ V1, cavin_goomboss_end

cavin_goomboss_collision_damage:
OR A1, R0, S0
LUI A0, 0x8034
JAL 0x8024D998		// Take damage and knockback
ADDIU A0, A0, 0xB170
BEQZ R0, cavin_goomboss_end
NOP

cavin_goomboss_end:
LW S2, 0x18 (SP)
LW S1, 0x14 (SP)
LW S0, 0x10 (SP)
LW RA, 0x1C (SP)
JR RA
ADDIU SP, SP, 0x20



// 0xFC: Action
cavin_goomboss_minion_start:
ADDIU SP, SP, 0xFFE0
SW RA, 0x1C (SP)
SW RA, 0x1C (SP)
SW S0, 0x10 (SP)
SW S1, 0x14 (SP)
SW S2, 0x18 (SP)
LUI S0, 0x8036
JAL 0x802A064C		// current object get floor height and data
LW S0, 0x1160 (S0)
SW V0, 0x1C0 (S0)	// Store floor struct
LW S1, 0x184 (S0)	// Load damage counter
LW S2, 0x150 (S0)	// Load subaction

LB T0, 0xFC (S0)	// Load action
ADDIU AT, R0, 0x1
BEQ T0, AT, cavin_goomboss_minion_squish
ADDIU AT, AT, 0x1
BEQ T0, AT, cavin_goomboss_minion_fling
ADDIU AT, AT, 0x1
BEQ T0, AT, cavin_goomboss_minion_wander

LW AT, 0x154 (S0)	// Load obj timer
ADDIU AT, AT, 0xFFE0	// 32 frames
BGTZ AT, cavin_goomboss_minion_init
NOP
JAL 0x802A25B4		// cur obj move using vel and gravity
NOP
BEQZ R0, cavin_goomba_minion_common
NOP

cavin_goomboss_minion_init:
LW AT, 0xA0 (S0)
SW AT, 0x164 (S0)	// Home X
LW AT, 0xA0 (S0)
SW AT, 0x168 (S0)	// Home Y
LW AT, 0xA8 (S0)
SW AT, 0x16C (S0)	// Home Z
LUI AT, 0x40E0		// 7.0f
SW AT, 0xB8 (S0)	// Store fvel
ORI AT, R0, 0x3
SB AT, 0xFC (S0)	// Store action
LUI AT, 0x402E		// Interaction flags
ORI AT, AT, 0x0100
BEQZ R0, cavin_goomba_minion_common
SW AT, 0xF8 (S0)	// Store interaction flags

cavin_goomboss_minion_squish:
LWC1 F0, 0x30 (S0)	// Load Y scale
LUI AT, 0x3E50		// 0.203125f
MTC1 AT, F1
SUB.S F0, F1
C.LT.S F0, F1
BC1F cavin_goomba_minion_end
SWC1 F0, 0x30 (S0)	// Store Y scale
BEQZ R0, cavin_goomba_minion_die
NOP

cavin_goomboss_minion_fling:
JAL 0x802E5160		// Move using fvel and yaw
OR A0, R0, S0
ORI AT, R0, 0x0001	// animate at basically zero speed
JAL 0x802A25B4		// cur obj move using vel and gravity
SW AT, 0x48 (S0)	// Store anim accel
LW A1, 0x68 (S0)
OR A0, R0, S0
JAL 0x802C8FE4		// Check hitbox overlap
OR V0, R0, R0		// Reset V0 because the function doesn't
BEQZ V0, @@logic_branch_1
LW T0, 0x68 (S0)	// Load parent struct
ORI AT, R0, 0x3
SW AT, 0x14C (T0)	// Store parent action
SW R0, 0x154 (T0)	// Reset parent obj timer
LW AT, 0x184 (T0)	// Load parent's damage
ADDIU AT, AT, 0x1
BEQZ R0, cavin_goomba_minion_die
SW AT, 0x184 (T0)	// Store parent's damage
@@logic_branch_1:
LWC1 F0, 0xA4 (S0)	// Load posY
LWC1 F1, 0xE8 (S0)	// Load floor height
C.LE.S F0, F1
BC1F cavin_goomba_minion_end
NOP
BEQZ R0, cavin_goomba_minion_die
NOP

cavin_goomboss_minion_wander:
JAL 0x802A2644		// cur obj move using fvel and gravity
NOP
LWC1 F0, 0xA4 (S0)	// Load posY
LWC1 F1, 0xE8 (S0)	// Load floor height
C.LE.S F0, F1
BC1F @@logic_branch_1
OR A0, R0, S0
SWC1 F1, 0xA4 (S0)	// Set posY to floor height
SW R0, 0xB0 (S0)	// Reset velY
@@logic_branch_1:
LW A1, 0x68 (S0)
JAL 0x802C8FE4		// Check hitbox overlap
OR V0, R0, R0		// Reset V0 because the function doesn't
BEQZ V0, @@logic_branch_2
ORI AT, R0, 0x1
SB AT, 0xFC (S0)	// Store action
@@logic_branch_2:
BEQZ R0, cavin_goomba_minion_common
NOP

cavin_goomba_minion_die:
LW T0, 0x68 (S0)	// Load parent struct
LBU AT, 0xFC (T0)	// Load minion count
ADDIU AT, AT, 0xFFFF
JAL 0x802A37AC		// Particle function
SB AT, 0xFC (T0)	// Store minion count
LUI A0, 0x5461
JAL 0x802CA1E0		// Sound Magic
ADDIU A0, A0, 0xFF81
BEQZ R0, cavin_goomba_minion_end_2
SW R0, 0x74 (S0)	// Despawn

cavin_goomba_minion_common:
JAL 0x80405218		// cavin_handle_boss_i_frames
NOP
JAL 0x80405210		// cavin_check_boss_collision
NOP
ORI AT, R0, 0x1
BEQ V1, AT, cavin_goomba_minion_collision_1
ORI AT, R0, 0x2
BEQ V1, AT, cavin_goomba_minion_collision_2
ORI AT, R0, 0x3
BEQ V1, AT, cavin_goomba_minion_collision_3
NOP
BEQZ V1, cavin_goomba_minion_end

cavin_goomba_minion_collision_damage:
OR A1, R0, S0
LUI A0, 0x8034
JAL 0x8024D998		// Take damage and knockback
ADDIU A0, A0, 0xB170
BEQZ R0, cavin_goomba_minion_end
NOP

cavin_goomba_minion_collision_1:
ORI AT, R0, 0x1		// Set action to 1
SB AT, 0xFC (S0)	// Store action
LUI T9, 0x8034
LWC1 F0, 0xB1BC (T9)	// Load Mario's Y speed
NEG.S F0, F0
BEQZ R0, cavin_goomba_minion_end
SWC1 F0, 0xB1BC (T9)	// Store Mario's Y speed

cavin_goomba_minion_collision_2:
LHU T1, 0x162 (S0)	// Get angle to Mario
XORI T1, T1, 0x8000
SW T1, 0xC8 (S0)	// Store move yaw
LUI T9, 0x8034
LHU T2, 0xB19E (T9)	// Load Mario's facing yaw
SUBU T3, T1, T2		// Get angle difference
ANDI AT, T3, 0x7FFF	// Remove sign bit
SRL AT, AT, 4
SLL AT, AT, 2
LUI T5, 0x8038		// Trig table
ADDU T6, T5, AT
LWC1 F11, 0x6000 (T6)	// Get Sine Ratio
LWC1 F12, 0x7000 (T6)	// Get Cosine Ratio
ABS.S F11, F11
ABS.S F12, F12
LWC1 F0, 0xB1C4 (T9)	// Load Mario's H speed
LUI AT, 0x4120		// 10.0f	// ADJUST THIS???
MTC1 AT, F1
ADD.S F0, F0, F1
LWC1 F5, 0xB1C8 (T9)	// Load Mario's X sliding speed
LWC1 F6, 0xB1CC (T9)	// Load Mario's Z sliding speed
MUL.S F4, F0, F11
MUL.S F5, F5, F11
MUL.S F6, F6, F11
SUB.S F2, F0, F4
SWC1 F4, 0xB1C4 (T9)	// Store Mario's H speed
SWC1 F5, 0xB1C8 (T9)	// Store Mario's X sliding speed
SWC1 F6, 0xB1CC (T9)	// Store Mario's Z sliding speed
SWC1 F2, 0xB8 (S0)	// Store velH
LUI AT, 0x4000		// 2.0f
MTC1 AT, F1
DIV.S F1, F2, F1
SWC1 F1, 0xB0 (S0)	// Store velY
LUI AT, 0xBF40		// 0.75f
//SW AT, 0xE4 (S0)	// Store gravity
ORI AT, R0, 0x2		// Set action to 2
BEQZ R0, cavin_goomba_minion_end
SB AT, 0xFC (S0)	// Store action

cavin_goomba_minion_collision_3:
ORI AT, R0, 0x1		// Set action to 1
BEQZ R0, cavin_goomba_minion_end
SB AT, 0xFC (S0)	// Store action

cavin_goomba_minion_end:
LUI AT, 0xC4FA		// -2000.0f
MTC1 AT, F0
LWC1 F1, 0xA4 (S0)	// Load posY
C.LT.S F1, F0
BC1F cavin_goomba_minion_end_2
NOP
BEQZ R0, cavin_goomba_minion_die
NOP
cavin_goomba_minion_end_2:
LW S2, 0x18 (SP)
LW S1, 0x14 (SP)
LW S0, 0x10 (SP)
LW RA, 0x1C (SP)
JR RA
ADDIU SP, SP, 0x20



cavin_toxic_floor_start:
LUI T0, 0x8036
LW T0, 0x1160 (T0)
LUI T9, 0x8034
LW T8, 0xB174 (T9)	// Load flags
ANDI AT, T8, 0x4
BNEZ AT, cavin_toxic_floor_end
LW T8, 0xB1D8 (T9)	// Load floor triangle below Mario
BEQZ T8, cavin_toxic_floor_end
LHU T3, 0xB21E (T9)	// Load Mario's HP
BLEZ T3, cavin_toxic_floor_end
LH T2, 0x0 (T8)		// Load floor type below Mario
LBU T5, 0x188 (T0)	// Load target type
BNE T2, T5, cavin_toxic_floor_end
LW T6, 0xB17C (T9)	// Load Mario's action
ANDI T7, T6, 0x1C0	// Get action group
ADDIU AT, R0, 0x040
BNE AT, T7, cavin_toxic_floor_end
LHU AT, 0xB196 (T9)	// Load hitstun
BNEZ AT, cavin_toxic_floor_end
ADDIU AT, R0, 0x3C	// 60 frames
SH AT, 0xB196 (T9)	// Store hitstun
LUI AT, 0x0002
ORI AT, AT, 0x0464	// Stumbling from hit
SW AT, 0xB17C (T9)	// Store action state
ADDIU T3, T3, 0xFF00
cavin_toxic_floor_end:
JR RA
SH T3, 0xB21E (T9)



cavin_water_rescue_start:
ADDIU SP, SP, 0xFFE8
SW RA, 0x14 (SP)
LUI T0, 0x8036
LW T0, 0x1160 (T0)
ORI AT, R0, 0x2
SW AT, 0x130 (T0)	// Set interaction type grabbable
ORI AT, R0, 0x0409
SW AT, 0x8C (T0)	// Set obejct flags
JAL 0x8029F600		// obj_enable_rendering_and_become_tangible
OR A0, R0, T0
LW T5, 0x124 (T0)	// Load held state
LUI T4, 0xC1A0		// 20.0f vel Y
ORI AT, R0, 0x1
BNE AT, T5, cavin_water_rescue_hold
SW T4, 0xB0 (T0)	// Store vel Y
JAL 0x8029F644		// obj_disable_rendering_and_become_intangible
OR A0, R0, T0
cavin_water_rescue_hold:
LUI T0, 0x8036
LW T0, 0x1160 (T0)
LBU AT, 0xFC (T0)
BNEZ AT, cavin_water_rescue_end
LWC1 F12, 0xA0 (T0)	// pos X
JAL 0x80381BA0		// Find water level below point?
LWC1 F14, 0xA8 (T0)	// pos Z
LUI T0, 0x8036
LW T0, 0x1160 (T0)
LWC1 F2, 0xA4 (T0)	// pos Y
C.LE.S F2, F0
BC1T cavin_water_rescue_end
ORI AT, R0, 0x1
SB AT, 0xFC (T0)	// Store flag
LWC1 F12, 0xA0 (T0)	// Pos X
LUI AT, 0x4396		// 300.0f
MTC1 AT, F16
LWC1 F14, 0xA4 (T0)
ADD.S F14, F14, F16	// 500.0f units above pos Y
JAL 0x802F2B88		// Spawn star
LW A2, 0xA8 (T0)	// Pos Z
cavin_water_rescue_end:
JAL 0x802E4E90		// cur object step
NOP
LW RA, 0x14 (SP)
JR RA
ADDIU SP, SP, 0x18



cavin_stop_music_start:
LUI T0, 0x8036
LW T0, 0x1160 (T0)
LHU T2, 0x188 (T0)	// Load B Param 1&2
LWC1 F0, 0x15C (T0)	// Load distance to Mario
MTC1 T2, F1
CVT.S.W F1, F1
c.LE.S F0, F1
BC1F cavin_stop_music_end
NOP
J 0x8024927C		// Fadeout level music
LBU A0, 0x18A (T0)	// Load B Param 3
cavin_stop_music_end:
JR RA
NOP



cavin_break_box_star_start:
LUI T0, 0x8036
LW T0, 0x1160 (T0)
LW AT, 0x74 (T0)
BNEZ AT, cavin_break_box_star_end
LWC1 F12, 0x164 (T0)	// Home Z
LWC1 F14, 0x168 (T0)	// Home Y
LUI AT, 0x4396		// 300.0f
MTC1 AT, F16
ADD.S F14, F14, F16	// 500.0f units above Home Y
J 0x802F2B88		// Spawn star
LW A2, 0x16C (T0)	// PHome Z
cavin_break_box_star_end:
JR RA
NOP



.endarea

// Functions
.orga 0x01205100
.area 0xE00

BEQZ R0, cavin_goomboss_start
NOP
BEQZ R0, cavin_goomboss_minion_start
NOP

BEQZ R0, cavin_toxic_floor_start
NOP
BEQZ R0, cavin_water_rescue_start
NOP

BEQZ R0, cavin_stop_music_start
NOP
BEQZ R0, cavin_break_box_star_start
NOP

NOP
NOP
NOP
NOP

NOP
NOP
NOP
NOP

NOP
NOP
NOP
NOP

NOP
NOP
NOP
NOP

NOP
NOP
NOP
NOP

NOP
NOP
NOP
NOP

NOP
NOP
NOP
NOP

NOP
NOP
NOP
NOP

NOP
NOP
NOP
NOP

NOP
NOP
NOP
NOP

NOP
NOP
NOP
NOP

NOP
NOP
NOP
NOP

NOP
NOP
NOP
NOP

BEQZ R0, cavin_get_boss_vel_start
NOP
BEQZ R0, cavin_boss_reach_target_start
NOP

BEQZ R0, cavin_check_boss_collision_start
NOP
BEQZ R0, cavin_handle_boss_i_frames_start
NOP

BEQZ R0, cavin_boss_get_yaw_to_target_start
NOP
BEQZ R0, cavin_random_chance_start
NOP

BEQZ R0, cavin_mega_flyguy_start
NOP
BEQZ R0, cavin_mega_flyguy_fire_start
NOP

BEQZ R0, cavin_faulty_platform_start
NOP
BEQZ R0, cavin_SORC_init_start // Spawn Object Radius Controller
NOP

BEQZ R0, cavin_SORC_start // Spawn Object Radius Controller
NOP
NOP
NOP



.dw 0x00040000		// 0x405260
.dw 0x11012009		// Set obj flags
.dd 0x2726000008011A64	// Set animation model
.dw 0x28000000		// Animate
.dd 0x23000000007000E0	// Collision cylinder
.dw 0x103E0001		// Damage to Mario
.dw 0x3200012C		// Scale object
.dw 0x10050000		// Set collision timer
.dw 0x08000000
.dd 0x0C00000080405230	// cavin_mega_flyguy
.dw 0x09000000

.dw 0x00060000		// 0x405298
.dw 0x11012009		// Set obj flags
.dw 0x21000000		// Set billboarding
.dd 0x2300000000200020	// Collision cylinder
.dd 0x2F00000000040000	// Interaction type
.dw 0x3200012C		// Scale object
.dw 0x08000000
.dw 0x10050000		// Set collision timer
.dd 0x0C00000080405238	// cavin_mega_flyguy_fire
.dw 0x341A0002		// Fire flicker
.dw 0x09000000

.dw 0x00090000		// 0x4052D0
.dd 0x2A0000000800D710	// Collision pointer
.dw 0x08000000
.dd 0x0C000000803839CC	// Collision function
.dd 0x0C00000080405240	// cavin_faulty_platform_start
.dw 0x09000000

.dw 0x00040000		// 0x4052F4
.dd 0x0C00000080405248	// cavin_SORC_init_start
.dw 0x08000000
.dd 0x0C00000080405250	// cavin_SORC_start
.dw 0x09000000

.dw 0x00040000		// 0x405310
.dw 0x0A000000



/*
Gets componant velocities from target pos and vector vel
Assumes S0 is cur obj
Input:
A0: Velocity as float
vec3f_to at 0x100, 0x104, 0x108 from S0
Output:
Stores velX, velY, velZ, and velH to 0xAC, 0xB0, 0xB4, and 0xB8
Also stores them as F1, F2, F3 and F0.
*/
cavin_get_boss_vel_start: // FIX
ADDIU SP, SP, 0xFFE0
SW RA, 0x1C (SP)
SW A0, 0x18 (SP)
ADDIU A0, S0, 0xA0
ADDIU A1, S0, 0x100
ADDIU A2, SP, 0x10
JAL 0x8028AB60		// Get pitch and yaw
ADDIU A3, SP, 0x14
LUI T5, 0x8038
LHU AT, 0x10 (SP)	// Pitch
SRL AT, AT, 4
SLL AT, AT, 2
ADDU AT, T5, AT
LWC1 F11, 0x6000 (AT)	// Get Sine Ratio (Y)
LWC1 F12, 0x7000 (AT)	// Get Cosine Ratio (XZ)
LHU AT, 0x14 (SP)	// Yaw
SRL AT, AT, 4
SLL AT, AT, 2
ADDU AT, T5, AT
LWC1 F13, 0x6000 (AT)	// Get Sine Ratio (X)
LWC1 F14, 0x7000 (AT)	// Get Cosine Ratio (Z)
LWC1 F0, 0x18 (SP)	// Load velH
MUL.S F2, F0, F11
MUL.S F5, F0, F12
MUL.S F3, F5, F14
MUL.S F1, F5, F13
SWC1 F1, 0xAC (S0)	// Store velX
SWC1 F2, 0xB0 (S0)	// Store velY
SWC1 F3, 0xB4 (S0)	// Store velZ
SWC1 F0, 0xB8 (S0)	// Store velH
LW RA, 0x1C (SP)
JR RA
ADDIU SP, SP, 0x20



// Handles the end of a movement segment. Returns V0 = 0x1 if reached target.
// Assumes S0 is cur obj
cavin_boss_reach_target_start: // FIX
ADDIU SP, SP, 0xFFE8
SW RA, 0x14 (SP)

ADDIU A0, S0, 0xA0
JAL 0x8028AC28		// Calc abs dist
ADDIU A1, S0, 0x100
LWC1 F1, 0xB8 (S0)
C.LE.S F0, F1		// Compare if dist is less than velH
BC1F cavin_boss_reach_target_end
OR V0, R0, R0
SW R0, 0xAC (S0)
SW R0, 0xB0 (S0)
SW R0, 0xB4 (S0)
SW R0, 0xB8 (S0)
LW AT, 0x100 (S0)
SW AT, 0xA0 (S0)
LW AT, 0x104 (S0)
SW AT, 0xA4 (S0)
LW AT, 0x108 (S0)
SW AT, 0xA8 (S0)
ORI V0, R0, 0x1
cavin_boss_reach_target_end:
LW RA, 0x14 (SP)
JR RA
ADDIU SP, SP, 0x18



// Checks if the collision method with the interaction flags, else deal damage to Mario
// Returns V1: 1 to 4 for which set of interaction flags, 0 if none, -1 if hurtbox collision
// Assumes S0 is cur obj
cavin_check_boss_collision_start: // FIX
ADDIU SP, SP, 0xFFE8
SW RA, 0x14 (SP)

LUI A1, 0x8036
LW A0, 0x1160 (A1)
OR S0, R0, A0
JAL 0x802A1424		// Check collision
LW A1, 0x1158 (A1)
BEQZ V0, cavin_check_boss_collision_end
OR V1, R0, R0		// Set V1 to 0
LUI A1, 0x8036
LW A1, 0x1160 (A1)
LUI A0, 0x8034
JAL 0x8024C1D8		// Determine interaction
ADDIU A0, A0, 0xB170
OR V1, R0, R0		// Set V1 to 0
cavin_check_boss_collision_loop:
ADDU T0, S0, V1
LBU T1, 0xF8 (T0)	// Load interaction flags
AND AT, V0, T1
BNEZ AT, cavin_check_boss_collision_end
ADDIU V1, V1, 0x1
ORI AT, R0, 0x4
BNE AT, V1, cavin_check_boss_collision_loop
LUI A0, 0x8036
LW A0, 0x1158 (A0)
JAL 0x802C91EC		// detect_object_hurtbox_overlap
OR A1, R0, S0
BEQZ V0, cavin_check_boss_collision_end
NOP
ADDIU V1, R0, 0xFFFF	// Set v1 to -1
cavin_check_boss_collision_end:
LW RA, 0x14 (SP)
JR RA
ADDIU SP, SP, 0x18



// Handle I frames based on collision timer
// Assumes S0 is cur obj
cavin_handle_boss_i_frames_start: // FIX
LW AT, 0x9C (S0)	// Get collision timer
BGTZ AT, cavin_handle_boss_i_frames_end
LH AT, 0x2 (S0)		// Load graph flags
ORI AT, AT, 0x0010
cavin_handle_boss_i_frames_end:
XORI AT, AT, 0x0010
JR RA
SH AT, 0x2 (S0)		// Store graph flags



// Gets yaw to face target position
// Assumes S0 is cur obj
cavin_boss_get_yaw_to_target_start: // FIX
ADDIU SP, SP, 0xFFE8
SW RA, 0x14 (SP)
ADDIU A0, S0, 0xA0
JAL 0x8028AAD8		// Calc yaw
ADDIU A1, S0, 0x100
SW V0, 0x10C (S0)	// Store target yaw
LW RA, 0x14 (SP)
JR RA
ADDIU SP, SP, 0x18



// Takes A0 and treats it as decimal number from 0 to 1, and returns true that % of the time
cavin_random_chance_start: // FIX
ADDIU SP, SP, 0xFFE8
SW RA, 0x14 (SP)
JAL 0x80383CB4		// Random float
SW A0, 0x10 (SP)
LWC1 F2, 0x10 (SP)	// Load %
CVT.S.W F2, F2
LUI AT, 0x42C8		// 100.0f
MTC1 AT, F1
DIV.S F2, F2, F1
C.LT.S F0, F2
BC1F cavin_random_chance_end
OR V0, R0, R0
ORI V0, R0, 0x1
cavin_random_chance_end:
LW RA, 0x14 (SP)
JR RA
ADDIU SP, SP, 0x18



// Big Flyguy Boss
/* Action variables:
FC: Fire Count

FE: Lunge / Dive Count
FF: Bob Timer

0x1A4: Bob Offset


mega_flyguy_threshhold
Dive x2: 5
Dive x3: 12
Dive Fire: 8
Lunge: 10
Death: 17

Additional max fire every 2 damage
Gradual increase to speed, rotation speed, and attack chance at higher damage

*/
cavin_mega_flyguy_start:
ADDIU SP, SP, 0xFFE0
SW RA, 0x1C (SP)
SW S0, 0x10 (SP)
SW S1, 0x14 (SP)
SW S2, 0x18 (SP)

LUI S0, 0x8036
JAL 0x802A064C		// current object get floor height and data
LW S0, 0x1160 (S0)
SW V0, 0x1C0 (S0)	// Store floor struct
LW S1, 0x184 (S0)	// Load damage counter
LW S2, 0x150 (S0)	// Load subaction

cavin_mega_flyguy_action:
LW T0, 0x14C (S0)	// Load action
ADDIU AT, R0, 0x1
BEQ T0, AT, cavin_mega_flyguy_track_start
ADDIU AT, AT, 0x1
BEQ T0, AT, cavin_mega_flyguy_dive_start
ADDIU AT, AT, 0x1
BEQ T0, AT, cavin_mega_flyguy_lunge_start
ADDIU AT, AT, 0x1
BEQ T0, AT, cavin_mega_flyguy_die_start
ADDIU AT, AT, 0x1
BEQ T0, AT, cavin_mega_flyguy_wander_start
ADDIU AT, AT, 0x1
BEQ T0, AT, cavin_mega_flyguy_damaged_start

LW AT, 0x1F8 (S0)
SW AT, 0x200 (S0)	// Fix hurtbox
LW AT, 0x1FC (S0)
SW AT, 0x204 (S0)	// Fix hurtbox
LWC1 F1, 0xE8 (S0)	// Load floor height
LUI T1, 0x43FA		// 500.0f units
MTC1 T1, F0
ADD.S F0, F0, F1
SWC1 F0, 0xA4 (S0)	// Store posY
SWC1 F0, 0x168 (S0)	// Store homeY
LWC1 F1, 0xA0 (S0)	// Load posX
LWC1 F3, 0xA8 (S0)	// Load posZ
SWC1 F1, 0x164 (S0)	// Store homeX
SWC1 F3, 0x16C (S0)	// Store homeZ
ORI AT, R0, 0x5		// Set action to 5
SW AT, 0x14C (S0)	// Store action
BEQZ R0, cavin_mega_flyguy_end_3
SW R0, 0x184 (S0)	// Reset damage counter


cavin_mega_flyguy_track_start:
LUI AT, 0x0420
SW AT, 0xF8 (S0)	// Store interaction flags
LUI T0, 0x8034
LWC1 F1, 0xB1AC (T0)	// Load Mario's posX
LWC1 F2, 0xA4 (S0)	// Load obj posY
LWC1 F3, 0xB1B4 (T0)	// Load Mario's posZ
SWC1 F1, 0x100 (S0)	// Store Target X
SWC1 F2, 0x104 (S0)	// Store Target Y
SWC1 F3, 0x108 (S0)	// Store Target Z
ADDIU AT, S1, 0xC	// 12 speed + damage
MTC1 AT, F0
CVT.S.W F0, F0
JAL 0x80405220		// cavin_boss_get_yaw_to_target // FIX
SWC1 F0, 0xB8 (S0)	// Store velH
LW A0, 0x10C (S0)	// Load target yaw
SLL AT, S1, 6
JAL 0x8029E5EC		// Rotate to face point
ADDIU A1, AT, 0x200	// Rotation increment + damage * 0x40
JAL 0x802E5160		// Move using fvel and yaw
OR A0, R0, S0
LW T4, 0x148 (S0)	// Load action timer
SLL AT, S1, 4
ADDIU AT, AT, 0x00F0	// 240 frames + damage * 16
BNE T4, AT, cavin_mega_flyguy_track_sub_0_B
ORI AT, R0, 0x2		// Set action to 2
SW AT, 0x14C (S0)	// Store action
cavin_mega_flyguy_track_sub_0_B:
ORI AT, R0, 0x30	// 48 frames
DIVU T4, AT
MFHI AT
BNEZ AT, cavin_mega_flyguy_track_sub_0_A
SLL AT, S1, 2
JAL 0x80405228		// cavin_random_chance // FIX
ADDIU A0, AT, 0x1E	// 30 + Damage * 4 %chance to spit fire
BNEZ V0, cavin_mega_flyguy_spit_fire_start
NOP
cavin_mega_flyguy_track_sub_0_A:
BEQZ R0, cavin_mega_flyguy_bob_start
NOP


cavin_mega_flyguy_dive_start:
LUI AT, 0x0620
SW AT, 0xF8 (S0)	// Store interaction flags
BGTZ S2, cavin_mega_flyguy_dive_sub_1
ORI AT, R0, 0x1		// Set subaction to 1
SW AT, 0x150 (S0)	// Store subaction
LUI T0, 0x8034
LWC1 F1, 0xB1AC (T0)	// Load Mario's posX
LWC1 F2, 0xB1B0 (T0)	// Load Mario's posY
LWC1 F3, 0xB1B4 (T0)	// Load Mario's posZ
SWC1 F1, 0x100 (S0)	// Store Target X
SWC1 F2, 0x104 (S0)	// Store Target Y
JAL 0x80405220		// cavin_boss_get_yaw_to_target // FIX
SWC1 F3, 0x108 (S0)	// Store Target Z

cavin_mega_flyguy_dive_sub_1:
SUBIU AT, S2, 0x1	// Subtract 1
BGTZ AT, cavin_mega_flyguy_dive_sub_2
LW A0, 0x10C (S0)	// Load target yaw
LBU AT, 0xFE (S0)	// Load dive / lunge count
ADDIU A1, R0, 0x400	// Rotation increment
JAL 0x8029E5EC		// Rotate to face point
SLLV A1, A1, AT
BEQZ V0, cavin_mega_flyguy_common
ORI AT, R0, 0x2		// Set subaction to 2
SW AT, 0x150 (S0)	// Store subaction
SW R0, 0x148 (S0)	// Reset action timer

cavin_mega_flyguy_dive_sub_2:
LUI A0, 0x1418
JAL 0x802CA1E0		// Sound Magic
ADDIU A0, A0, 0xFF01
LW T4, 0x148 (S0)	// Load action timer
SLL AT, T4, 7
LUI T5, 0x8038		// Trig table
ADDU T6, T5, AT
LWC1 F11, 0x6000 (T6)	// Get Sine Ratio (XZ)
LWC1 F12, 0x7000 (T6)	// Get Cosine Ratio (Y)
LW AT, 0x184 (S0)	// Load damage counter
SRL AT, AT, 1
ADDIU AT, AT, 0xF	// Add 15 speed
MTC1 AT, F1
CVT.S.W F3, F1
LUI AT, 0xC130		// -13.0f
MTC1 AT, F2
MUL.S F2, F2, F12
MUL.S F3, F3, F11
SWC1 F2, 0xB0 (S0)	// Store velY
JAL 0x8029F070		// cur obj move using vel
SWC1 F3, 0xB8 (S0)	// Store velH
JAL 0x802E5160		// Move using fvel and yaw
OR A0, R0, S0
LW T4, 0x148 (S0)	// Load action timer
SUBIU AT, S1, 0x8	// Testfor damage amount, dive fire // mega_flyguy_threshhold
BLTZ AT, @@cavin_mega_flyguy_damage_buff
ORI AT, R0, 0x22
BEQ AT, T4, cavin_mega_flyguy_spit_fire_start
@@cavin_mega_flyguy_damage_buff:
ORI AT, R0, 0x40
BNE AT, T4, cavin_mega_flyguy_common
SUBIU AT, S1, 0x5	// Testfor damage amount, dive x2 // mega_flyguy_threshhold
BLTZ AT, @@cavin_mega_flyguy_damage_buff2
ORI T1, R0, 0x0
SUBIU AT, S1, 0xC	// Testfor damage amount, dive x3 // mega_flyguy_threshhold
BLTZ AT, @@cavin_mega_flyguy_damage_buff2
ORI T1, R0, 0x1
ORI T1, R0, 0x2
@@cavin_mega_flyguy_damage_buff2:
LBU AT, 0xFE (S0)	// Load dive / lunge count
BEQ AT, T1, @@subaction_logic_branch_A
ADDIU AT, AT, 0x1	// Add 1
SB AT, 0xFE (S0)	// Store dive / lunge count
BEQZ R0, cavin_mega_flyguy_common
SW R0, 0x150 (S0)	// Reset subaction
@@subaction_logic_branch_A:
SB R0, 0xFE (S0)	// Reset dive / lunge count
ORI AT, R0, 0x5		// Set action to 5
BEQZ R0, cavin_mega_flyguy_common
SW AT, 0x14C (S0)	// Store action


cavin_mega_flyguy_lunge_start:
BGTZ S2, cavin_mega_flyguy_lunge_sub_1
ORI AT, R0, 0x1		// Set subaction to 1
SW AT, 0x150 (S0)
LW AT, 0x160 (S0)	// Load angle to Mario
XORI AT, AT, 0x8000
SW AT, 0x10C (S0)	// Store target yaw
LUI AT, 0x4248		// 50.0f velocity
SW AT, 0xB8 (S0)	// Store velHv

cavin_mega_flyguy_lunge_sub_1:
SUBIU AT, S2, 0x1
BGTZ AT, cavin_mega_flyguy_lunge_sub_2
LW A0, 0x10C (S0)	// Load target yaw
LW AT, 0x184 (S0)	// Load damage counter
SLL AT, AT, 6
JAL 0x8029E5EC		// Rotate to face point
ADDIU A1, AT, 0x600	// Rotation increment
JAL 0x802E5160		// Move using fvel and yaw
OR A0, R0, S0
LW T2, 0x1C0 (S0)	// Load floor
LUI AT, 0x0420
BEQZ T2, @@cavin_mega_flyguy_floor_detect_A
SW AT, 0xF8 (S0)	// Store interaction flags
LHU AT, 0x0 (T2)	// Load floor type
BEQZ AT, cavin_mega_flyguy_bob_start
@@cavin_mega_flyguy_floor_detect_A:
ORI AT, R0, 0x2		// Set subaction to 2
SW AT, 0x150 (S0)	// Store subaction

cavin_mega_flyguy_lunge_sub_2:
SUBIU AT, S2, 0x2
BGTZ AT, cavin_mega_flyguy_lunge_sub_3
LW AT, 0x160 (S0)	// Load angle to Mario
SW AT, 0x10C (S0)	// Store target yaw
LWC1 F1, 0xA0 (S0)	// Load posX
LWC1 F2, 0x168 (S0)	// Load homeY
LWC1 F3, 0xA8 (S0)	// Load posZ
LUI AT, 0x43C8		// 400.0f
MTC1 AT, F4
SUB.S F2, F2, F4
SWC1 F1, 0x100 (S0)	// Store Target X
SWC1 F2, 0x104 (S0)	// Store Target Y
SWC1 F3, 0x108 (S0)	// Store Target Z
LW A0, 0x10C (S0)	// Load target yaw
JAL 0x8029E5EC		// Rotate to face point
ADDIU A1, R0, 0x800	// Rotation increment
ADDIU A0, R0, 0x3800
JAL 0x802FA6D4		// Rotate face pitch
ADDIU A1, R0, 0x580	// Rotation increment
JAL 0x80405200		// cavin_get_boss_vel
LUI A0, 0x41A0		// 20.0f
JAL 0x8029F070		// cur obj move using vel
SW R0, 0xF8 (S0)	// Store interaction flags
LUI AT, 0x42E0		// 112.0f
SW AT, 0x1FC (S0)	// Store col_cyl_height
JAL 0x80405208		// cavin_boss_reach_target
SW AT, 0x204 (S0)
BEQZ V0, cavin_mega_flyguy_common
ORI AT, R0, 0x3		// Set subaction to 3
SW AT, 0x150 (S0)	// Store subaction
SW R0, 0x148 (S0)	// Reset action timer
LUI A0, 0x0557
JAL 0x802CA1E0		// Sound Magic
ADDIU A0, A0, 0xFF81

cavin_mega_flyguy_lunge_sub_3:
SUBIU AT, S2, 0x3
BGTZ AT, cavin_mega_flyguy_lunge_sub_4
ADDIU AT, R0, 0x2	// Set damage to Mario to 2
SW AT, 0x180 (S0)	// Store damage to Mario
LUI AT, 0x42C8		// 100.0f
MTC1 AT, F0
LBU AT, 0xFE (S0)	// Load dive / lunge count
SLL AT, AT, 4
MTC1 AT, F1
CVT.S.W F1, F1
ADD.S F0, F0, F1
SWC1 F0, 0xB8 (S0)	// Store velHv
JAL 0x802E5160		// Move using fvel and yaw
OR A0, R0, S0
ADDIU AT, R0, 0x0100
SW AT, 0xF8 (S0)	// Store interaction flags
LW T7, 0x148 (S0)	// Load action timer
ADDIU AT, T7, 0xFFFA	// Subtract 6
BLEZ AT, cavin_mega_flyguy_common
ORI AT, R0, 0x8
BNE AT, T7, @@subaction_logic_branch_A
ADDIU T2, T2, 0x1
BEQZ R0, cavin_mega_flyguy_spit_fire_start
SB T2, 0xFF (S0)	// Store fire trail counter
@@subaction_logic_branch_A:
LW T2, 0x1C0 (S0)	// Load floor
BEQZ T2, @@cavin_mega_flyguy_floor_detect_A
NOP
LHU AT, 0x0 (T2)	// Load floor type
BEQZ AT, cavin_mega_flyguy_common
@@cavin_mega_flyguy_floor_detect_A:
LBU T1, 0xFE (S0)	// Load dive / lunge count
ADDIU T1, T1, 0x1
SB T1, 0xFE (S0)	// Store lunge count
LW AT, 0x160 (S0)	// Load angle to Mario
SW AT, 0x10C (S0)	// Store target yaw
ORI AT, R0, 0x4		// Set subaction to 4
BEQZ R0, cavin_mega_flyguy_common
SW AT, 0x150 (S0)	// Store subaction

cavin_mega_flyguy_lunge_sub_4:
SB R0, 0xFF (S0)	// Reset fire trail counter
LBU T1, 0xFE (S0)	// Load lunge count
ORI AT, R0, 0x8
BEQ AT, T1, cavin_mega_flyguy_lunge_sub_5
LW A0, 0x10C (S0)	// Load target yaw
SW R0, 0xF8 (S0)	// Store interaction flags
SLL AT, T1, 9
JAL 0x8029E5EC		// Rotate to face point
ADDIU A1, AT, 0x800	// Rotation increment
BEQZ V0, cavin_mega_flyguy_common
ORI AT, R0, 0x3		// Set subaction to 3
SW AT, 0x150 (S0)	// Store subaction
LUI A0, 0x0557
JAL 0x802CA1E0		// Sound Magic
ADDIU A0, A0, 0xFF81
BEQZ R0, cavin_mega_flyguy_common
SW R0, 0x148 (S0)	// Reset action timer

cavin_mega_flyguy_lunge_sub_5:
LUI AT, 0x0420
SW AT, 0xF8 (S0)	// Store interaction flags
LUI AT, 0x4360		// 224.0f
SW AT, 0x1FC (S0)	// Store col_cyl_height
SW AT, 0x204 (S0)
LWC1 F1, 0xA0 (S0)	// Load posX
LWC1 F2, 0x168 (S0)	// Load homeY
LWC1 F3, 0xA8 (S0)	// Load posZ
SWC1 F1, 0x100 (S0)	// Store Target X
SWC1 F2, 0x104 (S0)	// Store Target Y
SWC1 F3, 0x108 (S0)	// Store Target Z
ADDIU A0, R0, 0x0000
JAL 0x802FA6D4		// Rotate face pitch
ADDIU A1, R0, 0x580	// Rotation increment
JAL 0x80405200		// cavin_get_boss_vel
LUI A0, 0x41A0		// 20.0f
JAL 0x8029F070		// cur obj move using vel
NOP
ADDIU AT, R0, 0x1	// Set damage to Mario to 1
JAL 0x80405208		// cavin_boss_reach_target
SW AT, 0x180 (S0)	// Store damage to Mario
BEQZ V0, cavin_mega_flyguy_common
ORI AT, R0, 0x1		// Set action to 1
SB R0, 0xFE (S0)	// Reset dive / lunge count
BEQZ R0,  cavin_mega_flyguy_common
SW AT, 0x14C (S0)	// Store action


cavin_mega_flyguy_die_start:
ORI AT, R0, 0x100
SW AT, 0x9C (S0)	// Store collision timer
LUI AT, 0x3F87		// 1.05469f
MTC1 AT, F4
OR T1, R0, R0		// Reset T1
cavin_mega_flyguy_die_loop:
ADDU T0, S0, T1
LWC1 F0, 0xAC (T0)	// Load vel
DIV.S F0, F0, F4
SWC1 F0, 0xAC (T0)	// Store vel
ORI AT, R0, 0xC
BNE AT, T1, cavin_mega_flyguy_die_loop
ADDIU T1, T1, 0x4
LW T2, 0x148 (S0)	// Load action timer
ORI AT, R0, 0x5A	// 90 frames
SUBU AT, T2, AT
BLEZ AT, cavin_mega_flyguy_common
LUI T0, 0x8034
LWC1 F12, 0xB1AC (T0)	// Load Mario's posX
LWC1 F14, 0x168 (S0)	// Load homeY
JAL 0x802F2B88		// Spawn star
LW A2, 0xB1B4 (T0)	// Load Mario's posZ
JAL 0x802A37AC		// Default Particle function
NOP
LUI A0, 0x5061
JAL 0x802CA1E0		// Sound Magic
ADDIU A0, A0, 0xFF81
BEQZ R0, cavin_mega_flyguy_common
SW R0, 0x74 (S0)	// Despawn boss


cavin_mega_flyguy_wander_start:
LUI AT, 0x0420
SW AT, 0xF8 (S0)	// Store interaction flags
BGTZ S2, cavin_mega_flyguy_wander_sub_1
ORI AT, R0, 0x1		// Set subaction to 1
SW AT, 0x150 (S0)
SW R0, 0xAC (S0)
SW R0, 0xB0 (S0)
SW R0, 0xB4 (S0)
ADDIU AT, S1, 0x8	// 8 speed + damage
MTC1 AT, F0
CVT.S.W F0, F0
JAL 0x80383BB0		// Call RNG
SWC1 F0, 0xB8 (S0)	// Store velH
SW V0, 0x10C (S0)	// Store target yaw
SW R0, 0x148 (S0)	// Reset action timer

cavin_mega_flyguy_wander_sub_1:
LW A0, 0x10C (S0)	// Get target yaw
SLL AT, S1, 6
JAL 0x8029E5EC		// Rotate to face point
ADDIU A1, AT, 0x200	// Rotation increment
JAL 0x802E5160		// Move using fvel and yaw
OR A0, R0, S0
LW T2, 0x1C0 (S0)	// Load floor
BEQZ T2, @@cavin_mega_flyguy_floor_detect_A
NOP
LHU AT, 0x0 (T2)	// Load floor type
BNEZ AT, @@cavin_mega_flyguy_floor_detect_A
LW AT, 0x148 (S0)	// Load action timer
ORI T3, R0, 0x2D	// 45 frames
BNE AT, T3, cavin_mega_flyguy_bob_start
ORI AT, R0, 0x0		// Set subaction to 0
SW AT, 0x150 (S0)	// Store subaction
SLL AT, S1, 2
JAL 0x80405228		// cavin_random_chance // FIX
ADDIU A0, AT, 0x14	// 20 + Damage * 4 %chance to track or lunge
BEQZ V0, cavin_mega_flyguy_bob_start
@@cavin_mega_flyguy_floor_detect_A:

SUBIU AT, S1, 0x0A	// Testfor damage lunge // mega_flyguy_threshhold
BLTZ AT, @@cavin_mega_flyguy_damage_buff
NOP
JAL 0x80405228		// cavin_random_chance // FIX
ADDIU A0, R0, 0x32	// 50 %chance to lunge
BEQZ V0, @@cavin_mega_flyguy_damage_buff
ORI AT, R0, 0x3		// Set action to 3
BEQZ R0, cavin_mega_flyguy_bob_start
SW AT, 0x14C (S0)	// Store action
@@cavin_mega_flyguy_damage_buff:
ORI AT, R0, 0x1		// Set action to 1
BEQZ R0, cavin_mega_flyguy_bob_start
SW AT, 0x14C (S0)	// Store action


cavin_mega_flyguy_damaged_start:
LWC1 F1, 0xA0 (S0)	// Load posX
LWC1 F2, 0x168 (S0)	// Load homeY
LWC1 F3, 0xA8 (S0)	// Load posZ
LWC1 F4, 0x1A4 (S0)	// Load Bob Offset
ADD.S F2, F4, F2
SWC1 F1, 0x100 (S0)	// Store Target X
SWC1 F2, 0x104 (S0)	// Store Target Y
SWC1 F3, 0x108 (S0)	// Store Target Z
JAL 0x80405200		// cavin_get_boss_vel
LUI A0, 0x41A0		// 20.0f
JAL 0x8029F070		// cur obj move using vel
NOP
JAL 0x80405208		// cavin_boss_reach_target
NOP
BEQZ V0, cavin_mega_flyguy_common
ORI AT, R0, 0x5		// Set action to 5
BEQZ R0, cavin_mega_flyguy_common
SW AT, 0x14C (S0)	// Store action


cavin_mega_flyguy_spit_fire_start:
LBU T3, 0xFC (S0)	// Load action variable 1, fire count
SRL AT, S1, 1
SUBU AT, T3, AT		// Every 2 Damage taken increases max flames by 1
BGEZ AT, cavin_mega_flyguy_common
cavin_mega_flyguy_spit_fire_bypass:
LUI A0, 0x5056
JAL 0x802CA1E0		// Sound Magic
ADDIU A0, A0, 0xFF81
OR A0, R0, S0
ORI A1, R0, 0xCB	// model_red_flame_shadow
LUI A2, 0x8040
ADDIU A2, A2, 0x5298
ADDIU T3, T3, 0x1	// Add 1 to fire count
JAL 0x8029EDCC		// Spawn child
SB T3, 0xFC (S0)	// Store action variable 1, fire count
SRL AT, S1, 1
SW AT, 0xFC (V0)	// Store damage count / 2 to fire struct
LHU AT, 0x1C (S0)	// Get gfx yaw
SRL AT, AT, 4
SLL AT, AT, 2
LUI T5, 0x8038		// Trig table
ADDU T6, T5, AT
LWC1 F11, 0x6000 (T6)	// Get Sine Ratio (X)
LWC1 F12, 0x7000 (T6)	// Get Cosine Ratio (Z)
LUI AT, 0x4258		// 54.0f
MTC1 AT, F0
LWC1 F4, 0xB8 (S0)	// Load velH
ADD.S F0, F0, F4	// Add velH to offset
MUL.S F1, F0, F11
MUL.S F3, F0, F12
LWC1 F4, 0xA0 (V0)
LWC1 F5, 0xA4 (V0)
LWC1 F6, 0xA8 (V0)
LUI AT, 0x428C		// 70.0f
MTC1 AT, F2
ADD.S F4, F4, F1
ADD.S F5, F5, F2
ADD.S F6, F6, F3
SWC1 F4, 0xA0 (V0)
SWC1 F5, 0xA4 (V0)
BEQZ R0, cavin_mega_flyguy_common
SWC1 F6, 0xA8 (V0)


cavin_mega_flyguy_bob_start:
//LUI A0, 0x0439
//JAL 0x802CA1E0		// Sound Magic
//ADDIU A0, A0, 0xFF01
LBU T1, 0xFF (S0)	// Load Bob Timer
ANDI T2, T1, 0x7F	// 128 frames per bob cycle
ADDIU T1, T1, 0x1
SB T1, 0xFF (S0)	// Store Bob timer
SLL AT, T2, 7
LUI T5, 0x8038		// Trig table
ADDU T6, T5, AT
LWC1 F11, 0x7000 (T6)	// Get Cosine Ratio
LUI AT, 0xC040		// -3.0f
MTC1 AT, F2
MUL.S F2, F2, F11
LWC1 F4, 0x1A4 (S0)	// Load Bob Offset
ADD.S F4, F4, F2
SWC1 F4, 0x1A4 (S0)	// Store Bob Offset
LWC1 F1, 0xA4 (S0)	// Load posY
ADD.S F2, F1, F2
BEQZ R0, cavin_mega_flyguy_common
SWC1 F2, 0xA4 (S0)	// Store vposY


cavin_mega_flyguy_common:
JAL 0x80405218		// cavin_handle_boss_i_frames
NOP
JAL 0x80405210		// cavin_check_boss_collision // FIX
NOP

BLTZ V1, cavin_mega_flyguy_collision_damage
ORI AT, R0, 0x1
BEQ V1, AT, cavin_mega_flyguy_collision_1
ORI AT, R0, 0x2
BEQ V1, AT, cavin_mega_flyguy_collision_2
ORI AT, R0, 0x3
BEQ V1, AT, cavin_mega_flyguy_collision_3
NOP
BEQZ R0, cavin_mega_flyguy_end

cavin_mega_flyguy_collision_damage:
LUI A1, 0x8036
LW A1, 0x1160 (A1)
LUI A0, 0x8034
JAL 0x8024D998		// Take damage and knockback
ADDIU A0, A0, 0xB170
BEQZ R0, cavin_mega_flyguy_end
NOP

cavin_mega_flyguy_collision_1:
LUI A0, 0x5464
JAL 0x802CA1E0		// Sound Magic
ADDIU A0, A0, 0xFF81
LW AT, 0x184 (S0)	// Load damage counter
ADDIU AT, AT, 0x2	// 2 damage
SW AT, 0x184 (S0)	// Store damage counter
ADDIU AT, R0, 0x3C	// 60 i-frames
SW AT, 0x9C (S0)	// Store collision timer
ORI AT, R0, 0x6		// Set action to 6
SW AT, 0x14C (S0)	// Store action
LUI T9, 0x8034
LWC1 F0, 0xB1C4 (T9)	// Load Mario's speed
NEG.S F0, F0
BEQZ R0, cavin_mega_flyguy_end
SWC1 F0, 0xB1C4 (T9)	// Store Mario's speed

cavin_mega_flyguy_collision_2:
LUI A0, 0x5463
JAL 0x802CA1E0		// Sound Magic
ADDIU A0, A0, 0xFF81
LW AT, 0x184 (S0)	// Load damage counter
ADDIU AT, AT, 0x1	// 1 damage
SW AT, 0x184 (S0)	// Store damage counter
ADDIU AT, R0, 0x64	// 100 i-frames
BEQZ R0, cavin_mega_flyguy_end
SW AT, 0x9C (S0)	// Store collision timer

cavin_mega_flyguy_collision_3:
LUI A0, 0x5464
JAL 0x802CA1E0		// Sound Magic
ADDIU A0, A0, 0xFF81
LW AT, 0x184 (S0)	// Load damage counter
ADDIU AT, AT, 0x2	// 2 damage
SW AT, 0x184 (S0)	// Store damage counter
ADDIU AT, R0, 0x14	// 20 i-frames
BEQZ R0, cavin_mega_flyguy_end
SW AT, 0x9C (S0)	// Store collision timer

cavin_mega_flyguy_end:
LW AT, 0x148 (S0)	// Load action timer
ADDIU AT, AT, 0x1	// Add 1
LW T1, 0x14C (S0)	// Load action
LW T2, 0x18C (S0)	// Load previous action
BEQ T1, T2, cavin_mega_flyguy_end_2
SW AT, 0x148 (S0)	// Store action timer
SW R0, 0x148 (S0)	// Reset action timer
cavin_mega_flyguy_end_2:
SUBIU AT, S1, 0x11	// Max damage to die
BLTZ AT, cavin_mega_flyguy_end_3
ORI AT, R0, 0x4		// Set action to 4
SW AT, 0x14C (S0)	// Store action
//SW R0, 0x148 (S0)	// Reset action timer
JAL 0x8024927C		// Fadeout level music
ORI A0, R0, 0x5A
cavin_mega_flyguy_end_3:
LW S2, 0x18 (SP)
LW S1, 0x14 (SP)
LW S0, 0x10 (SP)
LW RA, 0x1C (SP)
JR RA
ADDIU SP, SP, 0x20



// Fire from mega flyguy
cavin_mega_flyguy_fire_start:
ADDIU SP, SP, 0xFFE0
SW RA, 0x1C (SP)
SW S0, 0x10 (SP)
LUI S0, 0x8036
JAL 0x802A064C		// current object get floor height and data
LW S0, 0x1160 (S0)
SW V0, 0x1C0 (S0)	// Store floor struct

LUI T0, 0x8034
LWC1 F1, 0xB1AC (T0)	// Load Mario's posX
LWC1 F2, 0xA4 (S0)	// Load obj posY
LWC1 F3, 0xB1B4 (T0)	// Load Mario's posZ
SWC1 F1, 0x100 (S0)	// Store Target X
SWC1 F2, 0x104 (S0)	// Store Target Y
JAL 0x80405220		// cavin_boss_get_yaw_to_target // FIX
SWC1 F3, 0x108 (S0)	// Store Target Z

LW AT, 0xFC (S0)	// Load parant damage count /2
ADDIU AT, AT, 0xA	// Add 10 speed
MTC1 AT, F5
CVT.S.W F5, F5
SWC1 F5, 0xB8 (S0)	// Store velH
LW A0, 0x10C (S0)	// Get target yaw
JAL 0x8029E5EC		// Rotate to face point
ADDIU A1, R0, 0x300	// Rotation increment
JAL 0x802E5160		// Move using fvel and yaw
OR A0, R0, S0
LUI AT, 0xBFF0		// -1.875f falling velocity
JAL 0x8029F070		// cur obj move using vel
SW AT, 0xB0 (S0)	// Store velY

LWC1 F2, 0xE8 (S0)	// Load floor height
LUI AT, 0x41C8		// 25.0f
MTC1 AT, F4
ADD.S F2, F2, F4	// 25.0f units above floor
LWC1 F0, 0xA4 (S0)	// Load posY
C.LT.S F0, F2
BC1F cavin_mega_flyguy_fire_end
LW T4, 0x68 (S0)	// Load parent obj struct
LBU T3, 0xFC (T4)	// Load action variable 1, fire count
ADDIU T3, T3, 0xFFFF	// Subtract 1 from fire count
SB T3, 0xFC (T4)	// Store action variable 1, fire count
OR A0, R0, S0
ORI A1, R0, 0xCB	// model_red_flame_shadow
LUI A2, 0x1300
JAL 0x8029EDCC		// Spawn child
ADDIU A2, A2, 0x1A0C
SW R0, 0x74 (S0)
LUI A0, 0x1411
JAL 0x802CA1E0		// Sound Magic
ADDIU A0, A0, 0xFF01

cavin_mega_flyguy_fire_end:
LW S0, 0x10 (SP)
LW RA, 0x1C (SP)
JR RA
ADDIU SP, SP, 0x20


cavin_faulty_platform_start:
LUI T9, 0x8036
LW T0, 0x1160 (T9)
LW T9, 0x1158 (T9)
LW T9, 0x214 (T9)	// Load platform Mario is on
BNE T0, T9, cavin_faulty_platform_skip_activate
LUI T2, 0xBF00		// -0.5f
SW T2, 0xE4 (T0)	// Set Gravity
cavin_faulty_platform_skip_activate:
LUI T3, 0xC448		// -800.0f
MTC1 T3, F0
LWC1 F2, 0xA4 (T0)
C.LT.S F0, F2
BC1T cavin_faulty_platform_skip_despawn
LUI T4, 0x8065
SW R0, 0x74 (T0)
cavin_faulty_platform_skip_despawn:
LBU T3, 0x189 (T0)	// Load B Param 2
BEQZ T3, cavin_faulty_platform_end
ADDIU T4, T4, 0xB5E4
SW T4, 0x218 (T0)
cavin_faulty_platform_end:
J 0x802A25B4
SWC1 F2, 0x24 (T0)	// Update visual Y position



cavin_SORC_init_start:
LUI T0, 0x8036
LW T2, 0x1160 (T0)
LW T0, 0x1160 (T0)
LBU T1, 0x18A (T0)	// Load B Param 3
LBU T3, 0x18B (T0)	// Load B Param 4
cavin_SORC_init_loop:
BEQZ T1, cavin_SORC_init_exit_loop
ADDIU T1, T1, 0xFFFF	// Subtract 1
BEQZ R0, cavin_SORC_init_loop
ADDIU T0, T0, 0x260	// Load next obj struct
cavin_SORC_init_exit_loop:
SW T0, 0x1A4 (T2)	// Store target obj ptr
BNEZ T3, cavin_SORC_init_end
LH T4, 0x2 (T0)		// Load graph flags
LUI AT, 0x8040
ORI AT, AT, 0x5310	// Static and non solid object
SW AT, 0x1CC (T0)	// Store entry behav ptr
ORI T4, T4, 0x0010	// Render invisible
cavin_SORC_init_end:
JR RA
SH T4, 0x2 (T0)		// Store graph flags



cavin_SORC_start:
ADDIU SP, SP, 0xFFE8
SW RA, 0x14 (SP)
LUI T0, 0x8036
LW A1, 0x1158 (T0)
LW T0, 0x1160 (T0)
JAL 0x8029E2F8		// Calculate distance
LW A0, 0x1A4 (T0)	// Load target obj ptr
LUI T0, 0x8036
LW T0, 0x1160 (T0)
LHU T2, 0x188 (T0)	// Load B Param 1&2
LBU T3, 0x18B (T0)	// Load B Param 4
MTC1 T2, F1
CVT.S.W F1, F1
c.LE.S F0, F1
BC1F cavin_SORC_end
LW T9, 0x1A4 (T0)	// Load target obj ptr
LW AT, 0x20C (T9)	// Load original obj behav
BEQZ T3, cavin_SORC_normal
LUI T6, 0x8040
ORI AT, T6, 0x5310	// Static and non solid object
cavin_SORC_normal:
SW AT, 0x1CC (T9)	// Store obj behav
LH AT, 0x2 (T9)		// Load graph flags
XORI AT, AT, 0x0010	// Toggle visible
SH AT, 0x2 (T9)		// Store graph flags
SW R0, 0x74 (T0)	// Despawn self
cavin_SORC_end:
LW RA, 0x14 (SP)
JR RA
ADDIU SP, SP, 0x18


.endarea

/*

SUBIU AT, S1, 0x0	// Testfor damage
BLTZ AT, @@cavin_mega_flyguy_damage_buff
NOP

@@cavin_mega_flyguy_damage_buff:

*/



















