.include "Objects/blaarg/defines.asm"

.headersize 0x8018F240
.orga 0x3D0DC0+0x2600


@_initial_yaw equ 0xF4

beh_blaarg_impl:
BHV_START OBJ_LIST_GENERIC
BHV_OR_FLAGS o_flags, OBJ_FLAG_UPDATE_GFX | OBJ_STORE_DISTANCE_TO_MARIO | OBJ_STORE_ANGLE_TO_MARIO
BHV_SET_FLOAT o_render_distance, 10000
BHV_SET_WORD o_animation_pointer, 0x0500616C
BHV_SET_ANIMATION 1
BHV_SET_INTERACTION INT_DAMAGE
BHV_SET_INT o_collision_damage, 1
BHV_SET_HITBOX 185, 200, 0
BHV_SET_HURTBOX 185, 200
BHV_SET_INT o_intangibility_timer, 0
BHV_EXEC @store_home_angle
BHV_SET_INT o_state, 5
BHV_LOOP_BEGIN
	BHV_EXEC @loop
	BHV_SET_INT o_interaction_status, 0
BHV_LOOP_END

.definelabel @beh_blaarg_fire, (org() - 0x80000000)
BHV_START OBJ_LIST_LEVEL
BHV_OR_FLAGS o_flags, OBJ_FLAG_UPDATE_GFX
BHV_SET_FLOAT o_render_distance, 15000
BHV_BILLBOARD
BHV_SCALE 700
BHV_SET_INTERACTION INT_FLAME
BHV_SET_HITBOX 25, 50, 25
BHV_SET_INT o_intangibility_timer, 0
BHV_SET_PHYSICS 25, -200, 0, 0, 0, 0
BHV_ADD_FLOAT o_y, 175
BHV_SET_RANDOM_SHORT 0xF4, 5, 0xFC00
BHV_SUM_INTS o_move_angle_yaw, o_face_angle_yaw, 0xF4
BHV_EXEC @flame_init
BHV_SET_INT o_move_flags, 0
BHV_REPEAT_BEGIN 100
	BHV_EXEC @move
	BHV_EXEC @handle_move_flags
	BHV_SET_INT o_interaction_status, 0
	BHV_SLEEP 1
	BHV_EXEC @move
	BHV_EXEC @handle_move_flags
	BHV_SET_INT o_interaction_status, 0
	BHV_ADD_INT o_animation_state, 1
BHV_REPEAT_END
BHV_REPEAT_BEGIN 10
	BHV_EXEC @shrink
	BHV_SET_INT o_interaction_status, 0
	BHV_SLEEP 1
	BHV_EXEC @shrink
	BHV_SET_INT o_interaction_status, 0
	BHV_ADD_INT o_animation_state, 1
BHV_REPEAT_END
BHV_DELETE
BHV_END

@AGGRO_RADIUS equ 1750
@ATTACK_RADIUS equ 1250




; from helper-functions.asm
/* colliding_with_type
Checks if the given object is currently colliding with an object that has the
given behaviour.
args:
	A0 - [pointer] object to check
	A1 - [segmented pointer] behaviour
return:
	V0 - [pointer] the first colliding object with the given behaviour, or NULL
*/
colliding_with_type:
ADDIU SP, SP, 0xFFE8
SW RA, 0x14 (SP)
SW A0, 0x10 (SP)

JAL segmented_to_virtual
SLL A0, A1, 0x0
SLL A1, V0, 0x0

LW A0, 0x10 (SP)
LHU T0, o_num_collided_objects (A0)
BEQ T0, R0, @@return_null
@@loop:
	LW V0, o_collided_objects (A0)
	LW AT, 0x20C (V0)
	BEQ AT, A1, @@return_ptr
	ADDIU T0, T0, 0xFFFF
	BNE T0, R0, @@loop
	ADDIU A0, A0, 0x4
