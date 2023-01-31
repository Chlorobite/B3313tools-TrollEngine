#include <ultra64.h>

#include "sm64.h"
#include "game/moving_texture.h"
#include "game/area.h"
#include "game/camera.h"
#include "game/rendering_graph_node.h"
#include "engine/math_util.h"
#include "game/memory.h"
#include "game/save_file.h"
#include "segment2.h"
#include "engine/surface_collision.h"
#include "game/geo_misc.h"
#include "game/rendering_graph_node.h"
#include "game/object_list_processor.h"

#include "AI/personalization_helpers.h"

/**
 * This file contains functions for generating display lists with moving textures
 * (abbreviated movtex). This is used for water, sand, haze, mist and treadmills.
 * Each mesh using this system has the animated vertices stored as an array of shorts.
 * The first entry is the texture movement speed. After that the vertices are stored
 * in one of two layouts: one without per-vertex color attributes and one with.
 * [speed, v0(x,y,z, s,t)      , v1(x,y,z, s,t)      , ...]
 * [speed, v0(x,y,z, r,g,b s,t), v1(x,y,z, r,g,b s,t), ...]
 * x, y, z = vertex position as integers
 * s, t = texture coordinates as 6.10 fixed point number. That means coordinates in
 * range [0, 1024] are a unique part of the image, after that it repeats the image.
 *
 * The first vertex 'v0' is special because all subsequent vertices inherit its
 * texture offset. So to animate e.g. a treadmill, the speed component arr[0] is
 * simply added to the s component arr[7] every frame and the texture scrolls
 * horizontally over the entire mesh without changing the rest of the array.
 * Note that while the system allows many kinds of vertex animations, in
 * practice the only animation used is horizontally scrolling textures.
 *
 * After updating the base mesh, the vertices are converted to the format the RSP
 * understands and a display list is generated. The RSP can buffer 16 vertices at
 * a time, and this code assumes everything fits in one buffer, so every moving
 * texture mesh must have at most 16 vertices. As a result some meshes are split
 * up into multiple parts, like the sand pathway inside the pyramid which has 3
 * parts. The water stream in the Cavern of the Metal Cap fits in one mesh.
 *
 * Apart from this general system, there is also a simpler system for flat
 * quads with a rotating texture. This is often used for water, but also
 * for mist, toxic haze and lava inside the volcano. One quad is described
 * by the struct MovtexQuad, and multiple MovtexQuads form a MovtexQuadCollection.
 * A geo node has an id that corresponds to the id of a certain MovtexQuadCollection,
 * which will then be matched with the id of entries in gEnvironmentRegions to get the
 * y-position. The x and z coordinates are stored in the MovtexQuads themself,
 * so the water rectangle is separate from the actually drawn rectangle.
 */

// First entry in array is texture movement speed for both layouts
#define MOVTEX_ATTR_SPEED 0

// Different layouts for vertices
#define MOVTEX_LAYOUT_NOCOLOR 0
#define MOVTEX_LAYOUT_COLORED 1

// Attributes for movtex vertices
#define MOVTEX_ATTR_X 1
#define MOVTEX_ATTR_Y 2
#define MOVTEX_ATTR_Z 3

// For MOVTEX_LAYOUT_NOCOLOR only
#define MOVTEX_ATTR_NOCOLOR_S 4
#define MOVTEX_ATTR_NOCOLOR_T 5

// For MOVTEX_LAYOUT_COLORED only
#define MOVTEX_ATTR_COLORED_R 4
#define MOVTEX_ATTR_COLORED_G 5
#define MOVTEX_ATTR_COLORED_B 6
#define MOVTEX_ATTR_COLORED_S 7
#define MOVTEX_ATTR_COLORED_T 8

/**
 * An object containing all info for a mesh with moving textures.
 * Contains the vertices that are animated, but also the display list which
 * determines the connectivity, as well as the texture, texture blend color
 * and drawing layer.
 */
struct MovtexObject {
    /// number that geo nodes have as parameter to refer to this mesh
    u32 geoId;
    /// which texture to use for this mesh, index into gMovtexIdToTexture
    s32 textureId;
    /// amount of moving vertices
    s32 vtx_count;
    /// segmented address to movtex mesh with vertices
    void *movtexVerts;
    /// display list inserted before moving triangles
    Gfx *beginDl;
    /// display list inserted after moving triangles
    Gfx *endDl;
    /// display list with the actual moving texture triangles.
    /// Assumes the animated vertices are buffered and correct texture is set
    Gfx *triDl;
    // if the list does not have per-vertex colors, all vertices have these colors
    u8 r;      /// red
    u8 g;      /// green
    u8 b;      /// blue
    u8 a;      /// alpha
    s32 layer; /// the drawing layer for this mesh
};

// Vertex colors for rectangles. Used to give mist a tint
#define MOVTEX_VTX_COLOR_DEFAULT 0 // no tint (white vertex colors)
#define MOVTEX_VTX_COLOR_YELLOW 1  // used for Hazy Maze Cave toxic haze
#define MOVTEX_VTX_COLOR_RED 2     // used for Shifting Sand Land around the Tox box maze

extern s8 gMovtexVtxColor;

void movtex_make_quad_vertex(Vtx *verts, s32 index, s16 x, s16 y, s16 z, s16 rot, s16 rotOffset,
                             f32 scale, u8 alpha) {
    register s16 s = 32.f * (32.f * scale - 1.f) * sins(rot + rotOffset);
    register s16 t = 32.f * (32.f * scale - 1.f) * coss(rot + rotOffset);
	// EDIT: darken moving textures at night
    register s8 maxColor = (nightMode ? 191 : 255);
	// END EDIT

    if (gMovtexVtxColor == MOVTEX_VTX_COLOR_YELLOW) {
        make_vertex(verts, index, x, y, z, s, t, maxColor, maxColor, 0, alpha);
    } else if (gMovtexVtxColor == MOVTEX_VTX_COLOR_RED) {
        make_vertex(verts, index, x, y, z, s, t, maxColor, 0, 0, alpha);
    } else {
        make_vertex(verts, index, x, y, z, s, t, maxColor, maxColor, maxColor, alpha);
    }
}
