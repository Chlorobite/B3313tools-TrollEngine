#include "bhv_headers.h"

extern s16 sElevatorHeights[];

void bhv_elevator_init(void) {
    s32 sp1C = sElevatorHeights[o->oBehParams2ndByte * 3 + 2];
    if (sp1C == 0) {
        o->oElevatorUnkF4 = sElevatorHeights[o->oBehParams2ndByte * 3] * levelScaleV;
        o->oElevatorUnkF8 = o->oHomeY;
        o->oElevatorUnkFC = (o->oElevatorUnkF4 + o->oElevatorUnkF8) / 2;
        o->oElevatorUnk100 = cur_obj_has_behavior(bhvRrElevatorPlatform);
    } else {
        o->oElevatorUnkF4 = sElevatorHeights[o->oBehParams2ndByte * 3] * levelScaleV;
        o->oElevatorUnkF8 = sElevatorHeights[o->oBehParams2ndByte * 3 + 1] * levelScaleV;
        o->oElevatorUnkFC = (o->oElevatorUnkF4 + o->oElevatorUnkF8) / 2;
        o->oElevatorUnk100 = 2;
    }
}

