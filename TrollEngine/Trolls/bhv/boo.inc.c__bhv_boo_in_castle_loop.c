#include "bhv_headers.h"

#define SPAWN_CASTLE_BOO_STAR_REQUIREMENT 12

f32 absf(f32 a);
void cur_obj_forward_vel_approach_upward(f32 a, f32 b);

void bhv_boo_in_castle_loop(void) {
    register s16 targetAngle;

    o->oBooBaseScale = 2.0f;

    if (o->oAction == 0) {
        cur_obj_unhide();

        o->oOpacity = 180;

        if (o->oTimer == 0) {
            cur_obj_scale(o->oBooBaseScale);
        }

        if (o->oDistanceToMario < 1000.0f && absf(o->oPosX - gMarioState->pos[0]) < 350.0f) {
            o->oAction++;
            cur_obj_play_sound_2(SOUND_OBJ_BOO_LAUGH_LONG);
        }

        o->oForwardVel = 0.0f;
        targetAngle = o->oAngleToMario;
    } else {
        cur_obj_forward_vel_approach_upward(32.0f, 1.0f);

        o->oHomeX = -1000.0f;
        o->oHomeZ = -9000.0f;

        targetAngle = cur_obj_angle_to_home();

        if (o->oPosZ < -5000.0f) {
            if (o->oOpacity > 0) {
                o->oOpacity -= 20;
            } else {
                o->oOpacity = 0;
            }
        }
    }

    o->oVelY = 0.0f;

    targetAngle = cur_obj_angle_to_home();

    cur_obj_rotate_yaw_toward(targetAngle, 0x5A8);
    boo_oscillate(TRUE);
    cur_obj_move_using_fvel_and_gravity();
}
