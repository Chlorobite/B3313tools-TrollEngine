#include <PR/ultratypes.h>

#include "sm64.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "game_init.h"
#include "geo_misc.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_table.h"
#include "level_update.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "obj_behaviors_2.h"
#include "object_constants.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "platform_displacement.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "seq_ids.h"
#include "spawn_sound.h"

#include "AI/stats_tracking.h"
#include "AI/personalization_helpers.h"

#define POS_OP_SAVE_POSITION 0
#define POS_OP_COMPUTE_VELOCITY 1
#define POS_OP_RESTORE_POSITION 2

#define o gCurrentObject

s8 obj_return_home_if_safe(struct Object *obj, f32 homeX, f32 y, f32 homeZ, s32 dist);
s8 obj_check_if_facing_toward_angle(u32 base, u32 goal, s16 range);
void obj_check_floor_death(s16 collisionFlags, struct Surface *floor);
s8 is_point_within_radius_of_mario(f32 x, f32 y, f32 z, s32 dist);

void treat_far_home_as_mario(f32 threshold);
s32 obj_forward_vel_approach(f32 target, f32 delta);
s32 obj_resolve_object_collisions(s32 *targetYaw);
s32 obj_bounce_off_walls_edges_objects(s32 *targetYaw);
s32 obj_resolve_collisions_and_turn(s16 targetYaw, s16 turnSpeed);
s16 obj_random_fixed_turn(s16 delta);
s16 random_linear_offset(s16 base, s16 range);
s32 obj_smooth_turn(s16 *angleVel, s32 *angle, s16 targetAngle, f32 targetSpeedProportion, s16 accel, s16 minSpeed, s16 maxSpeed);
s32 approach_f32_ptr(f32 *px, f32 target, f32 delta);
s32 obj_update_standard_actions(f32 scale);
s32 cur_obj_init_anim_and_check_if_end(s32 arg0);
s32 cur_obj_init_anim_check_frame(s32 arg0, s32 arg1);
s32 clamp_s16(s16 *value, s16 minimum, s16 maximum);

extern s8 gDddPaintingStatus;
#define BOWSERS_SUB_BEATEN 0x2
extern const Collision inside_castle_seg7_collision_ddd_warp[];
extern const Collision inside_castle_seg7_collision_ddd_warp_2[];
extern const Collision ssl_seg7_collision_07028274[];
extern const Collision ssl_seg7_collision_070282F8[];
extern const Collision ssl_seg7_collision_07028370[];
extern const Collision ssl_seg7_collision_070284B0[];
extern const Collision wdw_seg7_collision_07018528[];
