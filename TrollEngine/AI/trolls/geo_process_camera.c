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

#include "AI/personalization_helpers.h"

/**
 * This file contains the code that processes the scene graph for rendering.
 * The scene graph is responsible for drawing everything except the HUD / text boxes.
 * First the root of the scene graph is processed when geo_process_root
 * is called from level_script.c. The rest of the tree is traversed recursively
 * using the function geo_process_node_and_siblings, which switches over all
 * geo node types and calls a specialized function accordingly.
 * The types are defined in engine/graph_node.h
 *
 * The scene graph typically looks like:
 * - Root (viewport)
 *  - Master list
 *   - Ortho projection
 *    - Background (skybox)
 *  - Master list
 *   - Perspective
 *    - Camera
 *     - <area-specific display lists>
 *     - Object parent
 *      - <group with 240 object nodes>
 *  - Master list
 *   - Script node (Cannon overlay)
 *
 */

extern s16 gMatStackIndex;
extern Mat4 gMatStack[32];
extern Mtx *gMatStackFixed[32];

/**
 * Animation nodes have state in global variables, so this struct captures
 * the animation state so a 'context switch' can be made when rendering the
 * held object.
 */
struct GeoAnimState {
    /*0x00*/ u8 type;
    /*0x01*/ u8 enabled;
    /*0x02*/ s16 frame;
    /*0x04*/ f32 translationMultiplier;
    /*0x08*/ u16 *attribute;
    /*0x0C*/ s16 *data;
};

// For some reason, this is a GeoAnimState struct, but the current state consists
// of separate global variables. It won't match EU otherwise.
extern struct GeoAnimState gGeoTempState;

extern u8 gCurrAnimType;
extern u8 gCurrAnimEnabled;
extern s16 gCurrAnimFrame;
extern f32 gCurrAnimTranslationMultiplier;
extern u16 *gCurrAnimAttribute;
extern s16 *gCurrAnimData;

extern struct AllocOnlyPool *gDisplayListHeap;

struct RenderModeContainer {
    u32 modes[8];
};

/* Rendermode settings for cycle 1 for all 8 layers. */
extern struct RenderModeContainer renderModeTable_1Cycle[];
/* Rendermode settings for cycle 2 for all 8 layers. */
extern struct RenderModeContainer renderModeTable_2Cycle[];

extern struct GraphNodeRoot *gCurGraphNodeRoot;
extern struct GraphNodeMasterList *gCurGraphNodeMasterList;
extern struct GraphNodePerspective *gCurGraphNodeCamFrustum;
extern struct GraphNodeCamera *gCurGraphNodeCamera;
extern struct GraphNodeObject *gCurGraphNodeObject;
extern struct GraphNodeHeldObject *gCurGraphNodeHeldObject;
extern u16 gAreaUpdateCounter;

void geo_process_camera(struct GraphNodeCamera *node) {
    register Mat4 cameraTransform;
    register Mtx *rollMtx = alloc_display_list(sizeof(*rollMtx));
    register Mtx *mtx = alloc_display_list(sizeof(*mtx));

    if (node->fnNode.func != NULL) {
        node->fnNode.func(GEO_CONTEXT_RENDER, &node->fnNode.node, gMatStack[gMatStackIndex]);
    }
    mtxf_rotate_xy(rollMtx, node->rollScreen);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(rollMtx), G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);

    mtxf_lookat(cameraTransform, node->pos, node->focus, node->roll);
    mtxf_mul(gMatStack[gMatStackIndex + 1], cameraTransform, gMatStack[gMatStackIndex]);
    gMatStackIndex++;
    mtxf_to_mtx(mtx, gMatStack[gMatStackIndex]);
    gMatStackFixed[gMatStackIndex] = mtx;
	// ADD: level scale, set renderingLevel flag for level scale
    renderingLevel = TRUE;
	// END ADD
    if (node->fnNode.node.children != 0) {
        gCurGraphNodeCamera = node;
        node->matrixPtr = &gMatStack[gMatStackIndex];
        geo_process_node_and_siblings(node->fnNode.node.children);
        gCurGraphNodeCamera = NULL;
    }
	// ADD: level scale, unset renderingLevel flag; don't apply level scale
    renderingLevel = FALSE;
	// END ADD
    gMatStackIndex--;
}
