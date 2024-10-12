#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "course_table.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "envfx_bubbles.h"
#include "game_init.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_misc_macros.h"
#include "level_table.h"
#include "level_update.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "mario_misc.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "spawn_object.h"
#include "spawn_sound.h"
#include "rumble_init.h"

/**
 * @file obj_behaviors.c
 * This file contains a portion of the obj behaviors and many helper functions for those
 * specific behaviors. Few functions besides the bhv_ functions are used elsewhere in the repo.
 */

#define o gCurrentObject

#define OBJ_COL_FLAG_GROUNDED   (1 << 0)
#define OBJ_COL_FLAG_HIT_WALL   (1 << 1)
#define OBJ_COL_FLAG_UNDERWATER (1 << 2)
#define OBJ_COL_FLAG_NO_Y_VEL   (1 << 3)
#define OBJ_COL_FLAGS_LANDED    (OBJ_COL_FLAG_GROUNDED | OBJ_COL_FLAG_NO_Y_VEL)

/**
 * Current object floor as defined in object_step.
 */
static struct Surface *sObjFloor;

/**
 * Set to false when an object close to the floor should not be oriented in reference
 * to it. Happens with boulder, falling pillar, and the rolling snowman body.
 */
static s8 sOrientObjWithFloor = TRUE;

/**
 * Keeps track of Mario's previous non-zero room.
 * Helps keep track of room when Mario is over an object.
 */
extern s16 sPrevCheckMarioRoom;

/**
 * Tracks whether or not Yoshi has walked/jumped off the roof.
 */
extern s8 sYoshiDead;

extern void *ccm_seg7_trajectory_snowman;
extern void *inside_castle_seg7_trajectory_mips;

void obj_orient_graph(struct Object *obj, f32 normalX, f32 normalY, f32 normalZ);
void calc_obj_friction(f32 *objFriction, f32 floor_nY);
void turn_obj_away_from_surface(f32 velX, f32 velZ, f32 nX, f32 nY, f32 nZ, f32 *objYawX,
                            f32 *objYawZ);

