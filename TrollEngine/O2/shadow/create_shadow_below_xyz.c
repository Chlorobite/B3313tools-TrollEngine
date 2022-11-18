#include "shadow_headers.h"

Gfx *create_shadow_below_xyz(f32 xPos, f32 yPos, f32 zPos, s16 shadowScale, u8 shadowSolidity,
                             s8 shadowType) {
    switch (shadowType) {
        case SHADOW_CIRCLE_9_VERTS:
            return create_shadow_circle_9_verts(xPos, yPos, zPos, shadowScale, shadowSolidity);
        case SHADOW_CIRCLE_4_VERTS:
            return create_shadow_circle_4_verts(xPos, yPos, zPos, shadowScale, shadowSolidity);
        case SHADOW_CIRCLE_4_VERTS_FLAT_UNUSED: // unused shadow type
            return create_shadow_circle_assuming_flat_ground(xPos, yPos, zPos, shadowScale, shadowSolidity);
        case SHADOW_SQUARE_PERMANENT:
            return create_shadow_square(xPos, yPos, zPos, shadowScale, shadowSolidity, shadowType);
        case SHADOW_SQUARE_SCALABLE:
            return create_shadow_square(xPos, yPos, zPos, shadowScale, shadowSolidity, shadowType);
        case SHADOW_SQUARE_TOGGLABLE:
            return create_shadow_square(xPos, yPos, zPos, shadowScale, shadowSolidity, shadowType);
        case SHADOW_CIRCLE_PLAYER:
            return create_shadow_player(xPos, yPos, zPos, shadowScale, shadowSolidity, /* isLuigi */ FALSE);
        default:
            return create_shadow_hardcoded_rectangle(xPos, yPos, zPos, shadowScale, shadowSolidity, shadowType);
    }
    return NULL;
}
