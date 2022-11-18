#include "sm64.h"
#include "object_fields.h"
#include "object_helpers.h"
#include "object_list_processor.h"

#include "AI/personalization_helpers.h"

extern struct MarioState gMarioStates[1];
f32 absf(f32 f);

void *obj_create_mirror_mario() {
	register u8 flipArgs = gCurrentObject->oBehParams >> 24;
	register s32 viewRangeX = ((gCurrentObject->oBehParams & 0xFF0000) >> 16) * 20;
	register s32 viewRangeY = ((gCurrentObject->oBehParams & 0xFF00) >> 8) * 20;
	register s32 viewRangeZ = (gCurrentObject->oBehParams & 255) * 20;
	register char inRange;
	register struct GraphNodeObject *mirrorMario = (struct GraphNodeObject *)(&gCurrentObject->header);
	register struct Object *mario = gMarioStates[0].marioObj;

	if ((flipArgs & 0x20) == 0) {
		mirrorMario->sharedChild = mario->header.gfx.sharedChild;
	}
	mirrorMario->areaIndex = mario->header.gfx.areaIndex;

	if ((flipArgs & 8) != 0) {
		// box radius
		inRange = (viewRangeX == 0 || absf(mario->oPosX - gCurrentObject->oPosX) < viewRangeX * levelScaleH) &&
				(viewRangeY == 0 || absf(mario->oPosY - gCurrentObject->oPosY) < viewRangeY * levelScaleV) &&
				(viewRangeZ == 0 || absf(mario->oPosZ - gCurrentObject->oPosZ) < viewRangeZ * levelScaleH);
	}
	else {
		// sphere radius
		inRange = viewRangeX == 0 || (gCurrentObject->oDistanceToMario / levelScaleH) < viewRangeX;
	}
	
	if ((flipArgs & 0x10) != 0) {
		inRange = !inRange;
	}
	
	if (!inRange) {
		mirrorMario->node.flags &= ~GRAPH_RENDER_ACTIVE;
		mirrorMario->scale[0] = 0.0f;
		mirrorMario->scale[1] = 0.0f;
		mirrorMario->scale[2] = 0.0f;
	}
	else {
		vec3s_copy(mirrorMario->angle, mario->header.gfx.angle);
		vec3f_copy(mirrorMario->pos, mario->header.gfx.pos);
		vec3f_copy(mirrorMario->scale, mario->header.gfx.scale);

		mirrorMario->node.flags |= GRAPH_RENDER_ACTIVE;
		mirrorMario->animInfo = mario->header.gfx.animInfo;

		if (flipArgs & 1) {
			mirrorMario->pos[0] = 2 * gCurrentObject->oPosX - mirrorMario->pos[0];
			mirrorMario->angle[1] = -mirrorMario->angle[1];
			mirrorMario->scale[0] *= -1.0f;
		}

		if (flipArgs & 2) {
			mirrorMario->pos[1] = 2 * gCurrentObject->oPosY - mirrorMario->pos[1];
			mirrorMario->scale[1] *= -1.0f;
		}

		if (flipArgs & 4) {
			mirrorMario->pos[2] = 2 * gCurrentObject->oPosZ - mirrorMario->pos[2];
			mirrorMario->angle[1] = -mirrorMario->angle[1];
			mirrorMario->scale[2] *= -1.0f;
		}
	}
}

