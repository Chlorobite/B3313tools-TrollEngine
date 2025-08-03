#include "bhv_headers.h"

void tumbling_bridge_act_2(void) {
    cur_obj_hide();
    //if (cur_obj_has_behavior(bhvLllTumblingBridge)) {
    if (o->oBehParams2ndByte == 2) {
        cur_obj_unhide();
    } else if (o->oDistanceToMario > 1200.0f) {
        o->oAction = 3;
        cur_obj_unhide();
    }
}
