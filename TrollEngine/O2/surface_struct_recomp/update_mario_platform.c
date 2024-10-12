#include <PR/ultratypes.h>

#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "level_update.h"
#include "object_fields.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "platform_displacement.h"
#include "types.h"

extern struct Object *gMarioPlatform;

/**
 * Determine if Mario is standing on a platform object, meaning that he is
 * within 4 units of the floor. Set his referenced platform object accordingly.
 */
void update_mario_platform(void) {
    struct Surface *floor;
    UNUSED u8 filler[4];
    f32 marioX;
    f32 marioY;
    f32 marioZ;
    f32 floorHeight;
    u32 awayFromFloor;

    if (gMarioObject == NULL) {
        return;
    }

    //! If Mario moves onto a rotating platform in a PU, the find_floor call
    //  will detect the platform and he will end up receiving a large amount
    //  of displacement since he is considered to be far from the platform's
    //  axis of rotation.

    marioX = gMarioObject->oPosX;
    marioY = gMarioObject->oPosY;
    marioZ = gMarioObject->oPosZ;
    floorHeight = find_floor(marioX, marioY, marioZ, &floor);

    if (absf(marioY - floorHeight) < 4.0f) {
        awayFromFloor = 0;
    } else {
        awayFromFloor = 1;
    }

    switch (awayFromFloor) {
        case 1:
            gMarioPlatform = NULL;
            gMarioObject->platform = NULL;
            break;

        case 0:
            if (floor != NULL && floor->object != NULL) {
                gMarioPlatform = floor->object;
                gMarioObject->platform = floor->object;
            } else {
                gMarioPlatform = NULL;
                gMarioObject->platform = NULL;
            }
            break;
    }
}
