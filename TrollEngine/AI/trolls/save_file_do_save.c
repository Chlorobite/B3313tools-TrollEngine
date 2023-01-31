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

#include "AI/stats_tracking.h"

#define MENU_DATA_MAGIC 0x4849
#define SAVE_FILE_MAGIC 0x4441

STATIC_ASSERT(sizeof(struct SaveBuffer) == EEPROM_SIZE, "eeprom buffer size must match");

extern struct SaveBuffer gSaveBuffer;

extern struct WarpCheckpoint gWarpCheckpoint;

extern s8 gMainMenuDataModified;
extern s8 gSaveFileModified;

extern u8 gLastCompletedCourseNum;
extern u8 gLastCompletedStarNum;
extern s8 sUnusedGotGlobalCoinHiScore;
extern u8 gGotFileCoinHiScore;
extern u8 gCurrCourseStarFlags;

extern u8 gSpecialTripleJump;

#define STUB_LEVEL(_0, _1, courseenum, _3, _4, _5, _6, _7, _8) courseenum,
#define DEFINE_LEVEL(_0, _1, courseenum, _3, _4, _5, _6, _7, _8, _9, _10) courseenum,


void save_file_do_save(s32 fileIndex) {
    if (gSaveFileModified) {
        // Compute checksum
        add_save_block_signature(&gSaveBuffer.files[fileIndex][0],
                                 sizeof(gSaveBuffer.files[fileIndex][0]), SAVE_FILE_MAGIC);

        // Copy to backup slot
        bcopy(&gSaveBuffer.files[fileIndex][0], &gSaveBuffer.files[fileIndex][1],
              sizeof(gSaveBuffer.files[fileIndex][1]));

        // Write to EEPROM
        write_eeprom_data(gSaveBuffer.files[fileIndex], sizeof(gSaveBuffer.files[fileIndex]));

        gSaveFileModified = FALSE;
    }

	// ADD: avoid overriding the main menu data...?
	// seems like it prevents AI stuff from actually being saved, probably remove this lol
    gMainMenuDataModified = TRUE;
	// END ADD
    save_main_menu_data();
}
