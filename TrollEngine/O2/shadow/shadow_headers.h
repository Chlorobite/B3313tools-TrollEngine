#include <PR/ultratypes.h>
#include <PR/gbi.h>
#include <math.h>

#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "geo_misc.h"
#include "level_table.h"
#include "memory.h"
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include "segment2.h"
#include "shadow.h"
#include "sm64.h"

/**
 * @file shadow.c
 * This file implements a self-contained subsystem used to draw shadows.
 */

/**
 * Encapsulation of information about a shadow.
 */
struct Shadow {
    /* The (x, y, z) position of the object whose shadow this is. */
    f32 parentX;
    f32 parentY;
    f32 parentZ;
    /* The y-position of the floor (or water or lava) underneath the object. */
    f32 floorHeight;
    /* Initial (unmodified) size of the shadow. */
    f32 shadowScale;
    /* (nx, ny, nz) normal vector of the floor underneath the object. */
    f32 floorNormalX;
    f32 floorNormalY;
    f32 floorNormalZ;
    /* "originOffset" of the floor underneath the object. */
    f32 floorOriginOffset;
    /* Angle describing "which way a marble would roll," in degrees. */
    f32 floorDownwardAngle;
    /* Angle describing "how tilted the ground is" in degrees (-90 to 90). */
    f32 floorTilt;
    /* Initial solidity of the shadow, from 0 to 255 (just an alpha value). */
    u8 solidity;
};

/**
 * Constant to indicate that a shadow should not be drawn.
 * This is used to disable shadows during specific frames of Mario's
 * animations.
 */
#define SHADOW_SOLIDITY_NO_SHADOW 0
/**
 * Constant to indicate that a shadow's solidity has been pre-set by a previous
 * function and should not be overwritten.
 */
#define SHADOW_SOILDITY_ALREADY_SET 1
/**
 * Constant to indicate that a shadow's solidity has not yet been set.
 */
#define SHADOW_SOLIDITY_NOT_YET_SET 2

/**
 * Constant to indicate any sort of circular shadow.
 */
#define SHADOW_SHAPE_CIRCLE 10
/**
 * Constant to indicate any sort of rectangular shadow.
 */
#define SHADOW_SHAPE_SQUARE 20

/**
 * Constant to indicate a shadow consists of 9 vertices.
 */
#define SHADOW_WITH_9_VERTS 0
/**
 * Constant to indicate a shadow consists of 4 vertices.
 */
#define SHADOW_WITH_4_VERTS 1

/**
 * A struct containing info about hardcoded rectangle shadows.
 */
typedef struct {
    /* Half the width of the rectangle. */
    f32 halfWidth;
    /* Half the length of the rectangle. */
    f32 halfLength;
    /* Flag for if this shadow be smaller when its object is further away. */
    s8 scaleWithDistance;
} shadowRectangle;


Gfx *create_shadow_player(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 solidity, s32 isLuigi);
Gfx *create_shadow_circle_9_verts(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 solidity);
Gfx *create_shadow_circle_4_verts(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 solidity);
Gfx *create_shadow_circle_assuming_flat_ground(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 solidity);
Gfx *create_shadow_rectangle(f32 halfWidth, f32 halfLength, f32 relY, u8 solidity);
Gfx *create_shadow_square(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 solidity, s8 shadowType);
Gfx *create_shadow_hardcoded_rectangle(f32 xPos, f32 yPos, f32 zPos, UNUSED s16 shadowScale, u8 solidity, s8 shadowType);
f32 atan2_deg(f32 a, f32 b);
f32 scale_shadow_with_distance(f32 initial, f32 distFromFloor);
f32 disable_shadow_with_distance(f32 shadowScale, f32 distFromFloor);
u8 dim_shadow_with_distance(u8 solidity, f32 distFromFloor);
f32 get_water_level_below_shadow(struct Shadow *s);
void make_shadow_vertex_at_xyz(Vtx *vertices, s8 index, f32 relX, f32 relY, f32 relZ, u8 alpha, s8 shadowVertexType);
Gfx *create_shadow_below_xyz(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 shadowSolidity, s8 shadowType);
void get_vertex_coords(s8 index, s8 shadowVertexType, s8 *xCoord, s8 *zCoord);
f32 extrapolate_vertex_y_position(struct Shadow s, f32 vtxX, f32 vtxZ);
void calculate_vertex_xyz(s8 index, struct Shadow s, f32 *xPosVtx, f32 *yPosVtx, f32 *zPosVtx, s8 shadowVertexType);
s8 init_shadow(struct Shadow *s, f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 overwriteSolidity);
