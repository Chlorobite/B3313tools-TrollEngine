#include "shadow_headers.h"

void add_shadow_to_display_list(Gfx *displayListHead, Vtx *verts, s8 shadowVertexType, s8 shadowShape) {
    switch (shadowShape) {
        case SHADOW_SHAPE_CIRCLE:
            gSPDisplayList(displayListHead++, 0x020173A8);
            break;
        case SHADOW_SHAPE_SQUARE:
            gSPDisplayList(displayListHead++, 0x020173F0);
            break;
    }
    switch (shadowVertexType) {
        case SHADOW_WITH_9_VERTS:
            gSPVertex(displayListHead++, verts, 9, 0);
            gSPDisplayList(displayListHead++, 0x02017438);
            break;
        case SHADOW_WITH_4_VERTS:
            gSPVertex(displayListHead++, verts, 4, 0);
            gSPDisplayList(displayListHead++, 0x02017480);
            break;
    }
    gSPDisplayList(displayListHead++, 0x02017498);
    gSPEndDisplayList(displayListHead);
}
