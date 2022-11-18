#include "bhv_headers.h"


void bobomb_act_explode(void) {
    if (o->oTimer < 5) {
        cur_obj_scale(1.0f + (f32) o->oTimer / 5.0f);
    } else {
        struct Object *explosion = spawn_object(o, MODEL_EXPLOSION, bhvExplosion);
        explosion->oPosY += 100.0f;

        bobomb_spawn_coin();
        create_respawner(MODEL_BLACK_BOBOMB, bhvBobomb, 3000);

        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }
}

