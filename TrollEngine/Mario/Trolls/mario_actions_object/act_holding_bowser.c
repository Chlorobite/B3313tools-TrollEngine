#include "../mario_headers.h"
#include "AI/stats_tracking.h"

s32 act_holding_bowser(struct MarioState *m) {
    register s16 spin;
	register s16 max_spin = 0x1000;

	// eternal fart
	if (m->interactObj->oBehParams2ndByte == BOWSER_BP_BITS) {
		max_spin = 0x1800;
	}

    if (m->input & INPUT_B_PRESSED) {
        return set_mario_action(m, ACT_RELEASING_BOWSER, 0);
    }

    if (m->angleVel[1] == 0) {
        if (m->actionTimer++ > 120) {
            return set_mario_action(m, ACT_RELEASING_BOWSER, 1);
        }

        set_mario_animation(m, MARIO_ANIM_HOLDING_BOWSER);
    } else {
        m->actionTimer = 0;
        set_mario_animation(m, MARIO_ANIM_SWINGING_BOWSER);
    }

    if (m->intendedMag > 20.0f) {
        if (m->actionArg == 0) {
            m->actionArg = 1;
            m->twirlYaw = m->intendedYaw;
        } else {
            // spin = acceleration
            spin = (s16)(m->intendedYaw - m->twirlYaw) / 0x80;

            if (spin < -0x80) {
                spin = -0x80;
            }
            if (spin > 0x80) {
                spin = 0x80;
            }

            m->twirlYaw = m->intendedYaw;
            m->angleVel[1] += spin;

            if (m->angleVel[1] > max_spin) {
                m->angleVel[1] = max_spin;
            }
            if (m->angleVel[1] < -max_spin) {
                m->angleVel[1] = -max_spin;
            }
        }
    } else {
        m->actionArg = 0;
        m->angleVel[1] = approach_s32(m->angleVel[1], 0, 64, 64);
    }

    // spin = starting yaw
    spin = m->faceAngle[1];
    m->faceAngle[1] += m->angleVel[1];

    stationary_ground_step(m);

    if (get_red_star_count(gCurrSaveFileNum - 1) == 0) {
        if (m->angleVel[1] < 0) {
            TRACKER_flags &= ~TRACKER_flag_bowser_spin_direction;
        }
        else {
            TRACKER_flags |= TRACKER_flag_bowser_spin_direction;
        }
    }

	if (TRACKER_flags & TRACKER_flag_bowser_spin_direction) {
		m->marioObj->header.gfx.angle[0] = -m->angleVel[1];
	} else {
		m->marioObj->header.gfx.angle[0] = m->angleVel[1];
	}

    return FALSE;
}
