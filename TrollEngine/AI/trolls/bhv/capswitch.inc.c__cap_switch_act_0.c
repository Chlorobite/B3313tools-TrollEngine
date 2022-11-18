#include "bhv_headers.h"

extern s32 sCapSaveFlags[];

void cap_switch_act_0(void) {
    o->oAnimState = o->oBehParams2ndByte;
    cur_obj_scale(0.5f * levelScaleH);
    o->oPosY += 71.0f;

    spawn_object_relative_with_scale(0, 0, -71, 0, 0.5f, o, MODEL_CAP_SWITCH_BASE, bhvCapSwitchBase);

    o->oAction = 1;
    if (o->oBehParams2ndByte < 3) {
        if (save_file_get_flags() & sCapSaveFlags[o->oBehParams2ndByte]) {
            o->oAction = 3;
            o->header.gfx.scale[1] = 0.1f;
        }
    }
}

