/*

The new save file format is completely different for B3313, so the entire file has been recompiled.
Instead of a backup copy for each save file, we have an xor-ed version for each:

Vanilla:
File A - File A backup
File B - File B backup
File C - File C backup
File D - File D backup

B3313:
File A - File B
File C - File D
File A^B - File C^D

This saves significant space, allowing us to have save files supporting an extra 144 stars as well as the Gamma key stuff. A single corrupted save file can still be recovered perfectly fine. The unlikely case where 2 save files are corrupted at once technically adds a little bit of risk, which might only be semi-relevant with the real N64 cartridge (emulators are completely safe, so are Everdrives which save to the SD card).

*/

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

#define MENU_DATA_MAGIC 0x4849
#define SAVE_FILE_MAGIC 0x4441

//char checker(int);
//char checkSizeOfInt[sizeof(struct SaveBuffer)]={checker(&checkSizeOfInt)};

STATIC_ASSERT(sizeof(struct SaveBuffer) == EEPROM_SIZE, "eeprom buffer size must match");

extern struct SaveBuffer gSaveBuffer;

struct WarpCheckpoint gWarpCheckpoint;

s8 gMainMenuDataModified;
s8 gSaveFileModified;

u8 gLastCompletedCourseNum = COURSE_NONE;
u8 gLastCompletedStarNum = 0;
s8 sUnusedGotGlobalCoinHiScore = FALSE;
u8 gGotFileCoinHiScore = FALSE;
u8 gCurrCourseStarFlags = 0;

u8 gSpecialTripleJump = FALSE;

u8 backupXorBuffer[sizeof(struct SaveFile)];

/**
 * Read from EEPROM to a given address.
 * The EEPROM address is computed using the offset of the destination address from gSaveBuffer.
 * Try at most 4 times, and return 0 on success. On failure, return the status returned from
 * osEepromLongRead. It also returns 0 if EEPROM isn't loaded correctly in the system.
 */
s32 read_eeprom_data(void *buffer, s32 size) {
    s32 status = 0;

    if (gEepromProbe != 0) {
        s32 triesLeft = 4;
        u32 offset = (u32)((u8 *) buffer - (u8 *) &gSaveBuffer) / 8;

        do {
#if ENABLE_RUMBLE
            block_until_rumble_pak_free();
#endif
            triesLeft--;
            status = osEepromLongRead(&gSIEventMesgQueue, offset, buffer, size);
#if ENABLE_RUMBLE
            release_rumble_pak_control();
#endif
        } while (triesLeft > 0 && status != 0);
    }

    return status;
}

/**
 * Write data to EEPROM.
 * The EEPROM address is computed using the offset of the source address from gSaveBuffer.
 * Try at most 4 times, and return 0 on success. On failure, return the status returned from
 * osEepromLongWrite. Unlike read_eeprom_data, return 1 if EEPROM isn't loaded.
 */
s32 write_eeprom_data(void *_buffer, s32 size) {
    s32 status = 1;
    u8 *buffer = (u8*)_buffer;

    while (((u32)buffer & 7) != 0) {
        buffer--;
        size++;
    }

    while ((size & 7) != 0) {
        size++;
    }

    if (gEepromProbe != 0) {
        s32 triesLeft = 4;
        u32 offset = (u32)((u8 *) buffer - (u8 *) &gSaveBuffer) >> 3;

        do {
#if ENABLE_RUMBLE
            block_until_rumble_pak_free();
#endif
            triesLeft--;
            status = osEepromLongWrite(&gSIEventMesgQueue, offset, buffer, size);
#if ENABLE_RUMBLE
            release_rumble_pak_control();
#endif
        } while (triesLeft > 0 && status != 0);
    }

    return status;
}

/**
 * Sum the bytes in data to data + size - 2. The last two bytes are ignored
 * because that is where the checksum is stored.
 */
u16 calc_checksum(u8 *data, s32 size) {
    u16 chksum = 0;

    while (size-- > 2) {
        chksum += *data++;
    }
    return chksum;
}

/**
 * Verify the signature at the end of the block to check if the data is valid.
 */
s32 verify_save_block_signature(void *buffer, s32 size, u16 magic) {
    struct SaveBlockSignature *sig = (struct SaveBlockSignature *) ((size - 4) + (u8 *) buffer);

    if (sig->magic != magic) {
        return 0;
    }
    if (sig->chksum != calc_checksum(buffer, size)) {
        return 0;
    }
    return 1;
}

/**
 * Write a signature at the end of the block to make sure the data is valid
 */
void add_save_block_signature(void *buffer, s32 size, u16 magic) {
    struct SaveBlockSignature *sig = (struct SaveBlockSignature *) ((size - 4) + (u8 *) buffer);

    sig->magic = magic;
    sig->chksum = calc_checksum(buffer, size);
}

