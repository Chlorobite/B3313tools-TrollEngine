#include "mario_actions_stationary_headers.h"

s32 act_first_person(struct MarioState *m) {
    s32 sp1C = (m->input & (INPUT_OFF_FLOOR | INPUT_ABOVE_SLIDE | INPUT_STOMPED)) != 0;

    if (m->actionState == 0) {
        lower_background_noise(2);
        set_camera_mode(m->area->camera, CAMERA_MODE_C_UP, 0x10);
        m->actionState = 1;
    } else if (!(m->input & INPUT_FIRST_PERSON) || sp1C) {
        raise_background_noise(2);
        // Go back to the last camera mode
        set_camera_mode(m->area->camera, -1, 1);
        return set_mario_action(m, ACT_IDLE, 0);
    }

    if (m->floor->type == SURFACE_LOOK_UP_WARP
        && save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1) >= 10) {
        s16 sp1A = m->statusForCamera->headRotation[0];
        s16 sp18 = ((m->statusForCamera->headRotation[1] * 4) / 3) + m->faceAngle[1];
        if (sp1A == -0x1800 && (sp18 < -0x6FFF || sp18 >= 0x7000)) {
            level_trigger_warp(m, WARP_OP_UNKNOWN_01);
        }
    }

    stationary_ground_step(m);
    set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);
    return FALSE;
}
