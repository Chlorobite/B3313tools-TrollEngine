#include "bhv_headers.h"

extern struct ObjectHitbox sBobombHitbox;

void bobomb_check_interactions(void) {
    register struct Object *_o = o;
    
    obj_set_hitbox(_o, &sBobombHitbox);

    if (_o->oInteractStatus & INT_STATUS_INTERACTED) {
        if (_o->oInteractStatus & INT_STATUS_MARIO_KNOCKBACK_DMG) {
            _o->oMoveAngleYaw = gMarioObject->header.gfx.angle[1];
            _o->oForwardVel = 25.0f;
            _o->oVelY = 30.0f;
            _o->oAction = BOBOMB_ACT_LAUNCHED;
        }

        if (_o->oInteractStatus & INT_STATUS_TOUCHED_BOB_OMB) {
            _o->oAction = BOBOMB_ACT_EXPLODE;
        }

        _o->oInteractStatus = 0;
    }

    if (obj_attack_collided_from_other_object(o)) {
        _o->oAction = BOBOMB_ACT_EXPLODE;
    }
    
    if (bobombType != 0 && is_point_within_radius_of_mario(_o->oPosX, _o->oPosY, _o->oPosZ, 100.0f)) {
        _o->oAction = BOBOMB_ACT_EXPLODE;
    }
}

