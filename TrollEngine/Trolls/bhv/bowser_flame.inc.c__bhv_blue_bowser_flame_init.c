#include "bhv_headers.h"



void bhv_blue_bowser_flame_init(void) {
    register struct Object *_o = o;

    if (loadedObjectCount > 180) {
        if (loadedObjectCount > 220) {
            return;
        }

        if (gGlobalTimer & 1) {
            return;
        }
    }

    obj_translate_xz_random(_o, 80.0f);
    _o->oAnimState = (s32)(random_float() * 10.0f);
    _o->oVelY = 7.0f;
    _o->oForwardVel = 35.0f;
    _o->oFlameScale = 3.0f;
    _o->oFlameUnusedRand = random_float() * 0.5;
    _o->oGravity = 1.0f;
    _o->oFlameSpeedTimerOffset = (s32)(random_float() * 64.0f);
}
