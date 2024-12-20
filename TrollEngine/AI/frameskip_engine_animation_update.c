#include "O2/rendering_graph_node/rendering_graph_node_headers.h"

void physics_geo_try_process_children(struct GraphNode *node);
void physics_geo_process_node_and_siblings(struct GraphNode *firstNode);

void physics_geo_process_object(struct Object *node) {
    if (node->header.gfx.areaIndex == gCurGraphNodeRoot->areaIndex) {
        // FIXME: correct types
        if (is_pointer_valid(node->header.gfx.animInfo.curAnim)) {
            geo_set_animation_globals(&node->header.gfx.animInfo, (node->header.gfx.node.flags & GRAPH_RENDER_HAS_ANIMATION) != 0);
        }
    }
}

/**
 * Process a held object node.
 */
void physics_geo_process_held_object(struct GraphNodeHeldObject *node) {
    if (node->objNode != NULL && node->objNode->header.gfx.sharedChild != NULL) {
        s32 hasAnimation = (node->objNode->header.gfx.node.flags & GRAPH_RENDER_HAS_ANIMATION) != 0;

        if (node->objNode->header.gfx.animInfo.curAnim != NULL) {
            geo_set_animation_globals(&node->objNode->header.gfx.animInfo, hasAnimation);
        }
    }
}

/**
 * Process an object parent node. Temporarily assigns itself as the parent of
 * the subtree rooted at 'sharedChild' and processes the subtree, after which the
 * actual children are be processed. (in practice they are null though)
 */
void physics_geo_process_object_parent(struct GraphNodeObjectParent *node) {
    if (node->sharedChild != NULL) {
        node->sharedChild->parent = (struct GraphNode *) node;
        physics_geo_process_node_and_siblings(node->sharedChild);
        node->sharedChild->parent = NULL;
    }
    if (node->node.children != NULL) {
        physics_geo_process_node_and_siblings(node->node.children);
    }
}


/**
 * Processes the children of the given GraphNode if it has any
 */
void physics_geo_try_process_children(struct GraphNode *node) {
    if (node->children != NULL) {
        physics_geo_process_node_and_siblings(node->children);
    }
}

/**
 * Process a generic geo node and its siblings.
 * The first argument is the start node, and all its siblings will
 * be iterated over.
 */
void physics_geo_process_node_and_siblings(struct GraphNode *firstNode) {
    s16 iterateChildren = TRUE;
    struct GraphNode *curGraphNode = firstNode;
    if (!is_pointer_valid(curGraphNode)) return;

    // In the case of a switch node, exactly one of the children of the node is
    // processed instead of all children like usual
    /*if (curGraphNode->parent != NULL) {
        iterateChildren = (curGraphNode->parent->type != GRAPH_NODE_TYPE_SWITCH_CASE);
    }*/

    do {
        if (!is_pointer_valid(curGraphNode)) return;

        if (curGraphNode->flags & GRAPH_RENDER_ACTIVE) {
            if (curGraphNode->flags & GRAPH_RENDER_CHILDREN_FIRST) {
                geo_try_process_children(curGraphNode);
            } else {
                switch (curGraphNode->type) {
                    case GRAPH_NODE_TYPE_OBJECT_PARENT:
                        physics_geo_process_object_parent((struct GraphNodeObjectParent *) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_OBJECT:
                        physics_geo_process_object((struct Object *) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_HELD_OBJ:
                        physics_geo_process_held_object((struct GraphNodeHeldObject *) curGraphNode);
                        break;
                    default:
                        physics_geo_try_process_children((struct GraphNode *) curGraphNode);
                        break;
                }
            }
        }
    } while (iterateChildren && (curGraphNode = curGraphNode->next) != firstNode);
}

/**
 * Process a root node. This is the entry point for processing the scene graph.
 * The root node itself sets up the viewport, then all its children are processed
 * to set up the projection and draw display lists.
 */
void physics_geo_process_root(struct GraphNodeRoot *node) {
    if (node->node.flags & GRAPH_RENDER_ACTIVE) {
        gCurGraphNodeRoot = node;
        if (node->node.children != NULL) {
            physics_geo_process_node_and_siblings(node->node.children);
        }
        gCurGraphNodeRoot = NULL;
    }
}
