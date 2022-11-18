#include <PR/ultratypes.h>

#include "sm64.h"
#include "game/area.h"
#include "game/behavior_actions.h"
#include "behavior_data.h"
#include "game/camera.h"
#include "game/debug.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/geo_layout.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "game/game_init.h"
#include "helper_macros.h"
#include "game/ingame_menu.h"
#include "game/interaction.h"
#include "level_table.h"
#include "game/level_update.h"
#include "game/mario.h"
#include "game/mario_actions_cutscene.h"
#include "game/memory.h"
#include "game/obj_behaviors.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/rendering_graph_node.h"
#include "game/spawn_object.h"
#include "game/spawn_sound.h"


static const Vtx coin_green[] = {
    {{{   -32,      0,      0}, 0, {     0,   1984}, {0x20, 0xff, 0x20, 0xff}}},
    {{{    32,      0,      0}, 0, {  1984,   1984}, {0x20, 0xff, 0x20, 0xff}}},
    {{{    32,     64,      0}, 0, {  1984,      0}, {0x20, 0xff, 0x20, 0xff}}},
    {{{   -32,     64,      0}, 0, {     0,      0}, {0x20, 0xff, 0x20, 0xff}}},
};

static const Vtx coin_orange[] = {
    {{{   -24,      0,      0}, 0, {     0,   1984}, {0xff, 0x80, 0x10, 0xff}}},
    {{{    24,      0,      0}, 0, {  1984,   1984}, {0xff, 0x80, 0x10, 0xff}}},
    {{{    24,     48,      0}, 0, {  1984,      0}, {0xff, 0x80, 0x10, 0xff}}},
    {{{   -24,     48,      0}, 0, {     0,      0}, {0xff, 0x80, 0x10, 0xff}}},
};

static const Vtx coin_purple[] = {
    {{{   -32,      0,      0}, 0, {     0,   1984}, {0x80, 0x10, 0xff, 0xff}}},
    {{{    32,      0,      0}, 0, {  1984,   1984}, {0x80, 0x10, 0xff, 0xff}}},
    {{{    32,     64,      0}, 0, {  1984,      0}, {0x80, 0x10, 0xff, 0xff}}},
    {{{   -32,     64,      0}, 0, {     0,      0}, {0x80, 0x10, 0xff, 0xff}}},
};

