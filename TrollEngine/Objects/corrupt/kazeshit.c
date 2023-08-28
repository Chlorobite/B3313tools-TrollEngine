#include <PR/ultratypes.h>
#include "sm64.h"
#include "ai/stats_tracking.h"
#include "ai/audio_trolls.h"
#include "ai/personalization_helpers.h"
#include "ai/post_object_load_pass.h"
#include "game/interaction.h"
#include "game/level_update.h"
#include "game/memory.h"
#include "game/paintings.h"
#include "game/moving_texture.h"
#include "game/object_helpers.h"
#include "game/object_list_processor.h"
#include "game/save_file.h"
#include "game/spawn_object.h"
#include "engine/behavior_script.h"
#include "engine/surface_collision.h"
#include "behavior_data.h"
#include "course_table.h"

#include "actors/common1.h"
#include "make_const_nonconst.h"

extern u32 *area_geo_layout_addresses[16];

extern u32 *foundTextures[256];
s32 corruptTimer;
u32 shiftAmount;

static void crawlDisplayList(u32 *ptr) {
	register u32 *trollptr;
	u16 vtxSize;
	register s32 i;

	//if ((u32)ptr & 3) return;

	while (1) {
		switch ((*ptr) >> 24) {
			case 0x06: // gsSPDisplayList
				trollptr = (u32*)segmented_to_virtual((void*)*(ptr + 1));
				crawlDisplayList(trollptr);
				break;
			case 0xB8: // gsSPEndDisplayList
				return;

			case 0xFD: // gsDPSetTextureImage
				trollptr = (u32*)segmented_to_virtual((void*)*(ptr + 1));

				/*if ((u32)trollptr & 3) {
					trollptr = NULL;
					break;
				}*/

				for (i = 0; i < 256; i++) {
					if (foundTextures[i] == trollptr) {
						trollptr = NULL;
						break;
					}
				}

				if (trollptr != NULL) {
					for (i = 0; i < 256; i++) {
						if (foundTextures[i] == NULL) {
							foundTextures[i] = trollptr;
							break;
						}
					}

                    *(ptr + 1) = (*(ptr + 1) + shiftAmount) & 0x3FFFFF;
				}
				break;
		}

		ptr += 2;
	}
}

static void crawlGeoLayout(u32 *ptr) {
	u32 *dl;
    s32 i;

	for (i = 0; i < 256; i++) {
        foundTextures[i] = NULL;
	}

	while (1) {
		switch ((*ptr) >> 24) {
			case 0x01: // GEO_END
				return;
			case 0x15: // GEO_DISPLAY_LIST
				dl = (u32*)segmented_to_virtual((void*)*(++ptr));
				crawlDisplayList(dl);
				break;
		}

		ptr++;
	}
}

void corrupt_entryPoint(u32 shift) {
    if (corruptTimer < 6) {
        corruptTimer++;
        return;
    }
    else {
        corruptTimer = 0;
    }

    shiftAmount = shift;
	crawlGeoLayout(segmented_to_virtual(area_geo_layout_addresses[gCurrentArea->index]));
}
