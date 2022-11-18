#include "headers.h"

u32 mario_check_object_grab(struct MarioState *m) {
    u32 result = FALSE;
    const BehaviorScript *script;

    if (m->input & INPUT_INTERACT_OBJ_GRABBABLE) {
        script = virtual_to_segmented(0x13, m->interactObj->behavior);

        if (script == bhvBowser) {
            s16 facingDYaw = m->faceAngle[1] - m->interactObj->oMoveAngleYaw;
            if (facingDYaw >= -0x5555 && facingDYaw <= 0x5555) {
                m->faceAngle[1] = m->interactObj->oMoveAngleYaw;
                m->usedObj = m->interactObj;
                result = set_mario_action(m, ACT_PICKING_UP_BOWSER, 0);
            }
        } else {
            s16 facingDYaw = mario_obj_angle_to_object(m, m->interactObj) - m->faceAngle[1];
            if (facingDYaw >= -0x2AAA && facingDYaw <= 0x2AAA) {
                m->usedObj = m->interactObj;

                if (!(m->action & ACT_FLAG_AIR)) {
                    set_mario_action(
                        m, (m->action & ACT_FLAG_DIVING) ? ACT_DIVE_PICKING_UP : ACT_PICKING_UP, 0);
                }

                result = TRUE;
            }
        }
    }

    return result;
}
