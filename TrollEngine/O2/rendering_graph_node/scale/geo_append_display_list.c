#include "../rendering_graph_node_headers.h"
#include "AI/personalization_helpers.h"

void geo_append_display_list(void *displayList, s32 layer) {
    scaled_geo_append_display_list(displayList, layer);
}

void _geo_append_display_list(void *displayList, s32 layer) {
    register struct DisplayListNode *listNode =
        alloc_only_pool_alloc(gDisplayListHeap, sizeof(struct DisplayListNode));

    listNode->transform = gMatStackFixed[gMatStackIndex];
    listNode->displayList = displayList;
    listNode->next = 0;
    if (gCurGraphNodeMasterList->listHeads[layer] == 0) {
        gCurGraphNodeMasterList->listHeads[layer] = listNode;
    } else {
        gCurGraphNodeMasterList->listTails[layer]->next = listNode;
    }
    gCurGraphNodeMasterList->listTails[layer] = listNode;
}