static void save_main_menu_data(void) {
    if (gMainMenuDataModified) {
        // Troll the data with personalization data
        TRACKER_inject_save();

        // Compute checksum
        add_save_block_signature(&gSaveBuffer.menuData, sizeof(gSaveBuffer.menuData), MENU_DATA_MAGIC);

        // Write to EEPROM
        write_eeprom_data(&gSaveBuffer.menuData, sizeof(gSaveBuffer.menuData));

        gMainMenuDataModified = FALSE;
    }
}

static void wipe_main_menu_data(void) {
    bzero(&gSaveBuffer.menuData, sizeof(gSaveBuffer.menuData));

    gMainMenuDataModified = TRUE;
    save_main_menu_data();
}

static void xor_save_file_backup(s32 fileIndex) {
    u8 *bufferA = (u8*)&gSaveBuffer.files[fileIndex];
    u8 *bufferB = (u8*)&gSaveBuffer.files[fileIndex ^ 1];
    u8 *bufferXor = (u8*)&gSaveBuffer.file_backups[fileIndex >> 1];
    u32 i;

    for (i = 0; i < sizeof(struct SaveFile); i++) {
        bufferXor[i] = bufferA[i] ^ bufferB[i];
    }

    // Write destination data to EEPROM
    write_eeprom_data(bufferXor, sizeof(struct SaveFile));
}

static void load_xored_save_file_backup(s32 fileIndex) {
    u8 *bufferOther = (u8*)&gSaveBuffer.files[fileIndex ^ 1];
    u8 *bufferBackup = (u8*)&gSaveBuffer.file_backups[fileIndex >> 1];
    u32 i;

    for (i = 0; i < sizeof(struct SaveFile); i++) {
        backupXorBuffer[i] = bufferBackup[i] ^ bufferOther[i];
    }
}

/**
 * Copy save file data from one backup slot to the other slot.
 */
void restore_save_file_data(s32 destFileIndex) {
    void *srcBuffer = &backupXorBuffer;
    void *destBuffer = &gSaveBuffer.files[destFileIndex];

    load_xored_save_file_backup(destFileIndex);

    // Verify the xored data, just in case. If invalid, we can't recover - erase the destination file.
    if (!verify_save_block_signature(&backupXorBuffer, sizeof(backupXorBuffer), SAVE_FILE_MAGIC)) {
        save_file_erase(destFileIndex);
        return;
    }

    // Compute checksum on source data
    add_save_block_signature(srcBuffer, sizeof(struct SaveFile), SAVE_FILE_MAGIC);

    // Copy source data to destination slot
    bcopy(srcBuffer, destBuffer, sizeof(struct SaveFile));

    // Write destination data to EEPROM
    write_eeprom_data(&gSaveBuffer.files[destFileIndex],
                      sizeof(gSaveBuffer.files[destFileIndex]));
}

void save_file_do_save(s32 fileIndex) {
    if (gSaveFileModified) {
        // Compute checksum
        add_save_block_signature(&gSaveBuffer.files[fileIndex],
                                 sizeof(gSaveBuffer.files[fileIndex]), SAVE_FILE_MAGIC);

        // Write to EEPROM
        write_eeprom_data(&gSaveBuffer.files[fileIndex], sizeof(gSaveBuffer.files[fileIndex]));

        // Create the xor backup
        xor_save_file_backup(fileIndex);

        gSaveFileModified = FALSE;
    }

    // force overriding the main menu data so AI stuff are always saved
    gMainMenuDataModified = TRUE;
    save_main_menu_data();
}

void save_file_erase(s32 fileIndex) {
    bzero(&gSaveBuffer.files[fileIndex], sizeof(gSaveBuffer.files[fileIndex]));

    gSaveFileModified = TRUE;
    save_file_do_save(fileIndex);
}

//! Needs to be s32 to match on -O2, despite no return value.
BAD_RETURN(s32) save_file_copy(s32 srcFileIndex, s32 destFileIndex) {
    bcopy(&gSaveBuffer.files[srcFileIndex], &gSaveBuffer.files[destFileIndex],
          sizeof(gSaveBuffer.files[destFileIndex]));

    gSaveFileModified = TRUE;
    save_file_do_save(destFileIndex);
}

