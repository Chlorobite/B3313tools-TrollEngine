#include <PR/ultratypes.h>

#include "sm64.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "game_init.h"
#include "geo_misc.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_table.h"
#include "level_update.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "obj_behaviors_2.h"
#include "object_constants.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "platform_displacement.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "seq_ids.h"
#include "spawn_sound.h"

#define o gCurrentObject

enum oGearActions {
    ACT_GEAR_SPIN,
    ACT_GEAR_STOP
};

void gear_spin(void) {
    o->oAngleVelYaw = 0x200;

    if (o->oTimer > 15) {
        o->oAction = ACT_GEAR_STOP;
    }
}

void gear_stop(void) {
    o->oAngleVelYaw = 0;

    if (o->oTimer > 45) {
        o->oAction = ACT_GEAR_SPIN;
    }
}

void gear_main(void) {
    switch (o->oAction) {
        case ACT_GEAR_SPIN:
        gear_spin();
        break;
        case ACT_GEAR_STOP:
        gear_stop();
        break;
    }

    if (o->oTimer > 45) {
        cur_obj_play_sound_2(SOUND_GENERAL_BIG_CLOCK);
    }
}

void bhv_gear_loop(void) {
    gear_main();
    o->oFaceAnglePitch += o->oAngleVelPitch;
    o->oFaceAngleYaw += o->oAngleVelYaw;
    o->oFaceAngleRoll += o->oAngleVelRoll;
}
