#include <ultra64.h>

#include "sm64.h"
#include "game_init.h"
#include "main.h"
#include "engine/math_util.h"
#include "area.h"
#include "level_update.h"
#include "save_file.h"
#include "sound_init.h"
#include "level_table.h"
#include "course_table.h"
#include "rumble_init.h"

extern struct SaveBuffer gSaveBuffer;

void save_file_collect_star_or_key(register s16 coinScore, register s16 starIndex) {
    register s32 fileIndex = gCurrSaveFileNum - 1;
    register s32 courseIndex = COURSE_NUM_TO_INDEX(gCurrCourseNum);
    register s32 starFlag;

    if (starIndex >= 16) {
        starIndex -= 16;
        courseIndex = COURSE_NUM_TO_INDEX(COURSE_MIN) + (starIndex >> 3);
        starIndex &= 7;
    }
    
    if (courseIndex == COURSE_NUM_TO_INDEX(COURSE_MIN) + 0x3D) {
        if (starIndex == 6) {
            save_file_set_flags(SAVE_FLAG_HAVE_KEY_1);
            return;
        }
        if (starIndex == 7) {
            save_file_set_flags(SAVE_FLAG_HAVE_KEY_2);
            return;
        }
    }
    
    if (courseIndex > COURSE_MAX - 1 + 16)
        return;

    starFlag = 1 << starIndex;

    gLastCompletedCourseNum = courseIndex + 1;
    gLastCompletedStarNum = starIndex + 1;

    // hi score system was deleted because unseen and extra stars
    
    switch (gCurrLevelNum) {
        case LEVEL_BOWSER_3:
            break;
        default:
            save_file_set_star_flags(fileIndex, courseIndex, starFlag);
            break;
    }
}
