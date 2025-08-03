#include "bhv_headers.h"

void tumbling_bridge_act_0(void) {
    if (o->oBehParams2ndByte == 2 || o->oDistanceToMario < 1000.0f) {
        o->oAction = 1;
    }
}
