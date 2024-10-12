#include "obj_behaviors_headers.h"

/**
 * Updates an objects speed for gravity and updates Y position.
 */
void calc_new_obj_vel_and_pos_y(struct Surface *objFloor, f32 objFloorY, f32 objVelX, f32 objVelZ) {
    f32 floor_nX = objFloor->normal.x;
    f32 floor_nY = objFloor->normal.y;
    f32 floor_nZ = objFloor->normal.z;
    f32 objFriction;

    // Caps vertical speed with a "terminal velocity".
    o->oVelY -= o->oGravity;
    if (o->oVelY > 75.0f) {
        o->oVelY = 75.0f;
    }
    if (o->oVelY < -75.0f) {
        o->oVelY = -75.0f;
    }

    o->oPosY += o->oVelY;

    //Snap the object up to the floor.
    if (o->oPosY < objFloorY) {
        o->oPosY = objFloorY;

        // Bounces an object if the ground is hit fast enough.
        if (o->oVelY < -17.5f) {
            o->oVelY = -(o->oVelY / 2);
        } else {
            o->oVelY = 0;
        }
    }

    //! (Obj Position Crash) If you got an object with height past 2^31, the game would crash.
    if ((s32) o->oPosY >= (s32) objFloorY && (s32) o->oPosY < (s32) objFloorY + 37) {
        obj_orient_graph(o, floor_nX, floor_nY, floor_nZ);

        // Adds horizontal component of gravity for horizontal speed.
        objVelX += floor_nX * (floor_nX * floor_nX + floor_nZ * floor_nZ)
                   / (floor_nX * floor_nX + floor_nY * floor_nY + floor_nZ * floor_nZ) * o->oGravity
                   * 2;
        objVelZ += floor_nZ * (floor_nX * floor_nX + floor_nZ * floor_nZ)
                   / (floor_nX * floor_nX + floor_nY * floor_nY + floor_nZ * floor_nZ) * o->oGravity
                   * 2;

        if (objVelX < 0.000001f && objVelX > -0.000001f) {
            objVelX = 0;
        }
        if (objVelZ < 0.000001f && objVelZ > -0.000001f) {
            objVelZ = 0;
        }

        if (objVelX != 0 || objVelZ != 0) {
            o->oMoveAngleYaw = atan2s(objVelZ, objVelX);
        }

        calc_obj_friction(&objFriction, floor_nY);
        o->oForwardVel = sqrtf(objVelX * objVelX + objVelZ * objVelZ) * objFriction;
    }
}
