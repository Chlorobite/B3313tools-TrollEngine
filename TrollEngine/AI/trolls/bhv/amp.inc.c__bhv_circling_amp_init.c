#include "bhv_headers.h"


void bhv_circling_amp_init(void) {
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
    o->oAnimState = 1;

    // Determine the radius of the circling amp's circle
    switch (o->oBehParams2ndByte) {
        case AMP_BP_ROT_RADIUS_200:
            o->oAmpRadiusOfRotation = 200.0f;
            break;

        case AMP_BP_ROT_RADIUS_300:
            o->oAmpRadiusOfRotation = 300.0f;
            break;

        case AMP_BP_ROT_RADIUS_400:
            o->oAmpRadiusOfRotation = 400.0f;
            break;
    }
    o->oAmpRadiusOfRotation *= levelScaleH;

    // Choose a random point along the amp's circle.
    // The amp's move angle represents its angle along the circle.
    o->oMoveAngleYaw = random_u16();

    o->oAction = AMP_ACT_IDLE;
}