@@return_null:
SLL V0, R0, 0x0
@@return_ptr:
LW RA, 0x14 (SP)
JR RA
ADDIU SP, SP, 0x18


/* angle_from_object_to_point
Returns the angle (as a short) from the given object's
position to the given (x,z) point
args:
	A0 - [pointer] pointer to the object
	F12 - [float] x co-ordinate of the point
	F13 - [float] z co-ordinate of the point
returns:
	V0 - [short] angle to the point
*/
angle_to_point:
L.S F4, o_x (A0)
L.S F5, o_z (A0)
SUB.S F14, F12, F4
J atan2s
SUB.S F12, F13, F5



@move:
	ADDIU SP, SP, 0xFFE8
	SW RA, 0x14 (SP)
	
	JAL obj_update_floor
	NOP
	
	JAL obj_move_standard
	MOVE A0, R0
	
	JAL obj_resolve_wall_collisions
	NOP
	
	LW RA, 0x14 (SP)
	JR RA
	ADDIU SP, SP, 0x18

@store_home_angle:
	LW T0, g_current_obj_ptr
	LW AT, o_face_angle_yaw (T0)
	SW AT, @_initial_yaw (T0)
	ORI AT, R0, 0x1D
	JR RA
	SH AT, o_animation_frame (T0)

@loop:
	ADDIU SP, SP, 0xFFE8
	SW RA, 0x14 (SP)
	SW S0, 0x10 (SP)
	
	LW S0, g_current_obj_ptr
	
	LW T0, o_state (S0)
	ORI AT, R0, 5
	BNE T0, AT, @@endif_spawn_animation
	NOP
		JAL check_done_animation
		NOP
		BEQ V0, R0, @@return
		MOVE A0, R0
		JAL set_animation
		SW R0, o_state (S0)
		B @@return
	@@endif_spawn_animation:
	
	LW T0, o_state (S0)
	ORI AT, R0, 4
	BEQ T0, AT, @@dying
	
	LW A0, g_current_obj_ptr
	LI.U A1, beh_explosion
	JAL colliding_with_type
	LI.L A1, beh_explosion
	
	BEQ V0, R0, @@endif_bombed
		ORI AT, R0, 4
		SW AT, o_state (S0)
		LUI A0, 0x9204
		JAL cur_obj_play_sound_2
		ORI A0, A0, 0xC081
		JAL set_animation
		ORI A0, R0, 1
		SH R0, o_animation_frame (S0)
		ADDIU AT, R0, -1
		B @@return
		SW AT, o_intangibility_timer (S0)
	@@endif_bombed:
	
	LW AT, o_state (S0)
	BNE AT, R0, @@endif_idle
		LI.S F5, @AGGRO_RADIUS
		L.S F4, o_distance_to_mario (S0)
		C.LE.S F4, F5
		NOP
		BC1F @@endif_in_range
			ORI AT, R0, 1
			B @@return
			SW AT, o_state (S0)
		@@endif_in_range:
		LW A0, o_face_angle_yaw (S0)
		LW A1, @_initial_yaw (S0)
		JAL turn_angle
		ORI A2, R0, 0x200
		B @@return
		SW V0, o_face_angle_yaw (S0)
	@@endif_idle:
	
	LBU AT, o_arg0 (S0)
	BEQ AT, R0, @@endif_aim_forwards
		LI T0, g_mario
		LI.S F4, 250
		L.S F12, m_x (T0)
		L.S F13, m_z (T0)
		ADD.S F13, F13, F4
		JAL angle_to_point
		MOVE A0, S0
		B @@turn
		MOVE A1, V0
	@@endif_aim_forwards:
		LW A1, o_angle_to_mario (S0)
		
	@@turn:
	LW A0, o_face_angle_yaw (S0)
	JAL turn_angle
	ORI A2, R0, 0x400
	SW V0, o_face_angle_yaw (S0)
	
	LW T0, o_state (S0)
	ORI AT, R0, 3
	BEQ T0, AT, @@endif_active
		LI.S F5, @AGGRO_RADIUS
		L.S F4, o_distance_to_mario (S0)
		C.LE.S F4, F5
		LI.S F5, @ATTACK_RADIUS
		BC1T @@endif_far_away
		C.LE.S F4, F5
			B @@return
			SW R0, o_state (S0)
		@@endif_far_away:
		LW T0, o_state (S0)
		ORI AT, R0, 1
		BC1F @@return
		SW AT, o_state (S0)
		ORI AT, R0, 2
		BNE T0, AT, @@return
		SW AT, o_state (S0)
		LW AT, o_timer (S0)
		SLTIU AT, AT, 45
		BNE AT, R0, @@return
		ORI AT, R0, 3
		SW AT, o_state (S0)
		JAL set_animation
		ORI A0, R0, 1
		B @@return
		NOP
	@@endif_active:
	
	; attacking
	JAL check_done_animation
	NOP
	
	BEQ V0, R0, @@endif_done_attack
		ORI AT, R0, 1
		SW AT, o_state (S0)
		JAL set_animation
		MOVE A0, R0
		B @@return
		NOP
	@@endif_done_attack:
	
	LH T0, o_animation_frame (S0)
	SLTIU AT, T0, 0xA
	BNE AT, R0, @@return
	SLTIU AT, T0, 0x13
	BEQ AT, R0, @@return
	
	LUI A0, 0x6000
	JAL cur_obj_play_sound_2
	ORI A0, A0, 0x0001
	
	LH AT, o_animation_frame (S0)
	ANDI AT, T0, 0x1
	BNE AT, R0, @@return
	
	MOVE A0, S0
	LI A2, @beh_blaarg_fire
	JAL spawn_object
	ORI A1, R0, 0x90
	
	LH AT, o_animation_frame (S0)
	ADDIU AT, AT, -10
	SRL AT, AT, 1
	B @@return
	SB AT, o_arg0 (V0)
	
	@@dying:
	LH AT, o_animation_frame (S0)
	SLTIU AT, AT, 0x1D
	BNE AT, R0, @@return
	NOP
	
	SH R0, o_active_flags (S0)
	
	@@return:
	LW S0, 0x10 (SP)
	LW RA, 0x14 (SP)
	JR RA
	ADDIU SP, SP, 0x18

