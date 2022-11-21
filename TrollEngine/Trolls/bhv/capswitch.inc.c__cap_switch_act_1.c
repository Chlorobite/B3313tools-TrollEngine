#include "bhv_headers.h"

extern s32 sCapSaveFlags[];

void cap_switch_act_1(void) {
    if (cur_obj_is_mario_on_platform()) {
        troll_yellow_switch_or_set_flags(o->oBehParams2ndByte);
        o->oAction = 2;
        cur_obj_play_sound_2(SOUND_GENERAL_ACTIVATE_CAP_SWITCH);
    }
}

