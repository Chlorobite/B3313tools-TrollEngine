#include <PR/ultratypes.h>
#include <PR/gbi.h>

#include "area.h"
#include "camera.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "game/game_init.h"
#include "geo_misc.h"
#include "gfx_dimensions.h"
#include "memory.h"
#include "screen_transition.h"
#include "segment2.h"
#include "sm64.h"

extern u8 sTransitionColorFadeCount[4];
extern u16 sTransitionTextureFadeCount[2];

extern s32 render_frame_count;

s32 set_and_reset_transition_fade_timer(register s8 fadeTimer, register u8 transTime) {
    sTransitionColorFadeCount[fadeTimer] += render_frame_count; // EDIT: frameskip engine

    if (sTransitionColorFadeCount[fadeTimer] >= transTime) {
        sTransitionColorFadeCount[fadeTimer] = 0;
        sTransitionTextureFadeCount[fadeTimer] = 0;
        return TRUE;
    }
    return FALSE;
}
