#include "../mario_headers.h"


s32 common_death_handler(register struct MarioState *m, s32 animation, s32 frameToDeathWarp) {
    s32 animFrame = set_mario_animation(m, animation);
    if (animFrame >= frameToDeathWarp) {
        level_trigger_warp(m, WARP_OP_DEATH);
    }
    m->marioBodyState->eyeState = MARIO_EYES_DEAD;
    stop_and_set_height_to_floor(m);
    return animFrame;
}
