#include "bhv_headers.h"


void bowser_bits_action_list(void) {
    register struct Object *_o = o;
    register f32 rand = random_float();
    register u32 newAction = BOWSER_ACT_WALK_TO_MARIO;
    register f32 diff_div_10 = TRACKER_difficulty_modifier / 10.0f;
    
    if (_o->oBowserStatus & BOWSER_STATUS_ANGLE_MARIO) {
        newAction = troll_bowser_actions_bits_angle_mario(_o, rand, diff_div_10);
    } else {
        // Keep walking
        if (rand < diff_div_10 - 0.2f) {
            newAction = BOWSER_ACT_TELEPORT;
        }
        else if (rand < diff_div_10 - 0.1f) {
            newAction = BOWSER_ACT_BIG_JUMP;
        }
    }
    
    if (newAction == BOWSER_ACT_WALK_TO_MARIO && _o->oBehParams2ndByte == 3)
        newAction = BOWSER_ACT_TELEPORT;
    _o->oAction = newAction;
}

