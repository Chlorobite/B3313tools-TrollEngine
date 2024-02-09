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


Gfx *painting_model_view_transform(struct Painting *painting) {
    f32 sizeRatio = painting->size / PAINTING_SIZE;
    Mtx *rotX = alloc_display_list(4 * sizeof(Mtx));
    Mtx *rotY = rotX + 1;
    Mtx *translate = rotX + 2;
    Mtx *scale = rotX + 3;
    Gfx *dlist = alloc_display_list(5 * sizeof(Gfx));
    Gfx *gfx = dlist;

    if (rotX == NULL || dlist == NULL) {
    }

    guTranslate(translate, painting->posX, painting->posY, painting->posZ);
    guRotate(rotX, painting->pitch, 1.0f, 0.0f, 0.0f);
    guRotate(rotY, painting->yaw, 0.0f, 1.0f, 0.0f);
    guScale(scale, sizeRatio, sizeRatio, sizeRatio);

    gSPMatrix(gfx++, translate, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(gfx++, rotX,      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(gfx++, rotY,      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(gfx++, scale,     G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    gSPEndDisplayList(gfx);

    return dlist;
}
