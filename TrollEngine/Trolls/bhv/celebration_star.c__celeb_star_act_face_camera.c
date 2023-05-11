#include "bhv_headers.h"

void celeb_star_act_face_camera(void) {
    if (o->oTimer < 10) {
        cur_obj_scale((f32) o->oTimer / 10.0f);
        o->oAnimState++;
    } else {
        o->oAnimState = 0;
    }

    if (o->oTimer == 59) {
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }
}
