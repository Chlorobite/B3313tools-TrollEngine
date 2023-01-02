/* labels.asm + helper-functions.asm + behaviour-script-macros.asm
Falcobuster's Labels and Helper Functions v4.4.0
These two files are public domain. You may use, modify, and distribute them
however you wish without restriction. Preserving this header comment is
appreciated, but not required.
https://gitlab.com/mpharoah/sm64-romhacking-stuff
*/

.definelabel OBJ_LIST_EXPLOSION, 0x2 ; things that can damage other objects (bombs, explosions, and bouncy cork boxes)
.definelabel OBJ_LIST_GENERIC, 0x4 ; nothing too special about this list. Pretty much equivalent to OBJ_LIST_LEVEL
.definelabel OBJ_LIST_INTERACTIVE, 0x5 ; objects that can detect hitbox collisions with other objects (not just Mario!)
.definelabel OBJ_LIST_LEVEL, 0x6 ; nothing too special about this list. Pretty much equivalent to OBJ_LIST_GENERIC
.definelabel OBJ_LIST_DEFAULT, 0x8 ; where objects without an object list explicitly set go. Does not process collision
.definelabel OBJ_LIST_SURFACE, 0x9 ; objects with triangle collision
.definelabel OBJ_LIST_MAGNETIC, 0xA ; objects that Mario can climb (poles/trees) or that pull him in (tornado/whirlpool) or push him (wind)
.definelabel OBJ_LIST_SPAWNER, 0xB ; objects that spawn other objects. Objects in this list get processed first
.definelabel OBJ_LIST_PARTICLES, 0xC ; unimportant objects that will get automatically despawned if more important objects need to load

; Object flags (for use with BHV_OR_FLAGS o_flags, <flags>)
.definelabel OBJ_FLAG_UPDATE_GFX, 0x1 ; copy position and angle from object properties to GraphNode properties (required for visual update)
.definelabel OBJ_AUTO_MOVE_XZ, 0x2 ; automatically move horizontally using o_move_angle_yaw and o_speed_h
.definelabel OBJ_AUTO_MOVE_Y, 0x4 ; automatically move vertically using o_speed_y (caps terminal velocity to -70)
.definelabel OBJ_AUTO_FACE_FORWARDS_HORIZONTAL, 0x8 ; copy o_move_angle_yaw to o_face_angle_yaw every frame
.definelabel OBJ_AUTO_FACE_FORWARDS, 0x10 ; same as above, but also copies pitch and roll
.definelabel OBJ_STORE_DISTANCE_TO_MARIO, 0x40 ; store the distance to Mario to o_distance_to_mario every frame
.definelabel OBJ_ALWAYS_ACTIVE, 0x80 ; keep active even when far away from Mario
.definelabel OBJ_RELATIVE_TO_PARENT, 0x200 ; transform the object relative to its parent using o_relative_[x|y|z] and store the world coordinates in o_[x|y|z] (parent object must have OBJ_COMPUTE_MATRIX_TRANSFORM flag for this to work)
.definelabel OBJ_HOLDABLE, 0x400 ; marks the object as being holdable with its current behaviour script. Picking up an object with this flag results in its o_held_state being updated. Picking up an object without this flag results in it changing to a different held object behaviour script.
.definelabel OBJ_COMPUTE_MATRIX_TRANSFORM, 0x820 ; computes and stores the object's matrix transform based on its position and angle. This is required for OBJ_RELATIVE_TO_PARENT to work on child objects
.definelabel OBJ_STORE_ANGLE_TO_MARIO, 0x2000 ; store the angle to Mario in the xz plane to o_angle_to_mario every frame
.definelabel OBJ_PERSISTENT, 0x4000 ; when this object despawns, don't respawn it when re-entering the area (only use this on objects in the level script, not dynamically spawned objects which are always transient)

.macro @SET, cmd, offset, value
	.if offset < 0x88
		.error "Cannot set value of property with offset less than 0x88"
	.endif
	.byte cmd, ((offset - 0x88) >> 2)
	.halfword value
.endmacro

; Starts a behaviour script and set the object list to use
.macro BHV_START, objectList
	.byte 0x00, objectList, 0x00, 0x00
.endmacro

; Wait for N frames before continuing with the behaviour script
.macro BHV_SLEEP, frames
	.halfword 0x0100, frames
.endmacro

; Wait for a number of frames given by the value of the property
.macro BHV_SLEEP_VAR, prop
	@SET 0x25, prop, 0
.endmacro

; Jumps to another behaviour script
.macro BHV_JUMP, segPtr
	.word 0x04000000, segPtr
.endmacro

; Jumps to another behaviour script, and stores the script return address
.macro BHV_JAL, segPtr
	.word 0x02000000, segPtr
.endmacro

; Jumps to back to the saved return address
.macro BHV_RETURN
	.word 0x03000000
.endmacro

; Repeats the behaviour commands in the loop for N frames, then continues with the script
.macro BHV_REPEAT_BEGIN, numLoops
	.halfword 0x0500, numLoops
.endmacro

.macro BHV_REPEAT_END
	.word 0x06000000
.endmacro

; Runs the behaviour commands in the loop every frame
.macro BHV_LOOP_BEGIN
	.word 0x08000000
.endmacro

.macro BHV_LOOP_END
	.word 0x09000000
.endmacro

