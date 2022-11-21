#include "bhv_headers.h"


void bhv_door_init(void) {
    f32 x;
    f32 z;
    struct Surface *floor;

    x = o->oPosX;
    z = o->oPosZ;
    find_floor(x, o->oPosY, z, &floor);
    if (floor != NULL) {
        o->oDoorUnkF8 = floor->room;
    }

    x = o->oPosX + sins(o->oMoveAngleYaw) * 200.0f * levelScaleH;
    z = o->oPosZ + coss(o->oMoveAngleYaw) * 200.0f * levelScaleH;
    find_floor(x, o->oPosY, z, &floor);
    if (floor != NULL) {
        o->oDoorUnkFC = floor->room;
    }

    x = o->oPosX + sins(o->oMoveAngleYaw) * -200.0f * levelScaleH;
    z = o->oPosZ + coss(o->oMoveAngleYaw) * -200.0f * levelScaleH;
    find_floor(x, o->oPosY, z, &floor);
    if (floor != NULL) {
        o->oDoorUnk100 = floor->room;
    }

    if (o->oDoorUnkF8 > 0 && o->oDoorUnkF8 < 60) {
        gDoorAdjacentRooms[o->oDoorUnkF8][0] = o->oDoorUnkFC;
        gDoorAdjacentRooms[o->oDoorUnkF8][1] = o->oDoorUnk100;
    }
}

