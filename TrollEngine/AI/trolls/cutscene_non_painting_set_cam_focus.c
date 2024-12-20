#include <ultra64.h>
#include "sm64.h"
#include "types.h"
#include "camera.h"

extern s16 gCutsceneTimer;
extern s16 sStatusFlags;

extern s16 s8DirModeYawOffset;
extern struct PlayerCameraState *sMarioCamState;

extern struct CutsceneVariable sCutsceneVars[10];

void offset_rotated(Vec3f dst, Vec3f from, Vec3f to, Vec3s rotation);

/**
 * Update the camera focus depending on which level Mario exited,
 * except for the part where there's hardcoded positions by level
 */
BAD_RETURN(s32) cutscene_non_painting_set_cam_focus(struct Camera *c) {
    offset_rotated(c->focus, sCutsceneVars[7].point, sCutsceneVars[6].point, sCutsceneVars[7].angle);

    c->focus[1] = c->pos[1] + (sMarioCamState->pos[1] - c->pos[1]) * 0.2f;
}
