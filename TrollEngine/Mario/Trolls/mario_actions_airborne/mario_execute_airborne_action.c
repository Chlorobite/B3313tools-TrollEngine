#include "../mario_headers.h"


s32 act_jump(struct MarioState *m);
s32 act_double_jump(struct MarioState *m);
s32 act_freefall(struct MarioState *m);
s32 act_hold_jump(struct MarioState *m);
s32 act_hold_freefall(struct MarioState *m);
s32 act_side_flip(struct MarioState *m);
s32 act_wall_kick_air(struct MarioState *m);
s32 act_twirling(struct MarioState *m);
s32 act_water_jump(struct MarioState *m);
s32 act_hold_water_jump(struct MarioState *m);
s32 act_steep_jump(struct MarioState *m);
s32 act_burning_jump(struct MarioState *m);
s32 act_burning_fall(struct MarioState *m);
s32 act_triple_jump(struct MarioState *m);
s32 act_backflip(struct MarioState *m);
s32 act_long_jump(struct MarioState *m);
s32 act_riding_shell_air(struct MarioState *m);
s32 act_dive(struct MarioState *m);
s32 act_air_throw(struct MarioState *m);
s32 act_backward_air_kb(struct MarioState *m);
s32 act_forward_air_kb(struct MarioState *m);
s32 act_hard_forward_air_kb(struct MarioState *m);
s32 act_hard_backward_air_kb(struct MarioState *m);
s32 act_soft_bonk(struct MarioState *m);
s32 act_air_hit_wall(struct MarioState *m);
s32 act_forward_rollout(struct MarioState *m);
s32 act_shot_from_cannon(struct MarioState *m);
s32 act_butt_slide_air(struct MarioState *m);
s32 act_hold_butt_slide_air(struct MarioState *m);
s32 act_lava_boost(struct MarioState *m);
s32 act_getting_blown(struct MarioState *m);
s32 act_crazy_box_bounce(struct MarioState *m);
s32 act_special_triple_jump(struct MarioState *m);
s32 act_ground_pound(struct MarioState *m);
s32 act_thrown_forward(struct MarioState *m);
s32 act_thrown_backward(struct MarioState *m);
s32 act_flying_triple_jump(struct MarioState *m);
s32 act_slide_kick(struct MarioState *m);
s32 act_jump_kick(struct MarioState *m);
s32 act_flying(struct MarioState *m);
s32 act_riding_hoot(struct MarioState *m);
s32 act_top_of_pole_jump(struct MarioState *m);
s32 act_vertical_wind(struct MarioState *m);
s32 act_squatkick(struct MarioState *m);