void save_file_load_all(void) {
    register s32 file;
    s32 validSlots;

    gMainMenuDataModified = FALSE;
    gSaveFileModified = FALSE;

    bzero(&gSaveBuffer, sizeof(gSaveBuffer));
    read_eeprom_data(&gSaveBuffer, sizeof(gSaveBuffer));

    validSlots = verify_save_block_signature(&gSaveBuffer.menuData, sizeof(gSaveBuffer.menuData), MENU_DATA_MAGIC);
    if (validSlots == 0) {
        // too bad, backup slot had to be deleted to make space for AI
        wipe_main_menu_data();
    }

    TRACKER_read_save((u16*)&gSaveBuffer.menuData);

    for (file = 0; file < NUM_SAVE_FILES; file += 2) {
        // Verify the 2 slots, and if only one of them is broken, restore it using the other and the xor data.
        validSlots = verify_save_block_signature(&gSaveBuffer.files[file], sizeof(gSaveBuffer.files[file]), SAVE_FILE_MAGIC);
        validSlots |= verify_save_block_signature(&gSaveBuffer.files[file + 1], sizeof(gSaveBuffer.files[file + 1]), SAVE_FILE_MAGIC) << 1;

        switch (validSlots) {
            case 0: // Neither copy is correct
                save_file_erase(file);
                save_file_erase(file + 1);
                break;
            case 1: // Slot A is correct and slot B is incorrect
                restore_save_file_data(file + 1);
                break;
            case 2: // Slot B is correct and slot A is incorrect
                restore_save_file_data(file);
                break;
            case 3: // Both copies are correct
                xor_save_file_backup(file);
                break;
        }
    }
}

/**
 * Reload the current save file from its backup copy, which is effectively a
 * a cached copy of what has been written to EEPROM.
 * This is used after getting a game over.
 */
void save_file_reload(void) {
    save_file_load_all(); // meh lazy to do more xoring lol

    gMainMenuDataModified = FALSE;
    gSaveFileModified = FALSE;
}

/**
 * Update the current save file after collecting a star or a key.
 * If coin score is greater than the current high score, update it.
 */
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

    if (courseIndex > COURSE_MAX - 1 + 34)
        return;

    starFlag = 1 << starIndex;

    gLastCompletedCourseNum = courseIndex + 1;
    gLastCompletedStarNum = starIndex + 1;

    switch (gCurrLevelNum) {
        case LEVEL_BOWSER_3:
            break;
        default:
            save_file_set_star_flags(fileIndex, courseIndex, starFlag);
            break;
    }
}

s32 save_file_exists(s32 fileIndex) {
    return (gSaveBuffer.files[fileIndex].flags & SAVE_FLAG_FILE_EXISTS) != 0;
}

s32 save_file_get_course_star_count(s32 fileIndex, s32 courseIndex) {
    register s32 count = 0;
    register u8 starFlags = save_file_get_star_flags(fileIndex, courseIndex);

    while (starFlags) {
        if (starFlags & 1) {
            count++;
        }
        starFlags >>= 1;
    }
    return count;
}

s32 save_file_masked_get_course_star_count(register s32 fileIndex, register s32 courseIndex, register u8 mask) {
    register s32 count = 0;
    register u8 starFlags = save_file_get_star_flags(fileIndex, courseIndex) & mask;

    while (starFlags) {
        if (starFlags & 1) {
            count++;
        }
        starFlags >>= 1;
    }
    return count;
}

s32 get_red_star_count(register s32 fileIndex) {
    return save_file_masked_get_course_star_count(fileIndex, (COURSE_MAX - 1) + 15 - 1, 0xF8)
        + save_file_masked_get_course_star_count(fileIndex, (COURSE_MAX - 1) + 15, 0xFF);
}

s32 get_green_star_count(register s32 fileIndex) {
    return save_file_masked_get_course_star_count(fileIndex, (COURSE_MAX - 1) + 15 - 3, 0xC0)
        + save_file_masked_get_course_star_count(fileIndex, (COURSE_MAX - 1) + 15 - 2, 0xFF)
        + save_file_masked_get_course_star_count(fileIndex, (COURSE_MAX - 1) + 15 - 1, 0x07);
}

s32 save_file_get_total_star_count(register s32 fileIndex, register s32 minCourse, register s32 maxCourse) {
    register s32 count = 0;
    // 26 star IDs are reserved for reds and greens
	maxCourse += 15 - 3; // extra yaha stars

    // Get standard course star count.
    for (; minCourse < maxCourse; minCourse++) {
        count += save_file_get_course_star_count(fileIndex, minCourse);
    }

    // Add castle secret star count.
    count += save_file_get_course_star_count(fileIndex, COURSE_NUM_TO_INDEX(COURSE_NONE));
    // Add these 6 stars.
    count += save_file_masked_get_course_star_count(fileIndex, maxCourse, 0x3F);

    // not done yet, 18 extra courses worth of stars post red/green
    minCourse = maxCourse + 4;
    maxCourse = minCourse + 19;
    for (; minCourse < maxCourse; minCourse++) {
        count += save_file_get_course_star_count(fileIndex, minCourse);
    }

    return count;
}

void save_file_set_flags(u32 flags) {
    gSaveBuffer.files[gCurrSaveFileNum - 1].flags |= (flags | SAVE_FLAG_FILE_EXISTS);
    gSaveFileModified = TRUE;
}

