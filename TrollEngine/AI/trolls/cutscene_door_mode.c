#include <ultra64.h>
#include "sm64.h"
#include "types.h"
#include "camera.h"

extern s16 gCutsceneTimer;
extern s16 sStatusFlags;

extern s16 s8DirModeYawOffset;
extern struct PlayerCameraState *sMarioCamState;

void process_troll_signal(struct Camera *c);

/**
 * Used for entering a room that uses a specific camera mode, like the castle lobby or BBH
 */
BAD_RETURN(s32) cutscene_door_mode(struct Camera *c) {
    reset_pan_distance(c);
    process_troll_signal(c);
    camera_course_processing(c);

    if (c->mode == CAMERA_MODE_FIXED) {
        c->nextYaw = update_fixed_camera(c, c->focus, c->pos);
    }
    if (c->mode == CAMERA_MODE_PARALLEL_TRACKING) {
        c->nextYaw = update_parallel_tracking_camera(c, c->focus, c->pos);
    }

    c->yaw = c->nextYaw;

    // Loop until Mario is no longer using the door
    if (sMarioCamState->action != ACT_ENTERING_STAR_DOOR &&
        sMarioCamState->action != ACT_PULLING_DOOR &&
        sMarioCamState->action != ACT_PUSHING_DOOR) {
        gCutsceneTimer = CUTSCENE_STOP;
        c->cutscene = 0;
    }
}
