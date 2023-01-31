#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "audio/external.h"
#include "camera.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "game_init.h"
#include "interaction.h"
#include "level_update.h"
#include "mario.h"
#include "mario_step.h"
#include "save_file.h"
#include "rumble_init.h"

#define LERP(a, b, c) a = a + c * (b - a);

void update_flying(struct MarioState *m) {
    update_flying_pitch(m);
    update_flying_yaw(m);

    m->forwardVel -= 2.0f * ((f32) m->faceAngle[0] / 0x4000) + 0.1f;
    m->forwardVel -= 0.5f * (1.0f - coss(m->angleVel[1]));

    if (m->forwardVel < 0.0f) {
        m->forwardVel = 0.0f;
    }

	// ADD: mario "p-wing"
	// in beta, Mario could fly simply by shooting out of a cannon
	// this gives the wing cap an actual use
    if (m->flags & MARIO_WING_CAP) {
        m->particleFlags |= PARTICLE_SPARKLES;
        LERP(m->forwardVel, 48.0f, 0.05f);
    }
	// END ADD

    if (m->forwardVel > 16.0f) {
        m->faceAngle[0] += (m->forwardVel - 32.0f) * 6.0f;
    } else if (m->forwardVel > 4.0f) {
        m->faceAngle[0] += (m->forwardVel - 32.0f) * 10.0f;
    } else {
        m->faceAngle[0] -= 0x400;
    }

    m->faceAngle[0] += m->angleVel[0];

    if (m->faceAngle[0] > 0x2AAA) {
        m->faceAngle[0] = 0x2AAA;
    }
    if (m->faceAngle[0] < -0x2AAA) {
        m->faceAngle[0] = -0x2AAA;
    }

    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[1] = m->forwardVel * sins(m->faceAngle[0]);
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);

    m->slideVelX = m->vel[0];
    m->slideVelZ = m->vel[2];
}
