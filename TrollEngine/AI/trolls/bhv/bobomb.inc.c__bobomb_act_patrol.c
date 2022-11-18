#include "bhv_headers.h"

extern struct Surface *sObjFloor;

void bobomb_act_patrol(void) {
    register s16 collisionFlags;
    register struct Object *_o = o;

    _o->oForwardVel = 5.0f;
    collisionFlags = object_step();

    if ((obj_return_home_if_safe(o, _o->oHomeX, _o->oHomeY, _o->oHomeZ, 400) == TRUE)
        && (bobombType != 0 || obj_check_if_facing_toward_angle(_o->oMoveAngleYaw, _o->oAngleToMario, 0x2000) == TRUE)) {
        _o->oBobombFuseLit = 1;
        _o->oAction = BOBOMB_ACT_CHASE_MARIO;
    }

    obj_check_floor_death(collisionFlags, sObjFloor);
}

