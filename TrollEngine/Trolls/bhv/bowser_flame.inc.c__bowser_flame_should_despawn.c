#include "bhv_headers.h"



s32 bowser_flame_should_despawn(register s32 maxTime) {
    return MIN(maxTime, maxTime + 120 - loadedObjectCount) < o->oTimer || o->oFloorType == SURFACE_BURNING || o->oFloorType == SURFACE_DEATH_PLANE;
}
