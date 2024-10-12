#include "obj_behaviors_headers.h"

void calc_new_obj_vel_and_pos_y_underwater(struct Surface *objFloor, f32 floorY, f32 objVelX, f32 objVelZ,
                                    f32 waterY) {
    f32 floor_nX = objFloor->normal.x;
    f32 floor_nY = objFloor->normal.y;
    f32 floor_nZ = objFloor->normal.z;

    f32 netYAccel = (1.0f - o->oBuoyancy) * (-1.0f * o->oGravity);
    o->oVelY -= netYAccel;

    // Caps vertical speed with a "terminal velocity".
    if (o->oVelY > 75.0f) {
        o->oVelY = 75.0f;
    }
    if (o->oVelY < -75.0f) {
        o->oVelY = -75.0f;
    }

    o->oPosY += o->oVelY;

    //Snap the object up to the floor.
    if (o->oPosY < floorY) {
        o->oPosY = floorY;

        // Bounces an object if the ground is hit fast enough.
        if (o->oVelY < -17.5f) {
            o->oVelY = -(o->oVelY / 2);
        } else {
            o->oVelY = 0;
        }
    }

    // If moving fast near the surface of the water, flip vertical speed? To emulate skipping?
    if (o->oForwardVel > 12.5f && (waterY + 30.0f) > o->oPosY && (waterY - 30.0f) < o->oPosY) {
        o->oVelY = -o->oVelY;
    }

    if ((s32) o->oPosY >= (s32) floorY && (s32) o->oPosY < (s32) floorY + 37) {
        obj_orient_graph(o, floor_nX, floor_nY, floor_nZ);

        // Adds horizontal component of gravity for horizontal speed.
        objVelX += floor_nX * (floor_nX * floor_nX + floor_nZ * floor_nZ)
                   / (floor_nX * floor_nX + floor_nY * floor_nY + floor_nZ * floor_nZ) * netYAccel * 2;
        objVelZ += floor_nZ * (floor_nX * floor_nX + floor_nZ * floor_nZ)
                   / (floor_nX * floor_nX + floor_nY * floor_nY + floor_nZ * floor_nZ) * netYAccel * 2;
    }

    if (objVelX < 0.000001f && objVelX > -0.000001f) {
        objVelX = 0;
    }
    if (objVelZ < 0.000001f && objVelZ > -0.000001f) {
        objVelZ = 0;
    }

    if (o->oVelY < 0.000001f && o->oVelY > -0.000001f) {
        o->oVelY = 0;
    }

    if (objVelX != 0 || objVelZ != 0) {
        o->oMoveAngleYaw = atan2s(objVelZ, objVelX);
    }

    // Decreases both vertical velocity and forward velocity. Likely so that skips above
    // don't loop infinitely.
    o->oForwardVel = sqrtf(objVelX * objVelX + objVelZ * objVelZ) * 0.8f;
    o->oVelY *= 0.8f;
}
