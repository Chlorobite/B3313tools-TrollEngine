#include <PR/ultratypes.h>

#include "area.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "course_table.h"
#include "dialog_ids.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "game_init.h"
#include "interaction.h"
#include "level_update.h"
#include "mario.h"
#include "mario_step.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "object_helpers.h"
#include "save_file.h"
#include "seq_ids.h"
#include "sm64.h"
#include "sound_init.h"
#include "rumble_init.h"

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

extern u8 gTrollWarpDestination;


extern const BehaviorScript *bhvSilverStar;
u32 interact_star_or_key(register struct MarioState *m, UNUSED u32 interactType, register struct Object *o) {
    u32 starIndex;
    register u32 starGrabAction = ACT_STAR_DANCE_EXIT;
    u32 noExit = (o->oInteractionSubtype & INT_SUBTYPE_NO_EXIT) != 0;
    u32 grandStar = (o->oInteractionSubtype & INT_SUBTYPE_GRAND_STAR) != 0;

    if (m->health >= 0x100) {
        mario_stop_riding_and_holding(m);

        if (noExit) {
            starGrabAction = ACT_STAR_DANCE_NO_EXIT;
        }
        else {
            m->hurtCounter = 0;
            m->healCounter = 0;
            if (m->capTimer > 1) {
                m->capTimer = 1;
            }
            
            if (((o->oBehParams >> 16) & 0xFF) != 13) {
				gTrollWarpDestination = 2; // star exit
			}
			
            drop_queued_background_music();
            fadeout_level_music(126);
        }

        if (m->action & ACT_FLAG_AIR) {
            starGrabAction = ACT_FALL_AFTER_STAR_GRAB;
        }

        if (m->action & ACT_FLAG_METAL_WATER) {
            starGrabAction = ACT_STAR_DANCE_EXIT;
        }

        if (m->action & ACT_FLAG_SWIMMING ||
            // green stars
            (personalization_beeparams & 0x40)) {
            starGrabAction = ACT_STAR_DANCE_WATER;
        }

        o->oInteractStatus = INT_STATUS_INTERACTED;
        m->interactObj = o;
        m->usedObj = o;

        starIndex = ((o->oBehParams >> 24) & 0xFF) + (o->oBehParams & 0x100);
        
        if (m->interactObj->behavior != segmented_to_virtual(&bhvSilverStar)) {
            save_file_collect_star_or_key(m->numCoins, starIndex);
            play_sound(SOUND_MENU_STAR_SOUND, m->marioObj->header.gfx.cameraToObject);
        }
        
        on_collected_star(o);

        m->numStars =
            save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1);

        if (grandStar) {
            return set_mario_action(m, ACT_JUMBO_STAR_CUTSCENE, 0);
        }

        return set_mario_action(m, starGrabAction, noExit + 2 * grandStar);
    }

    return FALSE;
}
