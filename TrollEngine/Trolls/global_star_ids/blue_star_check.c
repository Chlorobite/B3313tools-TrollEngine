 #include <PR/ultratypes.h>

#include "sm64.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "game_init.h"
#include "geo_misc.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_table.h"
#include "level_update.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "obj_behaviors_2.h"
#include "object_constants.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "platform_displacement.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "seq_ids.h"
#include "spawn_sound.h"

#define POS_OP_SAVE_POSITION 0
#define POS_OP_COMPUTE_VELOCITY 1
#define POS_OP_RESTORE_POSITION 2

#define o gCurrentObject

u32 blue_star_check(register s32 behParams) {
	register s32 starId = ((behParams >> 24) & 0xFF) + (behParams & 0x100);
    register s32 courseId = COURSE_NUM_TO_INDEX(gCurrCourseNum);

	if (behParams & 0x8000) {
		o->oInteractionSubtype |= INT_SUBTYPE_NO_EXIT;
	}

	if (starId >= 16) {
		courseId = COURSE_NUM_TO_INDEX(COURSE_MIN) + ((starId - 16) >> 3);
		starId &= 7;
	}

	if (courseId == COURSE_NUM_TO_INDEX(COURSE_MIN) + 0x3D) {
		if (starId == 6) {
			return save_file_get_flags() & SAVE_FLAG_HAVE_KEY_1;
		}
		if (starId == 7) {
			return save_file_get_flags() & SAVE_FLAG_HAVE_KEY_2;
		}
	}

	return (save_file_get_star_flags(gCurrSaveFileNum - 1, courseId) & (1 << starId));
}
