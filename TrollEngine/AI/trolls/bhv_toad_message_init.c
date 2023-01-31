#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "envfx_snow.h"
#include "game_init.h"
#include "goddard/renderer.h"
#include "interaction.h"
#include "level_update.h"
#include "mario_actions_cutscene.h"
#include "mario_misc.h"
#include "memory.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "skybox.h"
#include "sound_init.h"

enum ToadMessageStates {
    TOAD_MESSAGE_FADED,
    TOAD_MESSAGE_OPAQUE,
    TOAD_MESSAGE_OPACIFYING,
    TOAD_MESSAGE_FADING,
    TOAD_MESSAGE_TALKING
};


void bhv_toad_message_init(void) {
	// EDIT: See object docs channel; Toads, instead of being hardcoded to check stars for 3 different dialog options,
	// can now check any star ID and set any dialog ID if the star is collected.
	// Also removed the star count check; Toads are always available now
    register s32 dialogId = (gCurrentObject->oBehParams >> 24) & 0xFF;
    register s32 dialogIdPostStar = (gCurrentObject->oBehParams >> 16) & 0xFF;
    register s32 starId = (gCurrentObject->oBehParams & 0x1FF);
    register s32 courseId = COURSE_NUM_TO_INDEX(gCurrCourseNum);

	if (starId >= 16) {
		courseId = COURSE_NUM_TO_INDEX(COURSE_MIN) + ((starId - 16) >> 3);
		starId &= 7;
	}

	if (dialogIdPostStar != 0 && save_file_get_star_flags(gCurrSaveFileNum - 1, courseId) & (1 << starId)) {
		dialogId = dialogIdPostStar;
	}
    
	gCurrentObject->oToadMessageDialogId = dialogId;
	gCurrentObject->oToadMessageRecentlyTalked = FALSE;
	gCurrentObject->oToadMessageState = TOAD_MESSAGE_FADED;
	gCurrentObject->oOpacity = 81;
	// END EDIT
}
