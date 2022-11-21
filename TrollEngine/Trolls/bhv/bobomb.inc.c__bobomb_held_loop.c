#include "bhv_headers.h"


void bobomb_held_loop(void) {
    register struct Object *_o = o;
    
    _o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
    cur_obj_init_animation(1);
    cur_obj_set_pos_relative(gMarioObject, 0, 60.0f, 100.0);

    _o->oBobombFuseLit = 1;
    if (bobombType != 0 || _o->oBobombFuseTimer > 150) {
        //! Although the Bob-omb's action is set to explode when the fuse timer expires,
        //  bobomb_act_explode() will not execute until the bob-omb's held state changes.
        //  This allows the Bob-omb to be regrabbed indefinitely.
        gMarioObject->oInteractStatus |= INT_STATUS_MARIO_DROP_OBJECT;
        _o->oAction = BOBOMB_ACT_EXPLODE;
    }
}

