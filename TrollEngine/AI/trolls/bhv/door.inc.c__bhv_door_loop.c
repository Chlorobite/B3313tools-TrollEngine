#include "bhv_headers.h"

struct DoorAction {
    u32 flag;
    s32 action;
};

extern struct DoorAction sDoorActions[];

void bhv_door_loop(void) {
    register s32 sp1C = 0;

    while (sDoorActions[sp1C].flag != 0xFFFFFFFF) {
        if (cur_obj_clear_interact_status_flag(sDoorActions[sp1C].flag)) {
            set_door_camera_event();
            cur_obj_change_action(sDoorActions[sp1C].action);
        }
        sp1C++;
    }
    o->oInteractStatus = 0;

    switch (o->oAction) {
        case 0:
            cur_obj_init_animation_with_sound(0);
            break;
        case 1:
        case 2:
            door_animation_and_reset(o->oAction);
            play_door_open_noise();
            break;
        case 3:
        case 4:
            door_animation_and_reset(o->oAction);
            play_warp_door_open_noise();
            break;
    }

    if (o->oAction == 0) {
        load_object_collision_model();
    }

    bhv_star_door_loop_2();
}
