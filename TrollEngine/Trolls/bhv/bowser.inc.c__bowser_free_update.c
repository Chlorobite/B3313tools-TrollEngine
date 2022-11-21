#include "bhv_headers.h"

extern void (*sBowserActions[])(void);
extern struct SoundState sBowserSoundStates[];
extern s8 sBowserRainbowLight[];
extern s8 sBowserHealth[];

void bowser_free_update(void) {
    struct Surface *floor;
    register struct Object *platform;
    register struct Object *_o = o;

    // Platform displacement check (for BitFS)
    if ((platform = _o->platform) != NULL) {
        apply_platform_displacement(FALSE, platform);
    }
    // Reset grabbed status
    _o->oBowserGrabbedStatus = BOWSER_GRAB_STATUS_NONE;
    // Update positions and actions (default action)
    cur_obj_update_floor_and_walls();
    cur_obj_call_action_function(sBowserActions);
    cur_obj_move_standard(-78);
    // Jump on stage if Bowser has fallen off
    if (bowser_check_fallen_off_stage()) {
        TRACKER_boss_performance -= 1.0f; // epic throw fail!!
        _o->oAction = BOWSER_ACT_JUMP_ONTO_STAGE;
    }
    // Check floor height and platform
    find_floor(_o->oPosX, _o->oPosY, _o->oPosZ, &floor);
    if ((floor != NULL) && (floor->object != NULL)) {
        _o->platform = floor->object;
    } else {
        _o->platform = NULL;
    }
    // Sound states for Bowser Animations
    exec_anim_sound_state(sBowserSoundStates);
}

