#include "object_helpers_headers.h"

s32 cur_obj_move_xz(f32 steepSlopeNormalY, s32 careAboutEdgesAndSteepSlopes) {
    struct Surface *intendedFloor;

    f32 intendedX = o->oPosX + o->oVelX;
    f32 intendedZ = o->oPosZ + o->oVelZ;

    f32 intendedFloorHeight = find_floor(intendedX, o->oPosY, intendedZ, &intendedFloor);
    f32 deltaFloorHeight = intendedFloorHeight - o->oFloorHeight;

    o->oMoveFlags &= ~OBJ_MOVE_HIT_EDGE;

    if (o->oRoom != -1 && intendedFloor != NULL) {
        if (intendedFloor->room != 0 && o->oRoom != intendedFloor->room && intendedFloor->room != 18) {
            // Don't leave native room
            return FALSE;
        }
    }

    if (intendedFloorHeight < FLOOR_LOWER_LIMIT_MISC) {
        // Don't move into OoB
        o->oMoveFlags |= OBJ_MOVE_HIT_EDGE;
        return FALSE;
    } else if (deltaFloorHeight < 5.0f) {
        if (!careAboutEdgesAndSteepSlopes) {
            // If we don't care about edges or steep slopes, okay to move
            o->oPosX = intendedX;
            o->oPosZ = intendedZ;
            return TRUE;
        } else if (deltaFloorHeight < -50.0f && (o->oMoveFlags & OBJ_MOVE_ON_GROUND)) {
            // Don't walk off an edge
            o->oMoveFlags |= OBJ_MOVE_HIT_EDGE;
            return FALSE;
        } else if (intendedFloor->normal.y > steepSlopeNormalY) {
            // Allow movement onto a slope, provided it's not too steep
            o->oPosX = intendedX;
            o->oPosZ = intendedZ;
            return TRUE;
        } else {
            // We are likely trying to move onto a steep downward slope
            o->oMoveFlags |= OBJ_MOVE_HIT_EDGE;
            return FALSE;
        }
    } else if (intendedFloor->normal.y > steepSlopeNormalY || o->oPosY > intendedFloorHeight) {
        // Allow movement upward, provided either:
        // - The target floor is flat enough (e.g. walking up stairs)
        // - We are above the target floor (most likely in the air)
        o->oPosX = intendedX;
        o->oPosZ = intendedZ;
        //! Returning FALSE but moving anyway (not exploitable; return value is
        //  never used)
    }

    // We are likely trying to move onto a steep upward slope
    return FALSE;
}
