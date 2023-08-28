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
#include "game/save_file.h"
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
extern u8 gIsLuigi;

extern u32 gCurrLevelArea;

extern Gfx *obj_display_painting(struct Painting *painting);
extern Gfx ripple_dl[];
extern Gfx non_ripple_dl[];
extern Lights1 painting_lighting;
extern Vtx static_painting_vertices[];
extern PaintingData *const painting_texture_maps[];

#define RIPPLE_LEFT 0x20
#define RIPPLE_MIDDLE 0x10
#define RIPPLE_RIGHT 0x8

#define ENTER_LEFT 0x4
#define ENTER_MIDDLE 0x2
#define ENTER_RIGHT 0x1

s32 eject = FALSE;
void painting_collision_test_lmao(register struct Painting *painting) {
    register s8 ripple = 0;
	register s32 positionShift;
	register Vec3f marioRelativePos;
	register f32 _cos, _sin, _tmp;
	register struct Object *obj = (struct Object *) gCurGraphNodeObject;

	// Map Mario's position to the painting's local space
	_tmp = -painting->yaw / 57.29577951308232f;
	_cos = cosf(_tmp);
	_sin = sinf(_tmp);
	marioRelativePos[0] = gPaintingMarioXPos - obj->oPosX;
	marioRelativePos[1] = gPaintingMarioYPos - painting->posY;
	marioRelativePos[2] = gPaintingMarioZPos - obj->oPosZ;
	
	_tmp = marioRelativePos[0];
	marioRelativePos[0] = _cos * marioRelativePos[0] + _sin * marioRelativePos[2];
	marioRelativePos[2] = -_sin * _tmp + _cos * marioRelativePos[2];
    
	// apply ripple based on mario's position
	
	// in front of / behind the painting?
    if (marioRelativePos[0] > 0.0f && marioRelativePos[0] < painting->size && marioRelativePos[2] > -150.0f * levelScaleH) {
		positionShift = (s32)(3.0f * (1.0f - marioRelativePos[0] / painting->size));
		// in the painting?
		if (marioRelativePos[2] < 0.0f && marioRelativePos[1] > -100.0f && marioRelativePos[1] < painting->size) {
			ripple = ENTER_RIGHT << positionShift;

			if (eject) {
				gRipplingPainting = painting; // needed by eject sound
                play_painting_eject_sound();
            } else {
				// eat mario to warp
				//vec3f_copy(&obj->oPosX, &gPaintingMarioXPos);
				//obj->oPosY = 0.0f;
				//vec3f_sub(&painting->posX, &gPaintingMarioXPos);
				obj->hitboxRadius = painting->size;
				obj->hitboxHeight = 50000.0f;
				obj->hitboxDownOffset = 25000.0f;
				obj->oInteractStatus = 0;
				obj->oIntangibleTimer = 0;

				if (!(save_file_get_flags() & SAVE_FLAG_YS_DISABLE_FIXED_WARPS)) {
					gLastPaintingId = (u8)painting->id;
					gLastArea = (u16)gCurrLevelArea;
				}

				// Luigi painting in internal garden
				if (painting->id == 126) {
					gIsLuigi = 1;
				}
			}
		}
		// in front of the painting?
		else if (marioRelativePos[2] < 100.0f) {
			ripple = RIPPLE_RIGHT << positionShift;
		}
	}

    painting->lastFloor = painting->currFloor;
    painting->currFloor = ripple;

    // floorEntered is true iff currFloor is true and lastFloor is false
    // (Mario just entered the floor on this frame)
    painting->floorEntered = (painting->lastFloor ^ painting->currFloor) & painting->currFloor;

    painting->marioWasUnder = painting->marioIsUnder;
    // Check if Mario has fallen below the painting (used for floor paintings)
    painting->marioIsUnder = gPaintingMarioYPos < painting->posY;

    // Mario "went under" if he was not under last frame, but is under now
    painting->marioWentUnder = (painting->marioWasUnder ^ painting->marioIsUnder) & painting->marioIsUnder;
}

