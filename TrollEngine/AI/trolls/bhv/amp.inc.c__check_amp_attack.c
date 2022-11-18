#include "bhv_headers.h"


void check_amp_attack(void) {
    // Strange placement for this call. The hitbox is never cleared.
    // For perspective, this code is run every frame of bhv_circling_amp_loop
    // and every frame of a homing amp's HOMING_AMP_ACT_CHASE action.
    obj_set_hitbox(o, (void*)0x80332664);
    o->oDamageOrCoinValue = ampType + 1;

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        // This function is used for both normal amps and homing amps,
        // AMP_ACT_ATTACK_COOLDOWN == HOMING_AMP_ACT_ATTACK_COOLDOWN
        o->oAction = AMP_ACT_ATTACK_COOLDOWN;

        // Clear interact status
        o->oInteractStatus = 0;
    }
}

