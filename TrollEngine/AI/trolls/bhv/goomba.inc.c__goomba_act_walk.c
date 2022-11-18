#include "bhv_headers.h"


void goomba_act_walk(void) {
    register struct Object *_o = o;
    const register u8 _goombaType = goombaType;
    const register s32 is_fast = _goombaType == 2 || _goombaType == 3;
    
    if (_goombaType != 5) {
        treat_far_home_as_mario(1000.0f);
    }

    obj_forward_vel_approach(_o->oGoombaRelativeSpeed * _o->oGoombaScale, 0.4f);

    // If walking fast enough, play footstep sounds
    if (_o->oGoombaRelativeSpeed > 2.0f) {
        cur_obj_play_sound_at_anim_range(2, 17, SOUND_OBJ_GOOMBA_WALK);
    }

    if (is_fast || _goombaType == 6) {
        _o->oDamageOrCoinValue = 2;
    }

    //! By strategically hitting a wall, steep slope, or another goomba, we can
    //  prevent the goomba from turning back toward home for a while (goomba
    //  chase extension)
    //! It seems theoretically possible to get 2-3 goombas to repeatedly touch
    //  each other and move arbitrarily far from their home, but it's
    //  extremely precise and chaotic in practice, so probably can't be performed
    //  for nontrivial distances
    if (_o->oGoombaTurningAwayFromWall) {
        _o->oGoombaTurningAwayFromWall = obj_resolve_collisions_and_turn(_o->oGoombaTargetYaw, 0x200);
    } else {
        // If far from home, walk toward home.
        if (_o->oDistanceToMario >= 25000.0f) {
            _o->oGoombaTargetYaw = _o->oAngleToMario;
            _o->oGoombaWalkTimer = random_linear_offset(20, 30);
        }

        if (!(_o->oGoombaTurningAwayFromWall =
                  obj_bounce_off_walls_edges_objects(&_o->oGoombaTargetYaw))) {
            if (_o->oDistanceToMario < 500.0f && _goombaType != 4) {
                // If close to mario, begin chasing him. If not already chasing
                // him, jump first

                if (_o->oGoombaRelativeSpeed <= 2.0f) {
                    goomba_begin_jump();
                }

                _o->oGoombaTargetYaw = _o->oAngleToMario;
                if (_goombaType == 5) {
                    _o->oGoombaTargetYaw += 0x8000; // face away
                    _o->oNumLootCoins = -1; // set blue coin
                }
                _o->oGoombaRelativeSpeed = (is_fast || _goombaType == 5) ? 40.0f : 20.0f;
            } else {
                // If mario is far away, walk at a normal pace, turning randomly
                // and occasionally jumping

                troll_goomba_wander(_o);
            }
        }

        cur_obj_rotate_yaw_toward(_o->oGoombaTargetYaw, (is_fast ? 0x600 : 0x200));
    }
}

