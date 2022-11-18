#include "bhv_headers.h"


void bowser_bitdw_actions(void) {
    register struct Object *_o = o;
    register f32 rand = random_float();
    register u32 newAction = BOWSER_ACT_WALK_TO_MARIO;
    register f32 diff_div_10 = TRACKER_difficulty_modifier / 10.0f;
    
    if (!_o->oBowserIsReacting) {
        newAction = troll_bowser_actions_bitdw_not_reacting(_o, rand, diff_div_10);
        _o->oBowserIsReacting++;
    } else {
        _o->oBowserIsReacting = FALSE;
        // Set starting Bowser level actions, randomly he can also start
        // dancing after the introduction
        if (rand < diff_div_10 - 0.2f) {
            newAction = BOWSER_ACT_QUICK_JUMP;
        }
        else if (rand < diff_div_10 - 0.1f) {
            newAction = BOWSER_ACT_CHARGE_MARIO;
        }
        else if (rand < 0.1) {
            newAction = BOWSER_ACT_DANCE;
        }
    }
    _o->oAction = newAction;
}

