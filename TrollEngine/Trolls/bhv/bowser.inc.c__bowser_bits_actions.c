#include "bhv_headers.h"


void bowser_bits_actions(void) {
    register f32 rand = random_float();
    register f32 diff_div_10 = TRACKER_difficulty_modifier / 10.0f;
    
    if (o->oBowserIsReacting) {
        o->oAction = troll_bowser_actions_bits_reacting(o, rand, diff_div_10);
    }
    else {
        bowser_bits_action_list();
    }
    o->oBowserIsReacting ^= TRUE;
}

