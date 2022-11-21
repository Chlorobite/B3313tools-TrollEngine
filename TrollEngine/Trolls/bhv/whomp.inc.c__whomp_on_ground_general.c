#include "bhv_headers.h"
#include "AI/stats_tracking.h"


void whomp_on_ground_general(void) {
    if (o->oSubAction != 10) {
        o->oForwardVel = 0.0f;
        o->oAngleVelPitch = 0;
        o->oAngleVelYaw = 0;
        o->oAngleVelRoll = 0;

        if (o->oBehParams2ndByte != 0) {
            king_whomp_on_ground();
        } else {
            whomp_on_ground();
        }
        if (o->oTimer > 100.f / TRACKER_difficulty_modifier_half || (gMarioState->action == ACT_SQUISHED && o->oTimer > 30)) {
            o->oSubAction = 10;
        }
    } else if (o->oFaceAnglePitch > 0) {
        o->oAngleVelPitch = -0x200;
        o->oFaceAnglePitch += o->oAngleVelPitch;
    } else {
        o->oAngleVelPitch = 0;
        o->oFaceAnglePitch = 0;
        if (o->oBehParams2ndByte != 0) {
            o->oAction = 2;
        } else {
            o->oAction = 1;
        }
    }
}

