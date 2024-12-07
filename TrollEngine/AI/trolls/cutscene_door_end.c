#include <ultra64.h>
#include "types.h"
#include "camera.h"

extern s16 gCutsceneTimer;
extern s16 sStatusFlags;

extern s16 s8DirModeYawOffset;

s32 cutscene_door_end(struct Camera *c) {
    if (c->mode == CAMERA_MODE_FIXED) { // lobby foyer not in a room
        c->mode = CAMERA_MODE_CLOSE;
    }
    else if (c->mode == CAMERA_MODE_8_DIRECTIONS) { // so view isnt at other side of the door lol
        s8DirModeYawOffset += DEGREES(180);
    }

    c->cutscene = 0;
    gCutsceneTimer = CUTSCENE_STOP;
    sStatusFlags |= CAM_FLAG_SMOOTH_MOVEMENT;
    sStatusFlags &= ~CAM_FLAG_BLOCK_SMOOTH_MOVEMENT;
    set_flag_post_door(c);
    update_camera_yaw(c);
}
