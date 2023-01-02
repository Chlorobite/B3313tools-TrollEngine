/* labels.asm + helper-functions.asm
Falcobuster's Labels and Helper Functions v4.5.0
These two files are public domain. You may use, modify, and distribute them
however you wish without restriction. Preserving this header comment is
appreciated, but not required.
https://gitlab.com/mpharoah/sm64-romhacking-stuff
*/

; NOTE: some types defining rotation are listed as "int (sign extended short)"
; This means that the value is stored as a signed 32-bit integer, but in normal
; gameplay it's really just a signed 16-bit short that has been sign extended to
; 32 bits. Before storing a rotation, I usually do this: (assuming rotation is in AT)
; SLL AT, AT, 0x10
; SRA AT, AT, 0x10
; This ensures that the value is a 16-bit value sign extended to 32 bits.

; Global structs
g_mario equ 0x8033B170				; struct
g_current_obj_ptr equ 0x80361160	; pointer
g_mario_obj_ptr equ 0x80361158		; pointer
g_camera_state equ 0x8033C698		; struct -- see camera state struct below
g_is_invulnerable equ 0x8033B272	; short (boolean) -- is Mario currently invulnerable
g_displayed_coins equ 0x8033B262	; short -- number of coins shown on the HUD (not the same as actual coins)
g_level_num equ 0x8032DDF8			; short -- level number shown in Quad64
g_course_num equ 0x8033BAC6			; short -- course number shown in game (save file uses this minus 1)
g_camera_position equ 0x8033C6A4	; float[3] -- x, y, and z co-ordinates of the camera (read-only)
g_camera_fov equ 0x8033C5A4			; float -- camera vertical field of view (in degrees)
g_save_file_num equ 0x8032DDF4		; short -- current save file number (starts at 1, game usually subtracts one from this before using it)
g_display_list_head equ 0x8033B06C	; pointer
g_global_timer equ 0x8032D5D4		; unsigned int
g_cutscene_finished	equ 0x8033CBC8	; int (boolean) -- 0 if a cutscene is active, 1 otherwise
g_timestop_flags equ 0x8033D480		; unsigned int

; Mario struct
m_action equ 0xC				; unsigned int
m_prev_action equ 0x10			; unsigned int
m_subaction equ 0x18 			; unsigned short
m_action_timer equ 0x1A			; unsigned short
m_action_arg equ 0x1C			; unsigned int
m_iframes equ 0x26				; short -- invulnerability frames
m_angle equ 0x2C				; short[3]
	m_angle_pitch equ 0x2C		; short
	m_angle_yaw equ 0x2E		; short
	m_angle_roll equ 0x30		; short
m_angle_vel equ 0x32			; short[3] -- angular velocity
	m_angle_vel_pitch equ 0x32 	; short
	m_angle_vel_yaw equ 0x34 	; short
	m_angle_vel_roll equ 0x36 	; short
m_slide_angle_yaw equ 0x38		; short
m_position equ 0x3C				; float[3]
	m_x equ 0x3C				; float
	m_y equ 0x40				; float
	m_z equ 0x44				; float
m_speed equ 0x48				; float[3]
	m_speed_x equ 0x48			; float
	m_speed_y equ 0x4C			; float
	m_speed_z equ 0x50			; float
