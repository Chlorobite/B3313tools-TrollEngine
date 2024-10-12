#include "camera_headers.h"

/**
 * Plays when mario enters a painting. The camera flies up to the painting's center, then it slowly
 * zooms in until the star select screen appears.
 */
BAD_RETURN(s32) cutscene_enter_painting(struct Camera *c) {
    // stolen from tpp, just to make absolutely sure we aren't accessing a surface incorrectly
    sStatusFlags |= CAM_FLAG_SMOOTH_MOVEMENT;
    c->mode = CAMERA_MODE_CLOSE;
}
