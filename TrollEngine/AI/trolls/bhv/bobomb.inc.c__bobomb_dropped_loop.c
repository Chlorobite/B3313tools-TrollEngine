#include "bhv_headers.h"


void bobomb_dropped_loop(void) {
    cur_obj_get_dropped();

    o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
    cur_obj_init_animation(0);

    o->oHeldState = 0;
    //o->oAction = BOBOMB_ACT_PATROL;
}

