#include "bhv_headers.h"

void bhv_celebration_star_init(void) {
    o->oHomeX = gMarioObject->header.gfx.pos[0];
    o->oPosY = gMarioObject->header.gfx.pos[1] + 30.0f;
    o->oHomeZ = gMarioObject->header.gfx.pos[2];
    o->oMoveAngleYaw = gMarioObject->header.gfx.angle[1] + 0x8000;
    o->oCelebStarDiameterOfRotation = 100;

    o->header.gfx.node.flags |= GRAPH_RENDER_BILLBOARD; // BILLBOARD()

    cur_obj_scale(0.4f);
    o->oFaceAnglePitch = 0;
    o->oFaceAngleRoll = 0;
}
