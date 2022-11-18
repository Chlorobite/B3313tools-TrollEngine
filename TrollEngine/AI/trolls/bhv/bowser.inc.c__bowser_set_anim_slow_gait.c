#include "bhv_headers.h"


s32 bowser_set_anim_slow_gait(void) {
    o->oForwardVel = 3.0f * TRACKER_difficulty_modifier_sqrt_half;
    cur_obj_init_animation_with_sound(BOWSER_ANIM_SLOW_GAIT);
    
    return troll_bowser_nyoom(o);
}