Gfx *painting_normal_dl(register struct Painting *painting) {
	register Gfx *dlist;
	register Gfx *gfx;
	
	dlist = alloc_display_list(16 * sizeof(Gfx));

	if (dlist != NULL) {
		gSPDisplayList(dlist, non_ripple_dl);
		gSPVertex(dlist + 1, static_painting_vertices, 8, 0);
		gDPSetTextureImage(dlist + 2, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, painting->textureArray[0]);
		gDPLoadSync(dlist + 3);
		gDPLoadBlock(dlist + 4, G_TX_LOADTILE, 0, 0, 64 * 32 - 1, CALC_DXT(64, G_IM_SIZ_16b_BYTES));
		gfx = dlist + 5;
		gSP2Triangles(gfx++, 0,  1,  2, 0x0,  0,  2,  3, 0x0);
		gDPSetTextureImage(dlist + 7, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, painting->textureArray[1]);
		gDPLoadSync(dlist + 8);
		gDPLoadBlock(dlist + 9, G_TX_LOADTILE, 0, 0, 64 * 32 - 1, CALC_DXT(64, G_IM_SIZ_16b_BYTES));
		gfx = dlist + 10;
		gSP2Triangles(gfx++, 4,  5,  6, 0x0,  4,  6,  7, 0x0);
		gSPTexture(dlist + 12, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
		gDPPipeSync(dlist + 13);
		gDPSetCombineMode(dlist + 14, G_CC_SHADE, G_CC_SHADE);
		gSPEndDisplayList(dlist + 15);
	}
	
	return dlist;
}
Gfx *obj_display_painting(register struct Painting *painting);


Gfx *obj_geo_painting_draw(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    register struct GraphNodeGenerated *gen = (struct GraphNodeGenerated *) node;
    register struct Object* obj;
	register struct Painting *painting;
    register Gfx *paintingDlist = NULL;
	register struct GraphNodeCullingRadius *toyal = ((struct GraphNodeCullingRadius *) gLoadedGraphNodes[0xBA]);

    if (callContext == GEO_CONTEXT_RENDER) {
		obj = (struct Object *) gCurGraphNodeObject;
		painting = paintings[(u8)(obj->oBehParams >> 24)];
		
		if (obj->oPosX == 0.f && obj->oPosZ == 0.f) {
			eject = TRUE;
			gLastPaintingId = 0;
			gLastArea = 0;
			gTrollWarpDestination = FALSE;
			
			obj->oPosX = painting->posX * levelScaleH;
			painting->posY *= levelScaleV; // the painting uses the pos y for wobble Y position
			obj->oPosZ = painting->posZ * levelScaleH;
			painting->size *= levelScaleH;
			
			if (toyal->cullingRadius == 331369.f)
				toyal->cullingRadius = 0.0f;
			if (painting->size > toyal->cullingRadius)
				toyal->cullingRadius = painting->size;
			
			painting->posX = 0.f;
			painting->posZ = 0.f;
		}
		if (!(gMarioState->action & ACT_FLAG_INTANGIBLE)) {
			eject = FALSE;
		}
		
        // Determine if the painting is transparent
        set_painting_layer(gen, painting);

        // Draw before updating
        paintingDlist = obj_display_painting(painting);

        // Update the painting
        painting_collision_test_lmao(painting);
        if (painting->pitch == 0.0f) {
			wall_painting_update(painting, paintings + 127);
        }
        else {
			floor_painting_update(painting, paintings + 127);
		}
    }
    else if (callContext == GEO_CONTEXT_AREA_LOAD) {
		gLastPaintingId = 0;
		gLastArea = 0;
		gTrollWarpDestination = FALSE;
		toyal->cullingRadius = 331369.f;
	}
    return paintingDlist;
}

