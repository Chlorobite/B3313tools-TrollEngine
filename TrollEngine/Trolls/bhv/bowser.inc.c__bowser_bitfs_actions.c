#include "bhv_headers.h"


void bowser_bitfs_actions(void) {
    register struct Object *_o = o;
    register f32 rand = random_float();
    register u32 newAction = BOWSER_ACT_WALK_TO_MARIO;
    register f32 diff_div_10 = TRACKER_difficulty_modifier / 10.0f;
    
    if (!_o->oBowserIsReacting) {
        newAction = troll_bowser_actions_bitfs_not_reacting(_o, rand, diff_div_10);
        _o->oBowserIsReacting++;
    } else {
        // Keep walking
        _o->oBowserIsReacting = FALSE;
        if (rand < diff_div_10 - 0.2f) {
            if (_o->oDistanceToMario < 1300.0f) {
                newAction = BOWSER_ACT_TELEPORT;
            }
            else {
                newAction = BOWSER_ACT_CHARGE_MARIO;
            }
        }
        else if (rand < diff_div_10 - 0.1f) {
            newAction = BOWSER_ACT_BIG_JUMP;
        }
    }
    _o->oAction = newAction;
}