m_speed_h equ 0x54				; float
m_slide_speed_x equ 0x58		; float
m_slide_speed_z equ 0x5C		; float
m_wall_ptr equ 0x60				; pointer -- referenced wall
m_ceiling_ptr equ 0x64			; pointer
m_floor_ptr equ 0x68			; pointer
m_ceiling_height equ 0x6C		; float
m_floor_height equ 0x70			; float
m_floor_pitch equ 0x74			; short
m_water_level equ 0x76			; short
m_held_object equ 0x7C			; pointer
m_used_object equ 0x80			; pointer
m_area equ 0x90					; pointer
m_camera equ 0x94				; pointer
m_controller equ 0x9C			; pointer -- see controller struct below
m_coins equ 0xA8				; short -- not automatically reflected by the HUD (see g_displayed_coins)
m_stars equ 0xAA				; short
m_lives equ 0xAD				; short
m_health equ 0xAE				; short (fixed point) -- upper byte is integer health, lower byte is 1/256th health units
m_hurt_counter equ 0xB2			; unisgned byte -- if > 0, damage mario by 1/4 health next frame and decrement
m_heal_counter equ 0xB3 		; unsigned byte -- same as above, but heals
m_squish_timer equ 0xB4			; unsigned byte -- decrements; 0 if not squished
m_opacity equ 0xB5				; unsigned byte -- opacity while in a fading warp
m_cap_timer equ 0xB6			; unsigned short
m_peak_height equ 0xBC			; float -- Mario's highest y co-ordinate since he last touched the ground. Used for fall damage


; Controller struct
c_analog_short_x equ 0x0	; short -- analog stick horizontal position [-80,80]
c_analog_short_y equ 0x2	; short -- analog stick vertical position [-80,80]
c_analog_float_x equ 0x4	; float -- analog stick horizontal position [-64,64]
c_analog_float_y equ 0x8	; float -- analog stick vertical position [-64,64]
c_analog_float_mag equ 0xC	; float -- analog stick distance from centre [0,64]
c_buttons_held equ 0x10		; unsigned short -- flags for buttons currently pressed (see constants below)
c_buttons_pressed equ 0x12	; unsigned short -- flags for buttons pressed this frame (see constants below)

; Controller Button Flag Constants
C_BUTTON_C_RIGHT equ		0x0001
C_BUTTON_C_LEFT equ			0x0002
C_BUTTON_C_DOWN equ			0x0004
C_BUTTON_C_UP equ			0x0008
C_TRIGGER_R equ				0x0010
C_TRIGGER_L equ				0x0020
C_BUTTON_D_PAD_RIGHT equ	0x0100
C_BUTTON_D_PAD_LEFT equ		0x0200
C_BUTTON_D_PAD_DOWN equ		0x0400
C_BUTTON_D_PAD_UP equ		0x0800
C_BUTTON_START equ			0x1000
C_TRIGGER_Z equ				0x2000
C_BUTTON_B equ				0x4000
C_BUTTON_A equ				0x8000

; Object struct
; The following are graph node properties inherited by objects. So long as the 0x1 object flag is enabled, the object properties
; are automatically copied to the corresponding graph node properties
o_gfx_flags equ 0x2				; unsigned short
o_animation_frame equ 0x40		; short
o_parent equ 0x68				; pointer
o_prev_obj equ 0x04				; pointer
o_gfx_angle equ 0x1A			; short[3] -- angle used for rendering; automatically copied from o_position if the OBJ_FLAG_UPDATE_GFX flag is set
	o_gfx_angle_pitch equ 0x1A	; short
	o_gfx_angle_yaw equ 0x1C	; short
	o_gfx_angle_roll equ 0x1E	; short
o_gfx_position equ 0x20			; float[3] -- position the object is rendered at; automatically copied from o_position if the OBJ_FLAG_UPDATE_GFX flag is set
	o_gfx_x equ 0x20			; float
	o_gfx_y equ 0x24			; float
	o_gfx_z equ 0x28			; float
o_scale equ 0x2C				; float[3]
	o_scale_x equ 0x2C			; float
	o_scale_y equ 0x30			; float
	o_scale_z equ 0x34			; float
o_active_flags equ 0x74			; short
o_num_collided_objects equ 0x76	; short
o_collided_objects equ 0x78		; pointer[4] -- 4 pointers stored contiguously
o_flags equ 0x8C				; unsigned int
o_intangibility_timer equ 0x9C	; int -- make negative to be infinite
o_position equ 0xA0				; float[3]
	o_x equ 0xA0				; float
	o_y equ 0xA4				; float
	o_z equ 0xA8				; float
o_speed equ 0xAC				; float[3]
	o_speed_x equ 0xAC			; float
	o_speed_y equ 0xB0			; float
	o_speed_z equ 0xB4			; float
