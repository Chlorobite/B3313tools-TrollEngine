#include <ultra64.h>
#include "sm64.h"

#include "engine/geo_layout.h"
#include "engine/math_util.h"
#include "game/memory.h"
#include "engine/graph_node.h"

#include "AI/personalization_helpers.h"

typedef void (*GeoLayoutCommandProc)(void);

extern GeoLayoutCommandProc GeoLayoutJumpTable[];

extern struct GraphNode gObjParentGraphNode;
extern struct AllocOnlyPool *gGraphNodePool;
extern struct GraphNode *gCurRootGraphNode;

extern s32 D_8038BCA8;

/* The gGeoViews array is a mysterious one. Some background:
 *
 * If there are e.g. multiple Goombas, the multiple Goomba objects share one
 * Geo node tree describing the goomba 3D model. Since every node has a single
 * parent field and not a parent array, the parent is dynamically rebinded to
 * each goomba instance just before rendering and set to null afterwards.
 * The same happens for ObjectParentNode, which has as his sharedChild a group
 * of all 240 object nodes. Why does the ObjectParentNode exist at all, if its
 * only purpose is to temporarily bind the actual group with objects? This might
 * be another remnant to Luigi.
 *
 * When creating a root node, room for (2 + cmd+0x02) pointers is allocated in
 * gGeoViews. Except for the title screen, cmd+0x02 is 10. The 2 default ones
 * might be for Mario and Luigi, and the other 10 could be different cameras for
 * different rooms / boss fights. An area might be structured like this:
 *
 * geo_camera mode_player //Mario cam
 * geo_open_node
 *   geo_render_obj
 *   geo_assign_as_view 1   // currently unused geo command
 * geo_close_node
 *
 * geo_camera mode_player //Luigi cam
 * geo_open_node
 *   geo_render_obj
 *   geo_copy_view 1        // currently unused geo command
 *   geo_assign_as_view 2
 * geo_close_node
 *
 * geo_camera mode_boss //boss fight cam
 * geo_assign_as_view 3
 * ...
 *
 * There might also be specific geo nodes for Mario or Luigi only. Or a fixed camera
 * might not have display list nodes of parts of the level that are out of view.
 * In the end Luigi got scrapped and the multiple-camera design did not pan out,
 * so everything was reduced to a single ObjectParent with a single group, and
 * camera switching was all done in one node. End of speculation.
 */
extern struct GraphNode **gGeoViews;
extern u16 gGeoNumViews; // length of gGeoViews array

extern uintptr_t gGeoLayoutStack[16];
extern struct GraphNode *gCurGraphNodeList[32];
extern s16 gCurGraphNodeIndex;
extern s16 gGeoLayoutStackIndex; // similar to SP register in MIPS
extern s16 D_8038BD7C;
extern s16 gGeoLayoutReturnIndex; // similar to RA register in MIPS
extern u8 *gGeoLayoutCommand;

extern u32 unused_8038B894[3];


void geo_layout_cmd_node_background(void) {
    register struct GraphNodeBackground *graphNode;

	// EDIT: personalization AI, intercept init_graph_node_background, see AI/personalization_helpers.c
    graphNode = troll_init_graph_node_background(
        gGraphNodePool, NULL,
        cur_geo_cmd_s16(0x02), // background ID, or RGBA5551 color if asm function is null
        (GraphNodeFunc) cur_geo_cmd_ptr(0x04)); // asm function
	// END EDIT

    register_scene_graph_node(&graphNode->fnNode.node);

    gGeoLayoutCommand += 0x08 << CMD_SIZE_SHIFT;
}
