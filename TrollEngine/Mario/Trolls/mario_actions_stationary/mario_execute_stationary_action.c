#include "../mario_headers.h"


s32 act_idle(struct MarioState *m);
s32 act_start_sleeping(struct MarioState *m);
s32 act_sleeping(struct MarioState *m);
s32 act_waking_up(struct MarioState *m);
s32 act_panting(struct MarioState *m);
s32 act_electric_idle(struct MarioState *m);
s32 act_hold_idle(struct MarioState *m);
s32 act_hold_heavy_idle(struct MarioState *m);
s32 act_in_quicksand(struct MarioState *m);
s32 act_standing_against_wall(struct MarioState *m);
s32 act_coughing(struct MarioState *m);
s32 act_shivering(struct MarioState *m);
s32 act_crouching(struct MarioState *m);
s32 act_start_crouching(struct MarioState *m);
s32 act_stop_crouching(struct MarioState *m);
s32 act_slide_kick_slide_stop(struct MarioState *m);
s32 act_shockwave_bounce(struct MarioState *m);
s32 act_first_person(struct MarioState *m);
s32 act_jump_land_stop(struct MarioState *m);
s32 act_double_jump_land_stop(struct MarioState *m);
s32 act_freefall_land_stop(struct MarioState *m);
s32 act_side_flip_land_stop(struct MarioState *m);
s32 act_hold_jump_land_stop(struct MarioState *m);
s32 act_hold_freefall_land_stop(struct MarioState *m);
s32 act_air_throw_land(struct MarioState *m);
s32 act_lava_boost_land(struct MarioState *m);
s32 act_twirl_land(struct MarioState *m);
s32 act_triple_jump_land_stop(struct MarioState *m);
s32 act_backflip_land_stop(struct MarioState *m);
s32 act_long_jump_land_stop(struct MarioState *m);
s32 act_ground_pound_land(struct MarioState *m);
s32 act_braking_stop(struct MarioState *m);
s32 act_butt_slide_stop(struct MarioState *m);
s32 act_hold_butt_slide_stop(struct MarioState *m);


s32 mario_execute_stationary_action(struct MarioState *m) {
    s32 cancel;

    if (check_common_stationary_cancels(m) || mario_update_quicksand(m, 0.5f)) {
        return TRUE;
    }

    /* clang-format off */
    switch (m->action) {
        case ACT_IDLE:                    cancel = act_idle(m); break;
        case ACT_START_SLEEPING:          cancel = act_start_sleeping(m); break;
        case ACT_SLEEPING:                cancel = act_sleeping(m); break;
        case ACT_WAKING_UP:               cancel = act_waking_up(m); break;
        case ACT_PANTING:                 cancel = act_panting(m); break;
        case ACT_ELECTRIC_FLOOR_IDLE:     cancel = act_electric_idle(m); break;
        case ACT_HOLD_IDLE:               cancel = act_hold_idle(m); break;
        case ACT_HOLD_HEAVY_IDLE:         cancel = act_hold_heavy_idle(m); break;
        case ACT_IN_QUICKSAND:            cancel = act_in_quicksand(m); break;
        case ACT_STANDING_AGAINST_WALL:   cancel = act_standing_against_wall(m); break;
        case ACT_COUGHING:                cancel = act_coughing(m); break;
        case ACT_SHIVERING:               cancel = act_shivering(m); break;
        case ACT_CROUCHING:               cancel = act_crouching(m); break;
        case ACT_START_CROUCHING:         cancel = act_start_crouching(m); break;
        case ACT_STOP_CROUCHING:          cancel = act_stop_crouching(m); break;
        case ACT_START_CRAWLING:          cancel = act_start_crawling(m); break;
        case ACT_STOP_CRAWLING:           cancel = act_stop_crawling(m); break;
        case ACT_SLIDE_KICK_SLIDE_STOP:   cancel = act_slide_kick_slide_stop(m); break;
        case ACT_SHOCKWAVE_BOUNCE:        cancel = act_shockwave_bounce(m); break;
        case ACT_FIRST_PERSON:            cancel = act_first_person(m); break;
        case ACT_JUMP_LAND_STOP:          cancel = act_jump_land_stop(m); break;
        case ACT_DOUBLE_JUMP_LAND_STOP:   cancel = act_double_jump_land_stop(m); break;
        case ACT_FREEFALL_LAND_STOP:      cancel = act_freefall_land_stop(m); break;
        case ACT_SIDE_FLIP_LAND_STOP:     cancel = act_side_flip_land_stop(m); break;
        case ACT_HOLD_JUMP_LAND_STOP:     cancel = act_hold_jump_land_stop(m); break;
        case ACT_HOLD_FREEFALL_LAND_STOP: cancel = act_hold_freefall_land_stop(m); break;
        case ACT_AIR_THROW_LAND:          cancel = act_air_throw_land(m); break;
        case ACT_LAVA_BOOST_LAND:         cancel = act_lava_boost_land(m); break;
        case ACT_TWIRL_LAND:              cancel = act_twirl_land(m); break;
        case ACT_TRIPLE_JUMP_LAND_STOP:   cancel = act_triple_jump_land_stop(m); break;
        case ACT_BACKFLIP_LAND_STOP:      cancel = act_backflip_land_stop(m); break;
        case ACT_LONG_JUMP_LAND_STOP:     cancel = act_long_jump_land_stop(m); break;
        case ACT_GROUND_POUND_LAND:       cancel = act_ground_pound_land(m); break;
        case ACT_BRAKING_STOP:            cancel = act_braking_stop(m); break;
        case ACT_BUTT_SLIDE_STOP:         cancel = act_butt_slide_stop(m); break;
        case ACT_HOLD_BUTT_SLIDE_STOP:    cancel = act_hold_butt_slide_stop(m); break;
    }
    /* clang-format on */

    if (!cancel && (m->input & INPUT_IN_WATER)) {
        m->particleFlags |= PARTICLE_IDLE_WATER_WAVE;
    }

    return cancel;
}
