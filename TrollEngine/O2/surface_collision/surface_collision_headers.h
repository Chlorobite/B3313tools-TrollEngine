#include <PR/ultratypes.h>

#include "sm64.h"
#include "game/debug.h"
#include "game/level_update.h"
#include "game/mario.h"
#include "game/object_list_processor.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "engine/math_util.h"

//#include "game/ai/personalization_helpers.h" 

s32 find_wall_collisions_from_list(struct SurfaceNode *surfaceNode, struct WallCollisionData *data);
struct Surface *find_floor_from_list(struct SurfaceNode *surfaceNode, s32 x, s32 y, s32 z, f32 *pheight);
struct Surface *find_ceil_from_list(struct SurfaceNode *surfaceNode, s32 x, s32 y, s32 z, f32 *pheight);
