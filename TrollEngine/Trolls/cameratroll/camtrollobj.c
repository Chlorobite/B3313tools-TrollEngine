#include <ultra64.h>
#include "types.h"

#include "Trolls/bhv/bhv_headers.h"
#include "camera.h"

extern Vec3f sFixedModeBasePosition;
extern u32 gCurrLevelArea;
extern s16 sStatusFlags;

s32 camera_troll_signal = 0;

s32 camtrollobj_in_mode_bounds(register Vec3f p) {
    if (gCurrLevelNum == LEVEL_CASTLE) {
        // vanilla lobby
        // the lobby, including funny hole
        if (p[0] > 1000.f || p[0] < -3050.f || p[1] < -450.f || p[2] < -1600.f) return 0;
        // the star door
        if (-p[0] - p[2] > 4050.f) return 0;
        // the pspspspsps door
        if (p[0] - p[2] > 2050.f) return 0;
        // cutout for wf and ccm doors
        if (p[1] < 300.f && p[2] < -1000.f) return 0;
        // cutout for foyer doors
        if (p[1] < 300.f && (p[0] > -2000.f && p[0] < -1001.f) && (-p[0] + p[2]) < 1000.f) return 0;
        if (p[1] < 300.f && (p[0] > -999.f && p[0] < 0.f) && (p[0] + p[2]) < -1000.f) return 0;
        // cutouts for foyer staircase (at least the part where your y can be >-450)
        if (p[1] < -100.f && (p[0] > -1300.f && p[0] < -750.f) && p[2] < 400.f) return 0;
        if (p[1] < 200.f && (p[0] > -1300.f && p[0] < -750.f) && p[2] < -100.f) return 0;
    }
    else {
        // beta lobby
        // the lobby yknow
        if (absf(p[0]) > 1672.f || p[2] < -650.f) return 0;
        // cutout for the foyer part that sticks into the lobby
        if (absf(p[0]) < 315.f && p[1] < 300.f && p[2] < -480.f) return 0;
    }

    return 1;
}

s32 camtrollobj_in_door_signal_bounds(register Vec3f p) {
    if (gCurrLevelNum == LEVEL_CASTLE) {
        // the lobby, including funny hole
        if (p[0] > 1200.f || p[0] < -3250.f || p[1] < -450.f || p[2] < -1600.f) return 0;
    }
    else {
        // beta lobby
        // the lobby yknow
        if (absf(p[0]) > 1850.f || p[2] < -850.f) return 0;
    }

    return 1;
}
s32 camtrollobj_in_untroll_bounds(register Vec3f p) {
    if (gCurrLevelNum == LEVEL_CASTLE) {
        // section behind pech
        if (p[1] > 850.f && p[2] > 1550.f) return 1;
    }

    return 0;
}

void bhv_camtrollobj_insidecastle() {
    Vec3f p;
    vec3f_copy(p, gMarioState->pos);

    if (camtrollobj_in_untroll_bounds(p)) {
        if (gLakituState.mode == CAMERA_MODE_FIXED) gLakituState.mode = CAMERA_MODE_CLOSE;
        return;
    }

    if (!camtrollobj_in_door_signal_bounds(p)) return;
    camera_troll_signal = 1;
    if (!camtrollobj_in_mode_bounds(p)) return;

    vec3f_copy(sFixedModeBasePosition, &o->oPosX);
    if (gLakituState.mode == CAMERA_MODE_CLOSE) gLakituState.mode = CAMERA_MODE_FIXED;
    camera_troll_signal = 2;
}

void process_troll_signal(struct Camera *c) {
    if (camera_troll_signal == 2) {
        if (c->mode != CAMERA_MODE_FIXED) {
            sStatusFlags &= ~CAM_FLAG_SMOOTH_MOVEMENT;
            vec3f_set(sFixedModeBasePosition, 660.f, 160.f, 800.f);
            c->mode = CAMERA_MODE_FIXED;
        }
        camera_troll_signal = 3; // just to double check this is in fact being run
    }
}