void save_file_clear_flags(u32 flags) {
    gSaveBuffer.files[gCurrSaveFileNum - 1].flags &= ~flags;
    gSaveBuffer.files[gCurrSaveFileNum - 1].flags |= SAVE_FLAG_FILE_EXISTS;
    gSaveFileModified = TRUE;
}

u32 save_file_get_flags(void) {
    if (gCurrCreditsEntry != NULL || gCurrDemoInput != NULL) {
        return 0;
    }
    return gSaveBuffer.files[gCurrSaveFileNum - 1].flags;
}

/**
 * Return the bitset of obtained stars in the specified course.
 * If course is COURSE_NONE, return the bitset of obtained castle secret stars.
 */
u32 save_file_get_star_flags(s32 fileIndex, s32 courseIndex) {
    if (courseIndex == COURSE_NUM_TO_INDEX(COURSE_NONE)) {
        return SAVE_FLAG_TO_STAR_FLAG(gSaveBuffer.files[fileIndex].flags);
    }

    return gSaveBuffer.files[fileIndex].courseStars[courseIndex];
}

/**
 * Add to the bitset of obtained stars in the specified course.
 * If course is COURSE_NONE, add to the bitset of obtained castle secret stars.
 */
void save_file_set_star_flags(s32 fileIndex, s32 courseIndex, u32 starFlags) {
    if (courseIndex == COURSE_NUM_TO_INDEX(COURSE_NONE)) {
        gSaveBuffer.files[fileIndex].flags |= STAR_FLAG_TO_SAVE_FLAG(starFlags);
    } else {
        gSaveBuffer.files[fileIndex].courseStars[courseIndex] |= starFlags;
    }

    gSaveBuffer.files[fileIndex].flags |= SAVE_FLAG_FILE_EXISTS;
    gSaveFileModified = TRUE;
}

s32 save_file_get_course_coin_score(s32 fileIndex, s32 courseIndex) {
    return gSaveBuffer.files[fileIndex].courseCoinScores[courseIndex];
}

/**
 * Return TRUE if the cannon is unlocked in the current course.
 */
s32 save_file_is_cannon_unlocked(void) {
    return (gSaveBuffer.files[gCurrSaveFileNum - 1].courseStars[gCurrCourseNum] & (1 << 7)) != 0;
}

/**
 * Sets the cannon status to unlocked in the current course.
 */
void save_file_set_cannon_unlocked(void) {
    gSaveBuffer.files[gCurrSaveFileNum - 1].courseStars[gCurrCourseNum] |= (1 << 7);
    gSaveBuffer.files[gCurrSaveFileNum - 1].flags |= SAVE_FLAG_FILE_EXISTS;
    gSaveFileModified = TRUE;
}

void save_file_set_cap_pos(s16 x, s16 y, s16 z) {
    struct SaveFile *saveFile = &gSaveBuffer.files[gCurrSaveFileNum - 1];

    saveFile->capLevel = gCurrLevelNum;
    saveFile->capArea = gCurrAreaIndex;
    vec3s_set(saveFile->capPos, x, y, z);
    save_file_set_flags(SAVE_FLAG_CAP_ON_GROUND);
}

s32 save_file_get_cap_pos(Vec3s capPos) {
    struct SaveFile *saveFile = &gSaveBuffer.files[gCurrSaveFileNum - 1];
    s32 flags = save_file_get_flags();

    if (saveFile->capLevel == gCurrLevelNum && saveFile->capArea == gCurrAreaIndex
        && (flags & SAVE_FLAG_CAP_ON_GROUND)) {
        vec3s_copy(capPos, saveFile->capPos);
        return TRUE;
    }
    return FALSE;
}

void save_file_set_sound_mode(u16 mode) {
    set_sound_mode(mode);
    gSaveBuffer.menuData.soundMode = mode;

    gMainMenuDataModified = TRUE;
    save_main_menu_data();
}

u16 save_file_get_sound_mode(void) {
    return gSaveBuffer.menuData.soundMode;
}

void save_file_move_cap_to_default_location(void) {
    return;
}


// chris brutal agresion wanted it to be persistent, then temporary...
// this is reset to 0 on init_mario
u16 betaKeyFlags;

// Beta key stuff from Gamma64 (iProgramInCpp)
s32 save_file_get_total_betakey_count(s32 fileIndex)
{
	s16 flag = 1, count = 0, i;
	for (i = 0; i < 8; i++) {
		if (betaKeyFlags & flag)
			count++;
		flag <<= 1;
	}
	return count;
}

s32 save_file_get_betakey_flags(s32 fileIndex)
{
	return betaKeyFlags;
}

/**
 * Update the current save file after collecting a Boo key.
 */
void save_file_collect_beta_key(s16 keyIndex) {
    s32 fileIndex = gCurrSaveFileNum - 1;
	betaKeyFlags |= (1 << keyIndex);
}

