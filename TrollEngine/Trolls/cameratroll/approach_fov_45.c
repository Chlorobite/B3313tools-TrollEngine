#include <ultra64.h>
#include "types.h"
#include "area.h"
#include "engine/math_util.h"

extern struct CameraFOVStatus sFOVState;

void approach_fov_45(void) {
    register f32 targetFoV;

    if (gCamera->mode == CAMERA_MODE_FIXED && gCamera->cutscene == 0) {
        targetFoV = gCurrLevelNum == LEVEL_CASTLE_GROUNDS ? 60.f : 45.f;
    } else {
        targetFoV = 45.f;
    }

    sFOVState.fov = approach_f32(sFOVState.fov, targetFoV, 2.f, 2.f);
}
