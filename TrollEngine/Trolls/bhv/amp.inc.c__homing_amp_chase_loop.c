#include "bhv_headers.h"

f32 absf(f32 arg);

void homing_amp_chase_loop(void) {
    register struct Object *_o = o;
    // Lock on to Mario if he ever goes within 11.25 degrees of the amp's line of sight
    if (absf(_o->oMoveAngleYaw - _o->oAngleToMario) < 0x400) {
        _o->oHomingAmpLockedOn = TRUE;
        _o->oTimer = 0;
    }

    // If the amp is locked on to Mario, start "chasing" him by moving
    // in a straight line at 15 units/second for 32 frames.
    if (_o->oHomingAmpLockedOn == TRUE) {
        f32 marioHead = gMarioObject->header.gfx.pos[1] + 150.0f;
        _o->oForwardVel = ampType == 1 ? 30.f : 15.f;

        // Move the amp's average Y (the Y value it oscillates around) to align with
        // Mario's head. Mario's graphics' Y + 150 is around the top of his head.
        // Note that the average Y will slowly go down to approach his head if the amp
        // is above his head, but if the amp is below it will instantly snap up.
        if (_o->oHomingAmpAvgY > marioHead) {
            _o->oHomingAmpAvgY -= 10.0f;
        } else {
            _o->oHomingAmpAvgY = marioHead;
        }

        if (_o->oTimer > 30) {
            _o->oHomingAmpLockedOn = FALSE;
        }
    } else {
        // If the amp is not locked on to Mario, move forward at 10 units/second
        // while curving towards him.
        _o->oForwardVel = ampType == 1 ? 20.f : 10.f;

        obj_turn_toward_object(o, gMarioObject, 16, 0x400);

        // The amp's average Y will approach Mario's graphical Y position + 250
        // at a rate of 10 units per frame. Interestingly, this is different from
        // the + 150 used while chasing him. Could this be a typo?
        if (_o->oHomingAmpAvgY < gMarioObject->header.gfx.pos[1] + 250.0f) {
            _o->oHomingAmpAvgY += 10.0f;
        }
    }

    // The amp's position will sinusoidally oscillate 40 units around its average Y.
    _o->oPosY = _o->oHomingAmpAvgY + sins(_o->oAmpYPhase * 0x400) * 20.0f;

    // Handle attacks
    check_amp_attack();

    // Give up if Mario goes further than 1500 units from the amp's original position
    if (!is_point_within_radius_of_mario(_o->oHomeX, _o->oHomeY, _o->oHomeZ, 1500.f)) {
        _o->oAction = HOMING_AMP_ACT_GIVE_UP;
    }
}

