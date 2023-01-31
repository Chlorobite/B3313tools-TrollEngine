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

#define READ_MASK (INPUT_B_PRESSED)
#define SIGN_RANGE 0x38E3

u32 check_read_sign(register struct MarioState *m, register struct Object *o) {
    if ((m->input & READ_MASK) && mario_can_talk(m, 0) && object_facing_mario(m, o, SIGN_RANGE)) {
        s16 facingDYaw = (s16)(o->oMoveAngleYaw + 0x8000) - m->faceAngle[1];
        if (facingDYaw >= -SIGN_RANGE && facingDYaw <= SIGN_RANGE) {
            f32 targetX = o->oPosX + 105.0f * sins(o->oMoveAngleYaw);
            f32 targetZ = o->oPosZ + 105.0f * coss(o->oMoveAngleYaw);

            m->marioObj->oMarioReadingSignDYaw = facingDYaw;
            m->marioObj->oMarioReadingSignDPosX = targetX - m->pos[0];
            m->marioObj->oMarioReadingSignDPosZ = targetZ - m->pos[2];

            m->interactObj = o;
            m->usedObj = o;

			// ADD: personalization AI, record data about you being a nerd
			// (UNUSED: sign behavior doesn't really work in B3313; we instead use bob-omb buddies with the sign model,
			// this calls check_npc_talk, which in turn calls TRACKER_on_npc_talk in AI/stats_tracking.c,
			// which has a check for the sign model)
            TRACKER_accum_nerd += 0.25f;
			// END ADD
            return set_mario_action(m, ACT_READING_SIGN, 0);
        }
    }

    return FALSE;
}