s32 mario_execute_airborne_action(struct MarioState *m) {
    u32 cancel;

    if (check_common_airborne_cancels(m)) {
        return TRUE;
    }

    play_far_fall_sound(m);

    /* clang-format off */
    // this 0xFF spam allows the function to use a LUT and be much smaller
    switch (m->action & 0xFF) {
        case ACT_JUMP & 0xFF:                 cancel = act_jump(m);                 break;
        case ACT_DOUBLE_JUMP & 0xFF:          cancel = act_double_jump(m);          break;
        case ACT_FREEFALL & 0xFF:             cancel = act_freefall(m);             break;
        case ACT_HOLD_JUMP & 0xFF:            cancel = act_hold_jump(m);            break;
        case ACT_HOLD_FREEFALL & 0xFF:        cancel = act_hold_freefall(m);        break;
        case ACT_SIDE_FLIP & 0xFF:            cancel = act_side_flip(m);            break;
        case ACT_WALL_KICK_AIR & 0xFF:        cancel = act_wall_kick_air(m);        break;
        case ACT_TWIRLING & 0xFF:             cancel = act_twirling(m);             break;
        case ACT_WATER_JUMP & 0xFF:           cancel = act_water_jump(m);           break;
        case ACT_HOLD_WATER_JUMP & 0xFF:      cancel = act_hold_water_jump(m);      break;
        case ACT_STEEP_JUMP & 0xFF:           cancel = act_steep_jump(m);           break;
        case ACT_BURNING_JUMP & 0xFF:         cancel = act_burning_jump(m);         break;
        case ACT_BURNING_FALL & 0xFF:         cancel = act_burning_fall(m);         break;
        case ACT_TRIPLE_JUMP & 0xFF:          cancel = act_triple_jump(m);          break;
        case ACT_BACKFLIP & 0xFF:             cancel = act_backflip(m);             break;
        case ACT_LONG_JUMP & 0xFF:            cancel = act_long_jump(m);            break;
        case ACT_RIDING_SHELL_JUMP & 0xFF:
        case ACT_RIDING_SHELL_FALL & 0xFF:    cancel = act_riding_shell_air(m);     break;
        case ACT_DIVE & 0xFF:                 cancel = act_dive(m);                 break;
        case ACT_AIR_THROW & 0xFF:            cancel = act_air_throw(m);            break;
        case ACT_BACKWARD_AIR_KB & 0xFF:      cancel = act_backward_air_kb(m);      break;
        case ACT_FORWARD_AIR_KB & 0xFF:       cancel = act_forward_air_kb(m);       break;
        case ACT_HARD_FORWARD_AIR_KB & 0xFF:  cancel = act_hard_forward_air_kb(m);  break;
        case ACT_HARD_BACKWARD_AIR_KB & 0xFF: cancel = act_hard_backward_air_kb(m); break;
        case ACT_SOFT_BONK & 0xFF:            cancel = act_soft_bonk(m);            break;
        case ACT_AIR_HIT_WALL & 0xFF:         cancel = act_air_hit_wall(m);         break;
        case ACT_FORWARD_ROLLOUT & 0xFF:      cancel = act_forward_rollout(m);      break;
        case ACT_SHOT_FROM_CANNON & 0xFF:     cancel = act_shot_from_cannon(m);     break;
        case ACT_BUTT_SLIDE_AIR & 0xFF:       cancel = act_butt_slide_air(m);       break;
        case ACT_HOLD_BUTT_SLIDE_AIR & 0xFF:  cancel = act_hold_butt_slide_air(m);  break;
        case ACT_LAVA_BOOST & 0xFF:           cancel = act_lava_boost(m);           break;
        case ACT_GETTING_BLOWN & 0xFF:        cancel = act_getting_blown(m);        break;
        case ACT_BACKWARD_ROLLOUT & 0xFF:     cancel = act_backward_rollout(m);     break;
        case ACT_CRAZY_BOX_BOUNCE & 0xFF:     cancel = act_crazy_box_bounce(m);     break;
        case ACT_SPECIAL_TRIPLE_JUMP & 0xFF:  cancel = act_special_triple_jump(m);  break;
        case ACT_GROUND_POUND & 0xFF:         cancel = act_ground_pound(m);         break;
        case ACT_THROWN_FORWARD & 0xFF:       cancel = act_thrown_forward(m);       break;
        case ACT_THROWN_BACKWARD & 0xFF:      cancel = act_thrown_backward(m);      break;
        case ACT_FLYING_TRIPLE_JUMP & 0xFF:   cancel = act_flying_triple_jump(m);   break;
        case ACT_SLIDE_KICK & 0xFF:           cancel = act_slide_kick(m);           break;
        case ACT_JUMP_KICK & 0xFF:            cancel = act_jump_kick(m);            break;
        case ACT_FLYING & 0xFF:               cancel = act_flying(m);               break;
        case ACT_RIDING_HOOT & 0xFF:          cancel = act_riding_hoot(m);          break;
        case ACT_TOP_OF_POLE_JUMP & 0xFF:     cancel = act_top_of_pole_jump(m);     break;
        case ACT_VERTICAL_WIND & 0xFF:        cancel = act_vertical_wind(m);        break;
        case ACT_SQUATKICK & 0xFF:            cancel = act_squatkick(m);            break;
    }
    /* clang-format on */

    return cancel;
}
