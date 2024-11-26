#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "engine/graph_node.h"
#include "engine/surface_collision.h"
#include "game_init.h"
#include "geo_misc.h"
#include "mario.h"
#include "memory.h"
#include "moving_texture.h"
#include "object_list_processor.h"
#include "paintings.h"
#include "save_file.h"
#include "segment2.h"


s32 is_segmented_pointer_valid(void *ptr);

void stop_other_paintings(s16 *idptr, struct Painting *paintingGroup[]) {
    s16 index;
    s16 id = *idptr;

    index = 0;
    while (paintingGroup[index] != NULL) {
        struct Painting *painting = segmented_to_virtual(paintingGroup[index]);
        // data validity check, these pointers are required for the painting to even function
        if (is_segmented_pointer_valid((void*)painting->normalDisplayList)) {
            // stop all rippling except for the selected painting
            if (painting->id != id) {
                painting->state = 0;
            }
        }
        index++;
    }
}
