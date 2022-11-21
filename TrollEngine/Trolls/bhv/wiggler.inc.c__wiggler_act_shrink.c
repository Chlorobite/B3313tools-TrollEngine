#include "bhv_headers.h"


void wiggler_act_shrink(void) {
    register struct Object *_o = o;
    
    if (_o->oTimer >= 20) {
        // 4 is the default scale, so shrink to 1/4 of regular size
        if (approach_f32_ptr(&_o->header.gfx.scale[0], 1.0f, 0.1f)) {
            spawn_boss_star((void*)0x80404F70);
            _o->oAction = WIGGLER_ACT_FALL_THROUGH_FLOOR;
        }

        cur_obj_scale(_o->header.gfx.scale[0]);
    }
}

