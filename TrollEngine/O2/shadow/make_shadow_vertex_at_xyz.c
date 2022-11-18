#include "shadow_headers.h"

void make_shadow_vertex_at_xyz(Vtx *vertices, s8 index, f32 relX, f32 relY, f32 relZ, u8 alpha,
                               s8 shadowVertexType) {
    register s32 vtxX = round_float(relX);
    register s32 vtxY = round_float(relY);
    register s32 vtxZ = round_float(relZ);
    s16 textureX, textureY;

    switch (shadowVertexType) {
        case SHADOW_WITH_9_VERTS:
            get_texture_coords_9_vertices(index, &textureX, &textureY);
            break;
        case SHADOW_WITH_4_VERTS:
            get_texture_coords_4_vertices(index, &textureX, &textureY);
            break;
    }

    // Move the shadow up and over slightly while standing on a flying carpet.
    if (sMarioOnFlyingCarpet) {
        vtxX += 5;
        vtxY += 5;
        vtxZ += 5;
    }
    make_vertex( // shadows are black
        vertices, index, vtxX, vtxY, vtxZ, textureX << 5, textureY << 5, 255, 255, 255, alpha
    );
}
