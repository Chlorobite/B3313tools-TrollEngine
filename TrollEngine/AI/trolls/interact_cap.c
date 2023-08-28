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


u32 interact_cap(struct MarioState *m, UNUSED u32 interactType, struct Object *o) {
    u32 capFlag = get_mario_cap_flag(o);
    u16 capMusic = 0;
    u16 capTime = 0;

    if (m->action != ACT_GETTING_BLOWN && capFlag != 0) {
        m->interactObj = o;
        o->oInteractStatus = INT_STATUS_INTERACTED;

        m->flags &= ~MARIO_CAP_ON_HEAD & ~MARIO_CAP_IN_HAND;
        m->flags |= capFlag;

        switch (capFlag) {
            case MARIO_VANISH_CAP:
                capTime = 600;
                capMusic = SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP);

                if (random_u16() < get_red_star_count(gCurrSaveFileNum - 1)) {
                    m->marioObj->activeFlags &= ~ACTIVE_FLAG_ACTIVE;
                }
                break;

            case MARIO_METAL_CAP:
                capTime = 600;
                capMusic = SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP);
                break;

            case MARIO_WING_CAP:
                capTime = 1800;
                capMusic = SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP);
                break;
        }

        if (capTime > m->capTimer) {
            m->capTimer = capTime;
        }

        if ((m->action & ACT_FLAG_IDLE) || m->action == ACT_WALKING) {
            m->flags |= MARIO_CAP_IN_HAND;
            set_mario_action(m, ACT_PUTTING_ON_CAP, 0);
        } else {
            m->flags |= MARIO_CAP_ON_HEAD;
        }

        play_sound(SOUND_MENU_STAR_SOUND, m->marioObj->header.gfx.cameraToObject);
        play_sound(SOUND_MARIO_HERE_WE_GO, m->marioObj->header.gfx.cameraToObject);

        if (capMusic != 0) {
            play_cap_music(capMusic);
        }

        return TRUE;
    }

    return FALSE;
}