; Ends a behaviour script (not necessary if using a loop)
.macro BHV_END
	.word 0x0A000000
.endmacro

; Calls an ASM function
.macro BHV_EXEC, pointer
	.word 0x0C000000, pointer
.endmacro

; Casts the value to a float, and adds it to the object property
; (NOTE: parameter must be at offset 0x88 or higher, so not a GraphNode property)
.macro BHV_ADD_FLOAT, prop, value
	@SET 0x0D, prop, value
.endmacro

; Casts the value to a float, and assigns it to the object property
; (NOTE: parameter must be at offset 0x88 or higher, so not a GraphNode property)
.macro BHV_SET_FLOAT, prop, value
	@SET 0x0E, prop, value
.endmacro

; Sums the floating point values in two properties and stores the result in
; another property
; (NOTE: parameter must be at offset 0x88 or higher, so not a GraphNode property)
.macro BHV_SUM_FLOATS, destProp, srcProp1, srcProp2
	.if destProp < 0x88 || srcProp1 < 0x88 || srcProp2 < 0x88
		.error "Cannot set value of property with offset less than 0x88"
	.endif
	.byte 0x1F, ((destProp - 0x88) >> 2), ((srcProp1 - 0x88) >> 2), ((srcProp2 - 0x88) >> 2)
.endmacro

; Adds the given value to the object property
; (NOTE: parameter must be at offset 0x88 or higher, so not a GraphNode property)
.macro BHV_ADD_INT, prop, value
	@SET 0x0F, prop, value
.endmacro

; Sets the object property
; (NOTE: parameter must be at offset 0x88 or higher, so not a GraphNode property)
.macro BHV_SET_INT, prop, value
	@SET 0x10, prop, value
.endmacro

; Sums the integer values in two properties and stores the result in another
; property
; (NOTE: parameter must be at offset 0x88 or higher, so not a GraphNode property)
.macro BHV_SUM_INTS, destProp, srcProp1, srcProp2
	.if destProp < 0x88 || srcProp1 < 0x88 || srcProp2 < 0x88
		.error "Cannot set value of property with offset less than 0x88"
	.endif
	.byte 0x20, ((destProp - 0x88) >> 2), ((srcProp1 - 0x88) >> 2), ((srcProp2 - 0x88) >> 2)
.endmacro

; ORs the object property with the given bitmask
.macro BHV_OR_FLAGS, prop, value
	@SET 0x11, prop, value
.endmacro

; Zeroes out the masked bits of the object property
; WARNING: Also clears the upper 16 bits. Don't use this with o_flags
.macro BHV_CLEAR_FLAGS, prop, value
	@SET 0x12, prop, value
.endmacro

; Sets the object's model
.macro BHV_SET_MODEL, modelId
	.halfword 0x1B00, modelId
.endmacro

; Despawns the object and ends the behaviour script
.macro BHV_DELETE
	.word 0x1D000000
.endmacro

; Drops the object to the floor
.macro BHV_DROP_TO_FLOOR
	.word 0x1E000000
.endmacro

; Billboard the object
.macro BHV_BILLBOARD
	.word 0x21000000
.endmacro

; Sets the property to the given value
.macro BHV_SET_WORD, prop, value
	@SET 0x27, prop, 0x00
	.word value
.endmacro

; Sets the initial animation
.macro BHV_SET_ANIMATION, index
	.byte 0x28, index
	.halfword 0x0000
.endmacro

; Spawns a child object, and sets o_param2 (but not o_arg1)
.macro BHV_SPAWN_OBJECT, modelId, behaviour, param2
	.halfword 0x2900, param2
	.word modelId, behaviour
.endmacro

.macro BHV_SET_COLLISION, segPtr
	.word 0x2A000000, segPtr
.endmacro

; Sets the object's hitbox size
.macro BHV_SET_HITBOX, radius, height, downOffset
	.word 0x2B000000
	.halfword radius, height, downOffset, 0x0000
.endmacro

; Sets the object's hurtbox size
.macro BHV_SET_HURTBOX, radius, height
	.word 0x2E000000
	.halfword radius, height
.endmacro

; Saves the object's initial position to its home location
.macro BHV_STORE_HOME
	.word 0x2D000000
.endmacro

; Sets the object interaction (equivalent to 0x272A...)
.macro BHV_SET_INTERACTION, interaction
	.word 0x2F000000, interaction
.endmacro

; Sets the object's physics properties. All values except for wallHitboxRadius are divided by 100
.macro BHV_SET_PHYSICS, wallHitboxRadius, gravity, bounciness, drag, friction, bouyancy
	.word 0x30000000
	.halfword wallHitboxRadius, gravity, bounciness, drag, friction, bouyancy
	.word 0x00000000
.endmacro

; Sets the property to a random short (prop = (random_short() >> rshift) + offset)
.macro BHV_SET_RANDOM_SHORT, prop, rshift, offset
	@SET 0x13, prop, offset
	.halfword rshift, 0x0000
.endmacro

; Sets the property to a random float (prop = (random_float() * range) + min)
.macro BHV_SET_RANDOM_FLOAT, prop, range, min
	@SET 0x14, prop, min
	.halfword range, 0x0000
.endmacro

; Scales the object by the given percentage (eg. 200 to double size)
.macro BHV_SCALE, scalePercent
	.halfword 0x3200, scalePercent
.endmacro
