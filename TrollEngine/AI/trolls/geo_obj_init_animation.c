#include <ultra64.h>
#include "sm64.h"

#include "game/level_update.h"
#include "engine/math_util.h"
#include "game/memory.h"
#include "engine/graph_node.h"
#include "game/rendering_graph_node.h"
#include "game/area.h"
#include "engine/geo_layout.h"

s32 is_pointer_valid(void *ptr);

/**
 * Initialize the animation of an object node
 */
void geo_obj_init_animation(struct GraphNodeObject *graphNode, struct Animation **animPtrAddr) {
    struct Animation **animSegmented = segmented_to_virtual(animPtrAddr);
    struct Animation *anim;
    
    if (!is_pointer_valid(animSegmented)) return;
    
    anim = segmented_to_virtual(*animSegmented);
    if (!is_pointer_valid(anim)) return;

    if (graphNode->animInfo.curAnim != anim) {
        graphNode->animInfo.curAnim = anim;
        graphNode->animInfo.animFrame = anim->startFrame + ((anim->flags & ANIM_FLAG_FORWARD) ? 1 : -1);
        graphNode->animInfo.animAccel = 0;
        graphNode->animInfo.animYTrans = 0;
    }
}
