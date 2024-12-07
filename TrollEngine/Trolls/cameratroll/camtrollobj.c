#include <ultra64.h>
#include "types.h"

#include "Trolls/bhv/bhv_headers.h"
#include "camera.h"

extern Vec3f sFixedModeBasePosition;
extern u32 gCurrLevelArea;

void bhv_camtrollobj_insidecastle() {
    Vec3f p;
    vec3f_copy(p, gMarioState->pos);

    vec3f_copy(sFixedModeBasePosition, &o->oPosX);

    if (gCurrLevelNum == LEVEL_CASTLE) {
        // top 5 programing moments 1995 numero 1
        if (!
            (o->oTimer < 2 && p[2] > 300.f ||
            (p[0] > -3040.f && p[0] < -1970.f ||
             p[0] >  -100.f && p[0] <   995.f) &&
             p[2] > ((p[1] < 400.f) ? -1000.f : -1400.f))) return;
    }
    else {
        if (!(absf(p[0]) < 1672.f && p[2] > -480.f)) return;
    }

    if (gLakituState.mode == CAMERA_MODE_CLOSE) gLakituState.mode = CAMERA_MODE_FIXED;
}
