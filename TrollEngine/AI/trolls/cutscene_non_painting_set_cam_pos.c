#include <ultra64.h>
#include "sm64.h"
#include "types.h"
#include "camera.h"
#include "engine/surface_collision.h"

extern s16 gCutsceneTimer;
extern s16 sStatusFlags;

extern s16 s8DirModeYawOffset;
extern struct PlayerCameraState *sMarioCamState;

extern struct CutsceneVariable sCutsceneVars[10];

void offset_rotated(Vec3f dst, Vec3f from, Vec3f to, Vec3s rotation);

/**
 * Set the camera pos depending on which level Mario exited,
 * except for the part where there's hardcoded positions by level
 */
BAD_RETURN(s32) cutscene_non_painting_set_cam_pos(struct Camera *c) {
    struct Surface *floor;

    offset_rotated(c->pos, sCutsceneVars[7].point, sCutsceneVars[5].point, sCutsceneVars[7].angle);
    c->pos[1] = find_floor(c->pos[0], c->pos[1] + 1000.f, c->pos[2], &floor) + 125.f;
}