static const Vtx coin_white[] = {
    {{{   -32,      0,      0}, 0, {     0,   1984}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    32,      0,      0}, 0, {  1984,   1984}, {0xff, 0xff, 0xff, 0xff}}},
    {{{    32,     64,      0}, 0, {  1984,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   -32,     64,      0}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
};

static const Vtx coin_clear[] = {
    {{{   -32,      0,      0}, 0, {     0,   1984}, {0x20, 0xff, 0x20, 0xff}}},
    {{{    32,      0,      0}, 0, {  1984,   1984}, {0x20, 0xff, 0x20, 0xff}}},
    {{{    32,     64,      0}, 0, {  1984,      0}, {0x20, 0xff, 0x20, 0xff}}},
    {{{   -32,     64,      0}, 0, {     0,      0}, {0x20, 0xff, 0x20, 0xff}}},
};

static const Vtx coin_clear_visible[] = {
    {{{   -50,      0,      0}, 0, {     0,   1984}, {0x78, 0x78, 0xff, 0x80}}},
    {{{    50,      0,      0}, 0, {  1984,   1984}, {0x78, 0x78, 0xff, 0x80}}},
    {{{    50,    100,      0}, 0, {  1984,      0}, {0x78, 0x78, 0xff, 0x80}}},
    {{{   -50,    100,      0}, 0, {     0,      0}, {0x78, 0x78, 0xff, 0x80}}},
};

static const Vtx coin_pink[] = {
    {{{   -50,      0,      0}, 0, {     0,   1984}, {0xff, 0x00, 0xff, 0x80}}},
    {{{    50,      0,      0}, 0, {  1984,   1984}, {0xff, 0x00, 0xff, 0x80}}},
    {{{    50,    100,      0}, 0, {  1984,      0}, {0xff, 0x00, 0xff, 0x80}}},
    {{{   -50,    100,      0}, 0, {     0,      0}, {0xff, 0x00, 0xff, 0x80}}},
};

static const Texture tex_coin_outline[] = {
#include "coin_outline.ia16.inc"
};


Gfx *geo_yellow_coin(s32 callContext, struct GraphNode *node, UNUSED void *context) {
	Gfx *dlStart, *dlHead;
	struct Object* obj;
    struct GraphNodeGenerated *asGenerated;

	dlStart = NULL;

	if (callContext == GEO_CONTEXT_RENDER) {
		obj = (struct Object *) gCurGraphNodeObject;
        asGenerated = (struct GraphNodeGenerated *) node;

		dlStart = alloc_display_list(sizeof(Gfx) * 6);
		dlHead = dlStart;
		
		asGenerated->fnNode.node.flags =
			(asGenerated->fnNode.node.flags & 0xFF) | (LAYER_ALPHA << 8);

		switch (obj->oBehParams >> 24) {
			case 1: // green
				gDPPipeSync(dlHead++);
				gDPSetTextureImage(dlHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, 0x03005780 + 0x800 * ((obj->oAnimState >> 1) & 3));
				gSPDisplayList(dlHead++, 0x03007780);
				gSPVertex(dlHead++, coin_green, 4, 0);
				gSPBranchList(dlHead++, 0x030077D0);
				break;
			case 2: // orange
				gDPPipeSync(dlHead++);
				gDPSetTextureImage(dlHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, 0x03005780 + 0x800 * ((obj->oAnimState >> 1) & 3));
				gSPDisplayList(dlHead++, 0x03007780);
				gSPVertex(dlHead++, coin_orange, 4, 0);
				gSPBranchList(dlHead++, 0x030077D0);
				break;
			case 3: // purple
				gDPPipeSync(dlHead++);
				gDPSetTextureImage(dlHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, 0x03005780 + 0x800 * ((obj->oAnimState >> 1) & 3));
				gSPDisplayList(dlHead++, 0x03007780);
				gSPVertex(dlHead++, coin_purple, 4, 0);
				gSPBranchList(dlHead++, 0x030077D0);
				break;
			case 4: // white
				gDPPipeSync(dlHead++);
				gDPSetTextureImage(dlHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, 0x03005780 + 0x800 * ((obj->oAnimState >> 1) & 3));
				gSPDisplayList(dlHead++, 0x03007780);
				gSPVertex(dlHead++, coin_white, 4, 0);
				gSPBranchList(dlHead++, 0x030077D0);
				break;
			case 5: // clear
				gDPPipeSync(dlHead++);
				if (gMarioState->flags & MARIO_VANISH_CAP) {
					asGenerated->fnNode.node.flags =
						(asGenerated->fnNode.node.flags & 0xFF) | (LAYER_TRANSPARENT_INTER << 8);
					gDPSetTextureImage(dlHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, 0x03005780 + 0x800 * ((obj->oAnimState >> 1) & 3));
				}
				else {
					gDPSetTextureImage(dlHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, (Texture*)((u32)(&tex_coin_outline) & 0x7FFFFFFF));
				}
				gSPDisplayList(dlHead++, 0x03007780);
				gSPVertex(dlHead++, coin_clear_visible, 4, 0);
				gSPBranchList(dlHead++, 0x030077D0);
				break;
			case 6: // pink
				gDPPipeSync(dlHead++);
				gDPSetTextureImage(dlHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, 0x03005780 + 0x800 * ((obj->oAnimState >> 1) & 3));
				gSPDisplayList(dlHead++, 0x03007780);
				gSPVertex(dlHead++, coin_pink, 4, 0);
				gSPBranchList(dlHead++, 0x030077D0);
				break;
			default:
				gSPDisplayList(dlHead++, 0x03007800 + 0x28 * ((obj->oAnimState >> 1) & 3));
				break;
		}
		gSPEndDisplayList(dlHead);
	}

	return dlStart;
}
