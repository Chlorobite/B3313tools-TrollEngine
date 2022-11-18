#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "engine/graph_node.h"
#include "engine/surface_collision.h"
#include "game_init.h"
#include "geo_misc.h"
#include "mario.h"
#include "memory.h"
#include "moving_texture.h"
#include "object_list_processor.h"
#include "paintings.h"
#include "save_file.h"

extern s16 gPaintingUpdateCounter;
extern s16 gLastPaintingUpdateCounter;

Gfx *reel_geo_painting_update(s32 callContext, UNUSED struct GraphNode *node, UNUSED Mat4 c) {
    struct Surface *surface;

    // Reset the update counter
    if (callContext != GEO_CONTEXT_RENDER) {
        gLastPaintingUpdateCounter = gAreaUpdateCounter - 1;
        gPaintingUpdateCounter = gAreaUpdateCounter;
    } else {
        gLastPaintingUpdateCounter = gPaintingUpdateCounter;

        if (gPaintingUpdateCounter != gAreaUpdateCounter) {
            gPaintingUpdateCounter = gAreaUpdateCounter;
            // Store Mario's floor and position
            find_floor(gMarioObject->oPosX, gMarioObject->oPosY, gMarioObject->oPosZ, &surface);
            if (surface != NULL) {
                gPaintingMarioFloorType = surface->type;
            }
            gPaintingMarioXPos = gMarioObject->oPosX;
            gPaintingMarioYPos = gMarioObject->oPosY;
            gPaintingMarioZPos = gMarioObject->oPosZ;
        }
    }
    return NULL;
}
