#include "bhv_headers.h"


void bowser_tail_anchor_act_default(void) {
    register struct Object *_o = o;
    register struct Object *bowser = _o->parentObj;
    cur_obj_become_tangible();
    cur_obj_scale(1.0f);

    _o->hitboxRadius = 100.0f / TRACKER_difficulty_modifier_sqrt_half;
    
    bowser->oIntangibleTimer = -1;
    if (bowser->oAction != BOWSER_ACT_TILT_LAVA_PLATFORM && obj_check_if_collided_with_object(_o, gMarioObject)) {
        // When Mario collides his tail, it now gets
        // intangible so he can grab it through
        bowser->oIntangibleTimer = 0;
        _o->oAction = BOWSER_ACT_TAIL_TOUCHED_MARIO;
    }
}

