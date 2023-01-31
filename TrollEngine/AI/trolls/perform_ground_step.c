#include <ultra64.h>

#include "sm64.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "mario.h"
#include "audio/external.h"
#include "game_init.h"
#include "interaction.h"
#include "mario_step.h"

#include "AI/personalization_helpers.h"


s32 perform_ground_step(struct MarioState *m) {
	// new function too big for our changes; implementation in AI/personalization_helpers.c
	troll_perform_ground_step(m);
}
