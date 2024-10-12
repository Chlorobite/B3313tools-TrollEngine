#include <ultra64.h>

#include "sm64.h"
#include "game_init.h"
#include "memory.h"
#include "ingame_menu.h"
#include "envfx_snow.h"
#include "envfx_bubbles.h"
#include "engine/surface_collision.h"
#include "engine/math_util.h"
#include "engine/behavior_script.h"
#include "audio/external.h"
#include "obj_behaviors.h"

/**
 * Update the position of a lava bubble to be somewhere around centerPos
 * Uses find_floor to find the height of lava, if no floor or a non-lava
 * floor is found the bubble y is set to -10000, which is why you can see
 * occasional lava bubbles far below the course in Lethal Lava Land.
 * In the second Bowser fight arena, the visual lava is above the lava
 * floor so lava-bubbles are not normally visible, only if you bring the
 * camera below the lava plane.
 */
void envfx_set_lava_bubble_position(s32 index, Vec3s centerPos) {
    struct Surface *surface;
    s16 floorY;
    s16 centerX, centerY, centerZ;

    centerX = centerPos[0];
    centerY = centerPos[1];
    centerZ = centerPos[2];

    (gEnvFxBuffer + index)->xPos = random_float() * 6000.0f - 3000.0f + centerX;
    (gEnvFxBuffer + index)->zPos = random_float() * 6000.0f - 3000.0f + centerZ;

    if ((gEnvFxBuffer + index)->xPos > 8000) {
        (gEnvFxBuffer + index)->xPos = 16000 - (gEnvFxBuffer + index)->xPos;
    }
    if ((gEnvFxBuffer + index)->xPos < -8000) {
        (gEnvFxBuffer + index)->xPos = -16000 - (gEnvFxBuffer + index)->xPos;
    }

    if ((gEnvFxBuffer + index)->zPos > 8000) {
        (gEnvFxBuffer + index)->zPos = 16000 - (gEnvFxBuffer + index)->zPos;
    }
    if ((gEnvFxBuffer + index)->zPos < -8000) {
        (gEnvFxBuffer + index)->zPos = -16000 - (gEnvFxBuffer + index)->zPos;
    }

    floorY =
        find_floor((gEnvFxBuffer + index)->xPos, centerY + 500, (gEnvFxBuffer + index)->zPos, &surface);
    if (surface == NULL) {
        (gEnvFxBuffer + index)->yPos = -10000;
        return;
    }

    if (surface->type == SURFACE_BURNING) {
        (gEnvFxBuffer + index)->yPos = floorY;
    } else {
        (gEnvFxBuffer + index)->yPos = -10000;
    }
}
