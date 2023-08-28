#include <PR/ultratypes.h>

#include "area.h"
#include "engine/math_util.h"
#include "game_init.h"
#include "gfx_dimensions.h"
#include "main.h"
#include "memory.h"
#include "print.h"
#include "rendering_graph_node.h"
#include "shadow.h"
#include "sm64.h"


typedef void func(struct GraphNode *node);
func *jumptable[];

void geo_process_node_and_siblings(struct GraphNode *firstNode) {
    s16 iterateChildren = TRUE;
    struct GraphNode *curGraphNode = firstNode;
    if (!is_pointer_valid(curGraphNode)) return;

    // In the case of a switch node, exactly one of the children of the node is
    // processed instead of all children like usual
    if (curGraphNode->parent != NULL) {
        iterateChildren = (curGraphNode->parent->type != GRAPH_NODE_TYPE_SWITCH_CASE);
    }

    do {
        if (!is_pointer_valid(curGraphNode)) return;

        if (curGraphNode->flags & GRAPH_RENDER_ACTIVE) {
            if (curGraphNode->flags & GRAPH_RENDER_CHILDREN_FIRST) {
                geo_try_process_children(curGraphNode);
            } else {
                s32 index = (curGraphNode->type & 0xFF) - (GRAPH_NODE_TYPE_ORTHO_PROJECTION & 0xFF);
                if (index >= 0 && index <= ((GRAPH_NODE_TYPE_HELD_OBJ & 0xFF) - (GRAPH_NODE_TYPE_ORTHO_PROJECTION & 0xFF)) && jumptable[index] != NULL) {
                    jumptable[index](curGraphNode);
                }
                else {
                    geo_try_process_children(curGraphNode);
                }
            }
        } else {
            if (curGraphNode->type == GRAPH_NODE_TYPE_OBJECT) {
                ((struct GraphNodeObject *) curGraphNode)->throwMatrix = NULL;
            }
        }
    } while (iterateChildren && (curGraphNode = curGraphNode->next) != firstNode);
}

void geo_process_ortho_projection(struct GraphNode *node);
void geo_process_perspective(struct GraphNode *node);
void geo_process_master_list(struct GraphNode *node);
void geo_process_level_of_detail(struct GraphNode *node);
void geo_process_switch(struct GraphNode *node);
void geo_process_camera(struct GraphNode *node);
void geo_process_translation_rotation(struct GraphNode *node);
void geo_process_translation(struct GraphNode *node);
void geo_process_rotation(struct GraphNode *node);
void geo_process_object(struct GraphNode *node);
void geo_process_animated_part(struct GraphNode *node);
void geo_process_billboard(struct GraphNode *node);
void geo_process_display_list(struct GraphNode *node);
void geo_process_scale(struct GraphNode *node);
void geo_process_shadow(struct GraphNode *node);
void geo_process_object_parent(struct GraphNode *node);
void geo_process_generated_list(struct GraphNode *node);
void geo_process_background(struct GraphNode *node);
void geo_process_held_object(struct GraphNode *node);
func *jumptable[] = {
    geo_process_ortho_projection,
    geo_process_perspective,
    geo_process_master_list,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
    geo_process_level_of_detail,
    geo_process_switch,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
    geo_process_camera,
    geo_process_translation_rotation,
    geo_process_translation,
    geo_process_rotation,
    geo_process_object,
    geo_process_animated_part,
    geo_process_billboard,
    geo_process_display_list,
    geo_process_scale,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
    geo_process_shadow,
    geo_process_object_parent,
    geo_process_generated_list,
NULL,
    geo_process_background,
NULL,
    geo_process_held_object,
};
