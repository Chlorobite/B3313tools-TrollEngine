#include "object_helpers_headers.h"

//! @bug Same issue as geo_switch_anim_state.
#ifdef AVOID_UB
Gfx *geo_switch_area(s32 callContext, struct GraphNode *node, UNUSED void *context) {
#else
Gfx *geo_switch_area(s32 callContext, struct GraphNode *node) {
#endif
    s16 sp26;
    struct Surface *sp20;
    struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *) node;

    if (callContext == GEO_CONTEXT_RENDER) {
        if (gMarioObject == NULL) {
            switchCase->selectedCase = 0;
        } else {
            gFindFloorIncludeSurfaceIntangible = TRUE;

            find_floor(gMarioObject->oPosX, gMarioObject->oPosY, gMarioObject->oPosZ, &sp20);

            if (sp20) {
                gMarioCurrentRoom = sp20->room;
                sp26 = sp20->room - 1;

                if (sp26 >= 0) {
                    switchCase->selectedCase = sp26;
                }
            }
        }
    } else {
        switchCase->selectedCase = 0;
    }

    return NULL;
}
