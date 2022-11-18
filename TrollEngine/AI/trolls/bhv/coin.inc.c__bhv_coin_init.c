#include "bhv_headers.h"

extern struct ObjectHitbox sYellowCoinHitbox;

void bhv_coin_init(void) {
    o->oVelY = random_float() * 10.0f + 30 + o->oCoinUnk110;
    o->oForwardVel = random_float() * 10.0f;
    o->oMoveAngleYaw = random_u16();

    cur_obj_set_behavior(bhvYellowCoin);
    obj_set_hitbox(o, &sYellowCoinHitbox);
    cur_obj_become_intangible();
    
    personalize_coin(o, 2);
}

