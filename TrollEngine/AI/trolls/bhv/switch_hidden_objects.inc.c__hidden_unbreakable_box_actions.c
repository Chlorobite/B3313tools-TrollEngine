#include "bhv_headers.h"


void hidden_unbreakable_box_actions(void) {
    struct Object *sp1C;

    obj_set_collision_data(o, wdw_seg7_collision_07018528);

    if (o->oAction == 0) {
        cur_obj_disable_rendering();
        cur_obj_become_intangible();
        if (o->oHiddenObjectUnkF4 == NULL) {
            o->oHiddenObjectUnkF4 = cur_obj_nearest_object_with_behavior(bhvFloorSwitchHiddenObjects);
        }
        if ((sp1C = o->oHiddenObjectUnkF4) != NULL) {
            if (sp1C->oAction == 2) {
                o->oAction++;
                cur_obj_enable_rendering();
                cur_obj_unhide();
            }
        }
    } else {
        cur_obj_become_tangible();
        if (cur_obj_wait_then_blink(360, 20)) {
            o->oAction = 0;
        }
        load_object_collision_model();
    }
}

