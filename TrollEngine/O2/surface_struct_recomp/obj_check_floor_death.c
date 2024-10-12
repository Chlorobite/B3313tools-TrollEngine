#include "obj_behaviors_headers.h"

/**
 *Checks if a floor is one that should cause an object to "die".
 */
void obj_check_floor_death(s16 collisionFlags, struct Surface *floor) {
    if (floor == NULL) {
        return;
    }

    if ((collisionFlags & OBJ_COL_FLAG_GROUNDED) == OBJ_COL_FLAG_GROUNDED) {
        switch (floor->type) {
            case SURFACE_BURNING:
                o->oAction = OBJ_ACT_LAVA_DEATH;
                break;
            //! @BUG Doesn't check for the vertical wind death floor.
            case SURFACE_DEATH_PLANE:
                o->oAction = OBJ_ACT_DEATH_PLANE_DEATH;
                break;
            default:
                break;
        }
    }
}
