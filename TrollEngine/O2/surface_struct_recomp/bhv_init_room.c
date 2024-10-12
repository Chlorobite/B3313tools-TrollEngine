#include "object_helpers_headers.h"

void bhv_init_room(void) {
    struct Surface *floor;
    f32 floorHeight;

    if (is_item_in_array(gCurrLevelNum, sLevelsWithRooms)) {
        floorHeight = find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);

        if (floor != NULL) {
            if (floor->room != 0) {
                o->oRoom = floor->room;
            } else {
                // Floor probably belongs to a platform object. Try looking
                // underneath it
                find_floor(o->oPosX, floorHeight - 100.0f, o->oPosZ, &floor);
                if (floor != NULL) {
                    //! Technically possible that the room could still be 0 here
                    o->oRoom = floor->room;
                }
            }
        }
    } else {
        o->oRoom = -1;
    }
}