@flame_init:
	LW T0, g_current_obj_ptr
	LBU AT, o_arg0 (T0)
	MTC1 AT, F4
	LI.S F5, 2.5
	LI.S F6, 20
	LI.S F7, 30
	CVT.S.W F4, F4
	MUL.S F4, F4, F5
	ADD.S F8, F4, F7
	ADD.S F9, F4, F6
	S.S F8, o_speed_y (T0)
	J decompose_speed
	S.S F9, o_speed_h (T0)
	
@handle_move_flags:
	LW T0, g_current_obj_ptr
	LW AT, o_move_flags (T0)
	ANDI AT, AT, (MF_JUST_LANDED | MF_GROUNDED)
	BEQ AT, R0, @@return
	NOP
		SW R0, o_speed_h (T0)
		SW R0, o_speed_x (T0)
		SW R0, o_speed_z (T0)
	@@return:
	JR RA
	SW R0, o_move_flags (T0)

@shrink:
	LW T0, g_current_obj_ptr
	LI.S F5, 0.35
	L.S F4, o_scale_x (T0)
	SUB.S F4, F4, F5
	S.S F4, o_scale_x (T0)
	S.S F4, o_scale_y (T0)
	LI.S F5, 1.25
	L.S F4, o_hitbox_radius (T0)
	SUB.S F4, F4, F5
	S.S F4, o_hitbox_radius (T0)
	ADD.S F5, F5, F5
	L.S F4, o_hitbox_height (T0)
	SUB.S F4, F4, F5
	JR RA
	S.S F4, o_hitbox_height (T0)
 
