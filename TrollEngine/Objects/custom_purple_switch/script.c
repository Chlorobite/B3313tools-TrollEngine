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

/* Purple Switch */
/* oBehParams2ndByte */
#define PURPLE_SWITCH_BP_NO_TICK                        0x0
#define PURPLE_SWITCH_BP_ANIMATES                       0x1
#define PURPLE_SWITCH_BP_REVEAL_HIDDEN                  0x2
/* oAction */
#define PURPLE_SWITCH_ACT_IDLE                          0x0
#define PURPLE_SWITCH_ACT_PRESSED                       0x1
#define PURPLE_SWITCH_ACT_TICKING                       0x2
#define PURPLE_SWITCH_ACT_UNPRESSED                     0x3
#define PURPLE_SWITCH_ACT_WAIT_FOR_MARIO_TO_GET_OFF     0x4

#define SCALE_AXIS_X 0x01
#define SCALE_AXIS_Y 0x02
#define SCALE_AXIS_Z 0x04
#define MARIO_NO_PURPLE_SWITCH /* 0x00002000 */ (1 << 13)

void bhv_custom_purple_switch_loop(void) {
    switch (o->oAction) {
        /**
         * Set the switch's model and scale. If Mario is standing near the
         * switch's middle section, transition to the pressed state.
         */
        case PURPLE_SWITCH_ACT_IDLE:
            cur_obj_set_model(MODEL_PURPLE_SWITCH);
            cur_obj_scale(1.5f);
            if (
                gMarioObject->platform == o
                && !(gMarioStates[0].action & MARIO_NO_PURPLE_SWITCH)
                && lateral_dist_between_objects(o, gMarioObject) < 127.5f
            ) {
                o->oAction = PURPLE_SWITCH_ACT_PRESSED;
            }
            break;

        /**
         * Collapse the switch downward, play a sound, and shake the screen.
         * Immediately transition to the ticking state.
         */
        case PURPLE_SWITCH_ACT_PRESSED:
            cur_obj_scale_over_time(SCALE_AXIS_Y, 3, 1.5f, 0.2f);
            if (o->oTimer == 3) {
                cur_obj_play_sound_2(SOUND_GENERAL2_PURPLE_SWITCH);
                o->oAction = PURPLE_SWITCH_ACT_TICKING;
                cur_obj_shake_screen(SHAKE_POS_SMALL);
            }
            break;

        /**
         * Play a continuous ticking sound that gets faster when time is almost
         * up. When time is up, move to a waiting-while-pressed state.
         */
        case PURPLE_SWITCH_ACT_TICKING: {
            struct Object *badShip = cur_obj_nearest_object_with_behavior((BehaviorScript*)0x130063D0);

            if (badShip) {
                struct Object *goodShip = spawn_object(o, 0x8, (BehaviorScript*)0x130063D0);

                mark_obj_for_deletion(badShip);

                goodShip->oPosX = 4812.0f;
                goodShip->oPosY = -1290.0f;
                goodShip->oPosZ = 1755.0f;
            }
            break;
        }
            // bhv to delete: 0x130063D0
            // bhv: 0x13007A70
            // model: 0x8
            // x: 4812
            // y: -1770
            // z: 1755



            // if (o->oBehParams2ndByte != 0) {
            //     if (o->oBehParams2ndByte == 1 && gMarioObject->platform != o) {
            //         o->oAction++;
            //     } else {
            //         if (o->oTimer < 360) {
            //             play_sound(SOUND_GENERAL2_SWITCH_TICK_FAST, gGlobalSoundSource);
            //         } else {
            //             play_sound(SOUND_GENERAL2_SWITCH_TICK_SLOW, gGlobalSoundSource);
            //         }
            //         if (o->oTimer > 400) {
            //             o->oAction = PURPLE_SWITCH_ACT_WAIT_FOR_MARIO_TO_GET_OFF;
            //         }
            //     }
            // }
        /**
         * Make the switch look unpressed again, and transition back to the
         * idle state.
         */
        case PURPLE_SWITCH_ACT_UNPRESSED:
            cur_obj_scale_over_time(SCALE_AXIS_Y, 3, 0.2f, 1.5f);
            if (o->oTimer == 3) {
                o->oAction = PURPLE_SWITCH_ACT_IDLE;
            }
            break;

        /**
         * Mario is standing on the switch, but time has expired. Wait for
         * him to get off the switch, and when he does so, transition to the
         * unpressed state.
         */
        case PURPLE_SWITCH_ACT_WAIT_FOR_MARIO_TO_GET_OFF:
            if (!cur_obj_is_mario_on_platform()) {
                o->oAction = PURPLE_SWITCH_ACT_UNPRESSED;
            }
            break;
    }
}
