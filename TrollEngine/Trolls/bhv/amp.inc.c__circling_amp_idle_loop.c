#include "bhv_headers.h"


void circling_amp_idle_loop(void) {
    register struct Object *_o = o;
    // Move in a circle.
    // The Y oscillation uses the magic number 0x8B0 (2224), which is
    // twice that of the fixed amp. In other words, circling amps will
    // oscillate twice as fast. Also, unlike all other amps, circling
    // amps oscillate 60 units around their average Y instead of 40.
    _o->oPosX = _o->oHomeX + sins(_o->oMoveAngleYaw) * _o->oAmpRadiusOfRotation;
    _o->oPosZ = _o->oHomeZ + coss(_o->oMoveAngleYaw) * _o->oAmpRadiusOfRotation;
    _o->oPosY = _o->oHomeY + coss(_o->oAmpYPhase * 0x8B0) * 30.0f;
    _o->oMoveAngleYaw += 0x400 * (ampType + 1);
    _o->oFaceAngleYaw = _o->oMoveAngleYaw + 0x4000;

    // Handle attacks
    check_amp_attack();

    // Oscillate
    _o->oAmpYPhase++;

    cur_obj_play_sound_1(SOUND_AIR_AMP_BUZZ);
}