o_speed_h equ 0xB8				; float -- horizontal speed. Use the decompose_speed function to set o_speed_x and o_speed_z
o_move_angle equ 0xC4			; int[3] (sign extended short) 
	o_move_angle_pitch equ 0xC4	; int (sign extended short) 
	o_move_angle_yaw equ 0xC8	; int (sign extended short)
	o_move_angle_roll equ 0xCC	; int (sign extended short)
o_face_angle equ 0xD0			; int[3] (sign extended short)
	o_face_angle_pitch equ 0xD0	; int (sign extended short)
	o_face_angle_yaw equ 0xD4	; int (sign extended short)
	o_face_angle_roll equ 0xD8	; int (sign extended short)
o_gfx_y_offset equ 0xDC			; float -- offsets the visual y position from the object's actual y position
o_gravity equ 0xE4				; float -- normal gravity is negative (-4 for Mario)
o_floor_height equ 0xE8			; float -- height of the floor beneath the object (doesn't work for Mario object- use m_floor_height on the Mario struct instead)
o_move_flags equ 0xEC			; unsigned int
o_animation_state equ 0xF0		; integer
; offsets 0xF4 to 0x0x110 are specific to the behaviour, and can be safely used for your custom objects
o_angle_vel equ 0x114			; int[3] (sign extended short)
	o_angle_vel_pitch equ 0x114	; int (sign extended short)
	o_angle_vel_yaw equ 0x118	; int (sign extended short)
	o_angle_vel_roll equ 0x11C	; int (sign extended short)
o_animation_pointer equ 0x120	; pointer
o_held_state equ 0x124			; unsigned int (0 = not held, 1 = held, 2 = thrown, 3 = dropped) -- see OBJ_HOLDABLE in behaviour-script-macros.asm
o_wall_hitbox_radius equ 0x128	; float (used by collision processing functions to detect collisions with walls)
o_drag equ 0x12C				; float
o_interaction equ 0x130			; unsigned int
o_interaction_status equ 0x134	; unsigned int
o_relative_position equ 0x138	; float[3] -- See OBJ_RELATIVE_TO_PARENT in behaviour-script-macros.asm
	o_relative_x equ 0x138		; float -- See OBJ_RELATIVE_TO_PARENT in behaviour-script-macros.asm
	o_relative_y equ 0x13C		; float -- See OBJ_RELATIVE_TO_PARENT in behaviour-script-macros.asm
	o_relative_z equ 0x140		; float -- See OBJ_RELATIVE_TO_PARENT in behaviour-script-macros.asm
o_param2 equ 0x144				; int -- for some reason, the game copies B.Param 2 here ¯\_(ツ)_/¯
o_state equ 0x14C				; unsigned int -- A.K.A. action
o_substate equ 0x150			; unsigned int -- automatically reset to 0 when the action changes
o_timer equ 0x154				; int -- automatically increments every frame and resets to 0 when the state changes
o_bounce equ 0x158				; float
o_distance_to_mario equ 0x15C	; float -- automatically computed if OBJ_STORE_DISTANCE_TO_MARIO object flag is set
o_angle_to_mario equ 0x160		; int (sign extended float) -- automatically computed if OBJ_STORE_ANGLE_TO_MARIO object flag is set
o_home equ 0x164				; float[3] -- automatically initialized if BHV_STORE_HOME is called in the behaviour script
	o_home_x equ 0x164			; float
	o_home_y equ 0x168			; float
	o_home_z equ 0x16C			; float
o_friction equ 0x170			; float
o_buoyancy equ 0x174			; float
o_opacity equ 0x17C				; int (but only the lower byte actually matters)
o_collision_damage equ 0x180	; int -- or coin value if using the coin interaction
o_health equ 0x184				; int
o_arg0 equ 0x188				; byte -- B. Param 1
o_arg1 equ 0x189				; byte -- B. Param 2
o_arg2 equ 0x18A				; byte -- B. Param 3
o_arg3 equ 0x18B				; byte -- B. Param 4
o_prev_state equ 0x18C			; unsigned int -- automatically set by the game as you change o_state
o_interaction_arg equ 0x190		; unsigned int -- checked by interaction processing code, what it does depends on the interaction
o_collision_distance equ 0x194	; float
o_num_loot_coins equ 0x198		; integer
o_render_distance equ 0x19C		; float
o_room equ 0x1A0				; int -- ID of the room the object is in
o_wall_angle equ 0x1B4			; int (sign extended short) 
o_floor_ptr equ 0x1C0			; pointer -- pointer to floor triangle beneath the object (doesn't work for Mario object, use m_floor_ptr on the Mario struct)
o_hitbox_radius equ 0x1F8		; float
o_hitbox_height equ 0x1FC		; float
o_hurtbox_radius equ 0x200		; float
o_hurtbox_height equ 0x204		; float
o_hitbox_down_offset equ 0x208	; float
o_behaviour equ 0x20C			; pointer -- virtual memory address (NOT segmented) of the behaviour script
o_collision_pointer equ 0x218	; pointer

; flags used by o_active_flags
AF_IS_OBJECT equ			0x0001 ; if this is not set, the object is effectively deleted (represents an empty object slot)
AF_FAR_AWAY equ				0x0002 ; set if the object is farther away than its render distance, and the OBJ_ALWAYS_ACTIVE object flag is not set
AF_DIFFERENT_ROOM equ		0x0008 ; in a different room than Mario
AF_UNIMPORTANT equ			0x0010 ; objects in the OBJ_LIST_PARTICLES list have this set
AF_ACTIVE_IN_TIMESTOP equ	0x0020 ; behaviour is still processed while objects are in timestop
AF_GHOSTLY equ				0x0040 ; does not collide with Vanish Cap surfaces
AF_DITHERED_ALPHA equ		0x0080 ; dither object when not fully opaque
AF_IGNORES_WATER equ		0x0400 ; always detects water level as -11000

; flags used by o_move_flags
MF_JUST_LANDED equ			0x0001
MF_GROUNDED equ				0x0002 ; oddly enough, this is mutally exclusive with MF_JUST_LANDED, so you may want to check both
MF_JUST_LEFT_GROUND equ		0x0004
MF_JUST_ENTERED_WATER equ	0x0008
MF_AT_WATER_SURFACE equ		0x0010
MF_UNDERWATER equ			0x0020 ; not set if MF_GROUNDED_UNDERWATER is
MF_GROUNDED_UNDERWATER equ	0x0040
MF_AIRBORNE equ				0x0080
; 0x100 is unused
MF_HIT_WALL equ				0x0200
MF_HIT_EDGE equ				0x0400
MF_OVER_LAVA equ			0x0800
MF_JUST_LEFT_WATER equ		0x1000
MF_JUST_BOUNCED equ			0x2000

; interactions
INT_OWL equ				0x00000001
INT_GRABBABLE equ		0x00000002
INT_DOOR equ			0x00000004
INT_DAMAGE equ			0x00000008
INT_COIN equ			0x00000010
INT_CAP equ				0x00000020
INT_POLE equ			0x00000040
INT_KOOPA equ			0x00000080
INT_SPINY equ			0x00000100
INT_BREAKABLE equ		0x00000200
INT_WIND equ			0x00000400
INT_WARP_DOOR equ		0x00000800
INT_STAR_OR_KEY equ		0x00001000
INT_WARP equ			0x00002000
INT_CANNON equ			0x00004000
INT_BOUNCE_TOP equ		0x00008000
INT_WATER_RING equ		0x00010000
INT_BULLY equ			0x00020000
INT_FLAME equ			0x00040000
INT_KOOPA_SHELL equ		0x00080000
INT_TRAP_TURN equ		0x00100000
INT_MR_BLIZZARD equ		0x00200000
INT_HIT_FROM_BELOW equ	0x00400000
INT_TEXT equ			0x00800000
INT_TORNADO equ			0x01000000
INT_WHIRLPOOL equ		0x02000000
INT_CLAM_OR_BUBBA equ	0x04000000
INT_BBH_ENTRANCE equ	0x08000000
INT_SNUFIT_BULLET equ	0x10000000
INT_SHOCK equ			0x20000000
INT_SOLID equ			0x40000000

; flags used by o_interaction_arg
; for INT_WARP
IA_FADING_WARP equ		0x0001
; for damaging interactions
IA_DELAY_IFRAMES equ	0x0002
IA_BIG_KNOCKBACK equ	0x0008
; for INT_GRABBABLE
IA_GRABS_MARIO equ		0x0004 ; Also makes the object heavy
IA_HOLDABLE_NPC equ		0x0010 ; Allows the object to be gently dropped, and sets vertical speed to 0 when dropped with no forwards velocity
IA_DROP_IMMEDIATELY equ	0x0040 ; This gets set by grabbable NPCs that talk to Mario to make him drop them after the dialog is finished
IA_KICKABLE equ			0x0100
IA_NOT_GRABBABLE equ	0x0200 ; Used by Heavy-Ho to allow it to throw Mario, without Mario being able to pick it up
; for INT_DOOR
IA_STAR_DOOR equ		0x0020
; for INT_BOUNCE_TOP
IA_TWIRL_BOUNCE equ		0x0080
; for INT_STAR_OR_KEY
IA_NO_EXIT equ			0x0400
IA_GRAND_STAR equ		0x0800
; for INT_TEXT
IA_SIGN equ				0x1000
IA_NPC equ				0x4000
; for INT_CLAM_OR_BUBBA
IA_EATS_MARIO equ		0x2000

; collision triangle struct
t_collision_type equ 0x0	; unsigned short
t_min_y equ 0x6				; short
t_max_y equ 0x8				; short
t_normal_x equ 0x1C			; float
t_normal_y equ 0x20			; float
t_normal_z equ 0x24			; float
t_object equ 0x2C			; pointer -- pointer to the object this collision triangle belongs to, or NULL (0) if it's level geometry

; camera state struct (read-only)
; These values get copied into the struct from other places.
; Changing these values won't affect the camera, but will mess up analog controls
cam_x equ 0x8C		; float
cam_y equ 0x90		; float
cam_z equ 0x94		; float
cam_pitch equ 0x4C	; short
cam_yaw equ 0x4E	; short

/* Functions */

/* set_object_hitbox 
a0: [pointer] object to set hitbox for
a1: [pointer] pointer to hitbox
Hitbox struct format:
	[uint] interaction
	[byte] vertical offset down
	[sbyte] damage (for damaging interactions) or coin value (for coin interaction)
	[sbyte] health
	[sbyte] loot coins
	[short] hitbox radius
	[short] hitbox height
	[short] hurtbox radius
	[short] hurtbox height
*/
set_object_hitbox equ 0x802A34A4

/* set_animation
Sets the object's current animation
a0: [int] animation index
*/
set_animation equ 0x8029F4B4

/* set_animation
Sets the object's current animation (does not reset the animation if the animation
is the same as the current one), and returns whether the current animation frame
is equal to a1
a0: [int] animation index
a1: [int] check if the current animation frame is equal to this value
[out] v0: [bool] non-zero if the current animatino frame is equal to A1
*/
set_animation_and_check_frame equ 0x802FA39C

/* check_done_animation
Checks if the current animation has finished
[out] v0: [bool] non-zero if the current animation has completed
*/
check_done_animation equ 0x8029FF04

/* set_sound
Plays a sound, using a vector from the object to the camera to determine volume
a0: [uint] sound
a1: [pointer] pointer to a vector from the sound origin to the camera (add 0x54 to an object pointer to get this pointer)
*/
set_sound equ 0x8031EB00

/* get_dist_3d
Gets the distance between two objects
a0: [pointer] object 1
a1: [pointer] object 2
[out] f0: [float] distance
*/
get_dist_3d equ 0x8029E2F8

/* get_dist_2d
Gets the lateral distance (on the xz plane) between two objects
a0: [pointer] object 1
a1: [pointer] object 2
[out] f0: [float] distance
*/
get_dist_2d equ 0x8029E27C

/* sqrt
Computes the square root of a number
NOTE: You can simply use the MIPS instruction SQRT.S instead
f12: number to square root
[out] f0: result
*/
sqrt equ 0x80323A50

/* decompose_speed
Sets the values of o_speed_x and o_speed_z using the values of o_speed_h and
o_move_angle_yaw
*/
decompose_speed equ 0x802A1308

/* decompose_speed
Sets the values of o_speed_x and o_speed_z using the values of o_speed_h and
o_move_angle_yaw, and then moves the object using these speeds
*/
decompose_speed_and_move equ 0x802A120C

/* obj_update_floor
Upadates the object's references to floors
*/
obj_update_floor equ 0x802A20F4

/* obj_update_floor_and_walls
Upadates the object's references to floors and walls, and sets the move flags
accordingly. You must set o_wall_hitbox_radius for wall collision resolution to
function properly.
*/
obj_update_floor_and_walls equ 0x802A2320

/* resolve_wall_collisions
Updates the object's references to walls, and updates the move flags. You must
set o_wall_hitbox_radius for wall collision resolution to function properly.
*/
obj_resolve_wall_collisions equ 0x802A1F3C


/* process_collision
Processes collision for objects with triangle collision. Would typically be
called after the behaviour loop by the behaviour script, though you can just
call it at the end of your behaviour loop instead
*/
process_collision equ 0x803839CC

/* are_objects_colliding
a0: [pointer] object 1
a1: [pointer] object 2
[out] v0: [bool] non-zero if the objects are colliding
*/
are_objects_colliding equ 0x802A1424

/* set_mario_speed
Sets Mario's forward velocity
a0: [pointer] pointer to mario struct (use g_mario)
a1: [float] speed
*/
set_mario_speed equ 0x80251708

/* mark_object_for_deletion
Marks the object slot as free, effectively deleting the object
(you can also just set o_active_flags to 0 instead)
a0: [pointer] object to delete
*/
mark_object_for_deletion equ 0x802A0568

/* get_nearest_object_with_behaviour
Gets the nearest object with the given behaviour script, or NULL (0) if no such
object exists. "Nearest" is with respect to the current object
a0: [segmented pointer] behaviour -- same value you would use in Quad64
[out] v0: [pointer] nearest object or NULL (0)
*/
get_nearest_object_with_behaviour equ 0x8029F95C

/* shake_screen
The game always passes in a value of 1 as the argument, but it passes this into
another function that branches to different code depending on if the value is 1,
2, 3, or 4. Not sure what these other values do yet.
a0: [int] effect type???? (use 1)
*/
shake_screen equ 0x802A50FC

/* spawn_particles
Spawns particles at the current object's position.
a0: [pointer] pointer to SpawnParticlesInfo struct
SpawnParticlesInfo struct format:
	[sbyte] behaviour argument
	[sbyte] number of particles
	[byte] modelId
	[sbyte] vertical offset
	[sbyte] base horizontal velocity
	[sbyte] random horizontal velocity range
	[sbyte] base vertical velocity
	[sbyte] random vertical velocity range
	[sbyte] gravity
	[sbyte] drag
	[float] base size
	[float] random size range
*/
spawn_particles equ 0x802A32AC

/* print
Prints text to the screen using the colourful text. Not all characters are
supported (there is no J, Q, V, X, or Z). It is safe to call this function
anywhere without messing up the graphics context. This function supports a
minimal subset of standard C printf formats, supporting only %d and %x formats.
For example, to print a 0-padded 4-digit hexidecimal value, use %04x
a0: [int] x screen position
a1: [int] y screen position
a2: [pointer] pointer to a null-terminated string describing what to print
a3: [int] argument value (number to print where %d or %x appears)
*/
print equ 0x802D62D8

/* turn_move_angle_towards_target_angle
Moves the current object's o_move_angle_yaw towards a target value
a0: [short] target angle
a1: [short] maximum angle to change o_move_angle_yaw by
[out] v0: [bool] non-zero if the object is already facing the target direction
*/
turn_move_angle_towards_target_angle equ 0x8029E5EC

/* turn_angle
Moves an angle towards a target angle
a0: [short] starting angle
a1: [short] target angle
a2: [short] maximum angle change
[out] v0: [short] resulting angle
*/
turn_angle equ 0x8029E530

/* is_animation_playing
[out] v0: [bool] non-zero if the current object is playing an animation
*/
is_animation_playing equ 0x8029FF04

/* get_random_short
[out] v0: [ushort] a random 16-bit value
*/
get_random_short equ 0x80383BB0

/* get_random_float
[out] f0: [float] a random float in the range [0,1)
*/
get_random_float equ 0x80383CB4

/* scale_object
a0: [pointer] object to scale
a1: [float] scaling value
*/
scale_object equ 0x8029F404

/* obj_angle_to_home
Returns the angle from the current object's current position to its home position
[out] v0: [short] angle
*/
obj_angle_to_home equ 0x802A2748

/* obj_xz_dist_from_home
Returns the lateral distance (on the xz plane) from the current object to its home
[out] f0: [float] distance
*/
obj_xz_dist_from_home equ 0x802A1634

/* take_damage_and_knockback
Causes Mario to take damage and knockback
a0: [pointer] pointer to mario struct (use g_mario)
a1: [pointer] pointer to the object that is the source of the damage
[out] v0: [bool] non-zero if Mario did take damage (could be 0 if Mario is invincible)
*/
take_damage_and_knockback equ 0x8024D998

/* obj_show_dialog
Shows a dialog message and puts Mario into the reading message action. You should
call this every frame until it returns a non-zero value
a0: [int] something to do with Mario's state?? (use 2)
a1: [int] dialog flags? (use 0x1 for normal dialog, or 0x11 to leave timestop enabled after the dialog closes)
a2: [int] cutscene ID (use 0xA2 for normal text, and 0xA3 for text with a yes/no option)
a3: [int] messageId (same ID you would use in text manager)
[out] v0: [bool] dialog response (0 if dialog has not been closed yet)
*/
obj_show_dialog equ 0x802A4BE4

/* copy_object_pos
a0: [pointer] object to copy position to
a1: [pointer] object to copt position from
*/
copy_object_pos equ 0x8029F120

/* obj_move_standard
a0: [short] floor slope angle -- uses degrees instead of the usual angle format because reasons (make negative to prevent it from walking off edges. Might require additional flags for this to work correctly?)
*/
obj_move_standard equ 0x802A2348

/* simple_move
Moves the current object using its x, y, and z speeds
*/
move_simple equ 0x8029F070

/* set_music
a0: [byte] music channel? (can be 0, 1, or 2. You can have up to 3 songs playing at the same time by putting them on different channels)
a1: [byte] music ID (same value that Bowser's Blueprints shows for music)
a2: [short] ?????? (set to 0)
*/
set_music equ 0x80320544

/* obj_was_attacked
[out] v0: [bool] non-zero if the object was attacked (eg. punched, jumped at from below,
ground pounded, or anything else that would destroy a breakable box)
*/
obj_was_attacked equ 0x802A51AC

/* get_or_set_camera_mode
a0: [byte] 0 = do not change camera, 1 = switch to Mario cam, 2 = switch to Lakitu cam
[out] v0: [byte] the current camera (1 = Mario, 2 = Lakitu)
*/
get_or_set_camera_mode equ 0x80288718

/* angle_to_object
a0: [pointer] source object
a1: [pointer] target object
[out] v0: [short] angle
*/
angle_to_object equ 0x8029E694

/* print_encoded_text
Call encode_text (see helper-functions.asm) first to convert ASCII text into
encoded text. You must also call begin_print_encoded_text before calling this
function, and then call end_print_encoded_text after all of your calls to
print_encoded_text.
a0: [short] x position
a1: [short] y position
a2: [pointer] encoded text
*/
print_encoded_text equ 0x802D77DC

/* malloc
Tries to allocate memory on the main pool (not the same as malloc in C)
a0: [uint] bytes of memory to allocate
a1: [uint] side (0 = left side, 1 = right side)
[out] v0: [pointer] pointer to allocated memory, or NULL if not enough space
*/
malloc equ 0x80278120

/* free
Frees memory that was allocated on the main pool.
IMPORTANT: This must be the most recently allocated block from its side of the
memory pool!
a0: [pointer] pointer that was allocated using malloc
[out] v0: [uint] free space remaining in the pool
*/
free equ 0x80278238

/* matrix_transform
Applies an affine transformation defined by a 4x4 matrix to a 3D vector of shorts.
The vector is modified in-place
a0: [pointer] pointer to the 4x4 matrix of floats
[ref] a1: [pointer] pointer to the 3D vector of shorts to transform
*/
matrix_transform equ 0x8037A348

/* start_cutscene
Starts a cutscene with the given object as the focus. Does not automatically set
the time stop state. You will need to do that yourself. To end the cutscene, use
the end_cutscene function in helper-functions.asm
a0: [usigned byte] cutscene ID (valid values are between 130 and 181. Star spawn is 173)
a1: [pointer] object to focus
*/
start_cutscene equ 0x8029000C

/* obj_reset_to_home
Resets the object's position to its home position
*/
obj_reset_to_home equ 0x802A184C

/* int_to_string
Converts a signed integer into an encoded string and store it at the given
position and advance the string position index. The string is not null
terminated. The string is store in ASCII format. You muist call encode_text (in
helper_functions.asm) to encode the text if you want to use this with the
print_encoded_text function. Note that negative values are prefixed with an M
instead of a hyphen.
a0: [int] the number to convert to a string
a1: [int] numerical base (eg. 10 for decimal, 16 for hex, etc.)
a2: [pointer to string] pointer to where the resulting numeric string will be written
a3: [pointer to integer] pointer to a number that will be incremented by the number of characters writen
SP+0x13: [unsigned byte] minimum width (number is padded if the numeric string is shorter than this)
SP+0x17: [byte (boolean)] if non-zero, the string is 0 padded if shorter than minimum width (otherwise, padded with 0xFF)
*/
int_to_string equ 0x802D5E54

/* commit_all_writes
Commits all data writes from the CPU cache to physical RAM. Call this before
swapping segment data. This is the osWritebackDCacheAll function from libultra.
*/
commit_all_writes equ 0x80322F40

/* invalidate_instruction_cache
Invalidates the instruction cache for the given region of memory. Call this
after doing self-modifying code or DMAing instructions from the ROM. This is the
osInvalICache function from libultra.
a0: [pointer] pointer to the start of memory to clear the cache for
a1: [unsigned int] size of the memory region in bytes
*/
invalidate_instruction_cache equ 0x80324610

/* invalidate_data_cache
Invalidates the data cache for the given region of memory. Call this after
DMAing non-instruction data from the ROM. This is the osInvalDCache function
from libultra.
a0: [pointer] pointer to the start of memory to clear the cache for
a1: [unsigned int] size of the memory region in bytes
*/
invalidate_data_cache equ 0x803243B0

/* lower_background_audio_volume
Fades the music volume down to a quieter volume
*/
lower_background_audio_volume equ 0x80290BA4

/* restore_background_audio_volume
Fades the music volume back to its normal level
*/
restore_background_audio_volume equ 0x80320EC4

/* get_is_mashing_out
Returns a non-zero value if the player is performing an input that should
increment the counter to mash out of a grab.
(See https://www.youtube.com/watch?v=_R8c70zuJSw for mechanics)
[out] v0: [bool] non-zero if mashing out
*/
get_is_mashing_out equ 0x802A5358

/* spawn_coins
Spawns a number of yellow coins at the object's position. The coins have a
random speed in a random direction.
a0: [pointer] pointer to object where the coins should spawn at
a1: [byte] number of coins to spawn
*/
spawn_coins equ 0x802E5760
