#include "shadow_headers.h"

void make_shadow_vertex(Vtx *vertices, s8 index, struct Shadow s, s8 shadowVertexType) {
    f32 xPosVtx, yPosVtx, zPosVtx;

    u8 solidity = s.solidity;
    if (gShadowAboveWaterOrLava) {
        solidity = 200;
    }

    calculate_vertex_xyz(index, s, &xPosVtx, &yPosVtx, &zPosVtx, shadowVertexType);

    make_shadow_vertex_at_xyz(vertices, index, xPosVtx - s.parentX, yPosVtx - s.parentY, zPosVtx - s.parentZ, solidity, shadowVertexType);
}
