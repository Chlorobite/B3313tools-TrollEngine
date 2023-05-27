#include "bhv_headers.h"



void bhv_blue_flames_group_loop(void) {
    if (o->oTimer == 0) {
        o->oMoveAngleYaw = obj_angle_to_object(o, gMarioObject);
        o->oBlueFlameNextScale = 5.0f;
    }

    if (o->oTimer < MIN(16, 230 - loadedObjectCount)) {
        if (!(o->oTimer & 1)) {
            register s32 i;

            for (i = 0; i < 3; i++) {
                register struct Object *flame = spawn_object(o, MODEL_BLUE_FLAME, bhvFlameBouncing);
                flame->oMoveAngleYaw += i * 0x5555;
                flame->header.gfx.scale[0] = o->oBlueFlameNextScale;
            }
            o->oBlueFlameNextScale -= 0.5;
        }
    } else {
        obj_mark_for_deletion(o);
    }
}
