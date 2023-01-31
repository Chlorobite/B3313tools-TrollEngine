#include <PR/ultratypes.h>

#include "game/area.h"
#include "actors/common1.h"
#include "audio/external.h"
#include "game/behavior_actions.h"
#include "behavior_data.h"
#include "game/camera.h"
#include "course_table.h"
#include "dialog_ids.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "game/game_init.h"
#include "game/interaction.h"
#include "game/level_update.h"
#include "game/mario.h"
#include "game/mario_step.h"
#include "game/memory.h"
#include "game/obj_behaviors.h"
#include "game/object_helpers.h"
#include "game/save_file.h"
#include "seq_ids.h"
#include "sm64.h"
#include "game/sound_init.h"
#include "game/rumble_init.h"

#include "AI/stats_tracking.h"
#include "AI/personalization_helpers.h"

#define INT_GROUND_POUND_OR_TWIRL (1 << 0) // 0x01
#define INT_PUNCH                 (1 << 1) // 0x02
#define INT_KICK                  (1 << 2) // 0x04
#define INT_TRIP                  (1 << 3) // 0x08
#define INT_SLIDE_KICK            (1 << 4) // 0x10
#define INT_FAST_ATTACK_OR_SHELL  (1 << 5) // 0x20
#define INT_HIT_FROM_ABOVE        (1 << 6) // 0x40
#define INT_HIT_FROM_BELOW        (1 << 7) // 0x80

#define INT_ATTACK_NOT_FROM_BELOW                                                 \
    (INT_GROUND_POUND_OR_TWIRL | INT_PUNCH | INT_KICK | INT_TRIP | INT_SLIDE_KICK \
     | INT_FAST_ATTACK_OR_SHELL | INT_HIT_FROM_ABOVE)

#define INT_ANY_ATTACK                                                            \
    (INT_GROUND_POUND_OR_TWIRL | INT_PUNCH | INT_KICK | INT_TRIP | INT_SLIDE_KICK \
     | INT_FAST_ATTACK_OR_SHELL | INT_HIT_FROM_ABOVE | INT_HIT_FROM_BELOW)

#define INT_ATTACK_NOT_WEAK_FROM_ABOVE                                                \
    (INT_GROUND_POUND_OR_TWIRL | INT_PUNCH | INT_KICK | INT_TRIP | INT_HIT_FROM_BELOW)

extern s16 sInvulnerable;
extern u8 sDisplayingDoorText;



u32 interact_door(register struct MarioState *m, UNUSED u32 interactType, register struct Object *o) {
    register s16 requiredNumStars = o->oBehParams >> 24;
	// EDIT: check red stars if the required number of stars is 13 every time
    register s16 numStars = requiredNumStars == 13 ? get_red_star_count(gCurrSaveFileNum - 1) : save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1);
	// END EDIT

    if (m->action == ACT_WALKING || m->action == ACT_DECELERATING) {
        if (numStars >= requiredNumStars) {
            u32 actionArg = should_push_or_pull_door(m, o);
            u32 enterDoorAction;
            u32 doorSaveFileFlag;

            if (actionArg & 0x00000001) {
                enterDoorAction = ACT_PULLING_DOOR;
            } else {
                enterDoorAction = ACT_PUSHING_DOOR;
            }

            doorSaveFileFlag = get_door_save_file_flag(o);
            m->interactObj = o;
            m->usedObj = o;

            if (o->oInteractionSubtype & INT_SUBTYPE_STAR_DOOR) {
                enterDoorAction = ACT_ENTERING_STAR_DOOR;
            }

            if (doorSaveFileFlag != 0 && !(save_file_get_flags() & doorSaveFileFlag)) {
                enterDoorAction = ACT_UNLOCKING_STAR_DOOR;
            }

            return set_mario_action(m, enterDoorAction, actionArg);
        } else if (!sDisplayingDoorText) {
            u32 text = DIALOG_022 << 16;

            switch (requiredNumStars) {
                case 1:
                    text = DIALOG_024 << 16;
                    break;
                case 3:
                    text = DIALOG_025 << 16;
                    break;
                case 8:
                    text = DIALOG_026 << 16;
                    break;
                case 30:
                    text = DIALOG_027 << 16;
                    break;
                case 50:
                    text = DIALOG_028 << 16;
                    break;
                case 13: // EDIT: 70 star doors are unused, so replace with 13
                    text = DIALOG_029 << 16;
                    break;
            }

            text += requiredNumStars - numStars;

            sDisplayingDoorText = TRUE;
            return set_mario_action(m, ACT_READING_AUTOMATIC_DIALOG, text);
        }
    }
	// REMOVE: special case where the 70 star door lets you in anyway, as they are unused

    return FALSE;
}
