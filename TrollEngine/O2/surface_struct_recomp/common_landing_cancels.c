#include "mario_actions_moving_headers.h"

s32 common_landing_cancels(struct MarioState *m, struct LandingAction *landingAction,
                           s32 (*setAPressAction)(struct MarioState *, u32, u32)) {
    //! Everything here, including floor steepness, is checked before checking
    // if Mario is actually on the floor. This leads to e.g. remote sliding.

    if (m->floor->normal.y < 0.2923717f) {
        return mario_push_off_steep_floor(m, landingAction->verySteepAction, 0);
    }

    m->doubleJumpTimer = landingAction->unk02;

    if (should_begin_sliding(m)) {
        return set_mario_action(m, landingAction->slideAction, 0);
    }

    if (m->input & INPUT_FIRST_PERSON) {
        return set_mario_action(m, landingAction->endAction, 0);
    }

    if (++m->actionTimer >= landingAction->numFrames) {
        return set_mario_action(m, landingAction->endAction, 0);
    }

    if (m->input & INPUT_A_PRESSED) {
        return setAPressAction(m, landingAction->aPressedAction, 0);
    }

    if (m->input & INPUT_OFF_FLOOR) {
        return set_mario_action(m, landingAction->offFloorAction, 0);
    }

    return FALSE;
}
