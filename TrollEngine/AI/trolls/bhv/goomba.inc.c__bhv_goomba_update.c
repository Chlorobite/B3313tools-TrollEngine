#include "bhv_headers.h"

extern struct ObjectHitbox sGoombaHitbox;
extern u8 sGoombaAttackHandlers[][6];

extern void (*gombaActions[])(void);

void bhv_goomba_update(void) {
    // PARTIAL_UPDATE
    register struct Object *_o = o;
    register f32 animSpeed;
    register f32 gombScale = _o->oGoombaScale;
    register s32 goombaHandledAttacks;

    if (obj_update_standard_actions(gombScale)) {
        // If this goomba has a spawner and mario moved away from the spawner, unload
        if (_o->parentObj != _o && _o->parentObj->oAction == GOOMBA_TRIPLET_SPAWNER_ACT_UNLOADED) {
            obj_mark_for_deletion(_o);
        }

        cur_obj_scale(gombScale);
        obj_update_blinking(&_o->oGoombaBlinkTimer, 30, 50, 5);
        cur_obj_update_floor_and_walls();

        if ((animSpeed = _o->oForwardVel / gombScale * 0.4f) < 1.0f) {
            animSpeed = 1.0f;
        }

        cur_obj_init_animation_with_accel_and_sound(0, animSpeed);

        gombaActions[_o->oAction]();
        
        if (goombaType == 3 && _o->oHealth == 0) {
            _o->oHealth = 2;
        }

        //! @bug Weak attacks on huge goombas in a triplet mark them as dead even if they're not.
        // obj_handle_attacks returns the type of the attack, which is non-zero
        // even for Mario's weak attacks. Thus, if Mario weakly attacks a huge goomba
        // without harming it (e.g. by punching it), the goomba will be marked as dead
        // and will not respawn if Mario leaves and re-enters the spawner's radius
        // even though the goomba isn't actually dead.
        if (obj_handle_attacks(&sGoombaHitbox, GOOMBA_ACT_ATTACKED_MARIO,
                               _o->oHealth > 1 ? sGoombaImmuneAttackHandlers : sGoombaAttackHandlers[_o->oGoombaSize & 1])) {
            if (_o->oHealth <= 1) {
                mark_goomba_as_dead();
            }
            if (_o->oHealth > 0)
                _o->oHealth--;
        }

        cur_obj_move_standard(-78);
    } else {
        _o->oAnimState = 1;
    }
}

void goomba_act_walk();
void goomba_act_attacked_mario();
void goomba_act_jump();
void (*gombaActions[])(void) = {
    goomba_act_walk,
    goomba_act_attacked_mario,
    goomba_act_jump
};
