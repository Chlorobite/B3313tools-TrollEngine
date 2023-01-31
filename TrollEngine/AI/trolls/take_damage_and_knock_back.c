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


u32 take_damage_and_knock_back(register struct MarioState *m, register struct Object *o) {
    register u32 damage;
	register u32 action;

    if (!sInvulnerable && !(m->flags & MARIO_VANISH_CAP)
        && !(o->oInteractionSubtype & INT_SUBTYPE_DELAY_INVINCIBILITY)) {
        o->oInteractStatus = INT_STATUS_INTERACTED | INT_STATUS_ATTACKED_MARIO;
        m->interactObj = o;

        damage = take_damage_from_interact_object(m);

        if (o->oInteractionSubtype & INT_SUBTYPE_BIG_KNOCKBACK) {
            m->forwardVel = 40.0f;
        }

        if (o->oDamageOrCoinValue > 0) {
            play_sound(SOUND_MARIO_ATTACKED, m->marioObj->header.gfx.cameraToObject);
        }

        update_mario_sound_and_camera(m);
		// EDIT: not enough space, offload lava bob-ombs lava boost check to AI/personalization_helpers.c
		return troll_knockback(m, o, damage);
		// END EDIT
    }

    return FALSE;
}
