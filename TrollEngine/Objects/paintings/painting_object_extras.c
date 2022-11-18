#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"
#include "geo_commands_leek.h"
#include "make_const_nonconst.h"
#include "bhvdefs.h"

#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "game/rendering_graph_node.h"
#include "game/paintings.h"
#include "game/interaction.h"
#include "game/level_update.h"
#include "game/memory.h"
#include "game/object_list_processor.h"

#include "AI/personalization_helpers.h"

// vanilla
extern struct Painting **sPaintingGroups[];
// ours
extern struct Painting *paintings[];

extern void set_painting_layer(struct GraphNodeGenerated*, struct Painting*);
extern Gfx *display_painting_rippling(struct Painting*);
extern void wall_painting_update(struct Painting *painting, struct Painting *paintingGroup[]);
extern void floor_painting_update(struct Painting *painting, struct Painting *paintingGroup[]);
extern u32 interact_warp(struct MarioState *m, UNUSED u32 interactType, struct Object *o);
extern Gfx *painting_model_view_transform(struct Painting *painting);

extern f32 gPaintingMarioXPos;
extern f32 gPaintingMarioYPos;
extern f32 gPaintingMarioZPos;
extern struct MarioState gMarioStates[1];

extern u8 gLastPaintingId;
extern u16 gLastArea;
extern u8 gTrollWarpDestination;

extern u32 gCurrLevelArea;

extern Gfx *obj_display_painting(struct Painting *painting);
extern Gfx ripple_dl[];
extern Gfx non_ripple_dl[];
extern Gfx non_ripple_dl_smol[];
extern Lights1 painting_lighting;
extern Vtx static_painting_vertices[];
extern PaintingData *const painting_texture_maps[];

#define RIPPLE_LEFT 0x20
#define RIPPLE_MIDDLE 0x10
#define RIPPLE_RIGHT 0x8

#define ENTER_LEFT 0x4
#define ENTER_MIDDLE 0x2
#define ENTER_RIGHT 0x1



Gfx *obj_display_painting(register struct Painting *painting) {
	register Gfx *dlist;
	
	if (painting->state == PAINTING_IDLE) {
		// somewhat custom non rippling painting
		// the vanilla one requires an identical display list for each painting
		// which intensely wastes space
		dlist = alloc_display_list(4 * sizeof(Gfx));

		if (dlist != NULL) {
			gSPDisplayList(dlist, painting_model_view_transform(painting));
			gSPDisplayList(dlist + 1, painting_normal_dl(painting));
			gSPPopMatrix(dlist + 2, G_MTX_MODELVIEW);
			gSPEndDisplayList(dlist + 3);
		}
		return dlist;
    }
	return display_painting_rippling(painting);
}
