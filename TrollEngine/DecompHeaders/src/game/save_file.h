#ifndef SAVE_FILE_H
#define SAVE_FILE_H

#include <PR/ultratypes.h>

#include "types.h"
#include "area.h"

#include "course_table.h"

#define EEPROM_SIZE 0x200
#define NUM_SAVE_FILES 4

struct SaveBlockSignature {
    u16 magic;
    u16 chksum;
};

struct SaveFile {
    // Location of lost cap.
    // Note: the coordinates get set, but are never actually used, since the
    // cap can always be found in a fixed spot within the course
    u8 capLevel;
    u8 capArea;
    Vec3s capPos;

    u32 flags;

    // Star flags for each course.
    // The most significant bit of the byte *following* each course is set if the
    // cannon is open.
    u8 courseStars[COURSE_COUNT];

    u8 courseCoinScores[COURSE_STAGES_COUNT];

    u8 extraStars[18];

    u16 keyFlags;

    struct SaveBlockSignature signature;
};

enum SaveFileIndex {
    SAVE_FILE_A,
    SAVE_FILE_B,
    SAVE_FILE_C,
    SAVE_FILE_D
};

struct MainMenuSaveData {
    u16 soundMode;

    u8 aiData[50];

    struct SaveBlockSignature signature;
};

struct SaveBuffer {
    struct SaveFile files[NUM_SAVE_FILES];
    struct SaveFile file_backups[NUM_SAVE_FILES / 2];
    struct MainMenuSaveData menuData;
};

extern u8 gLastCompletedCourseNum;
extern u8 gLastCompletedStarNum;
extern u8 gGotFileCoinHiScore;
extern u8 gCurrCourseStarFlags;
extern u8 gSpecialTripleJump;
extern s8 gLevelToCourseNumTable[];

extern s32 debug_save_file_status;

// game progress flags
#define SAVE_FLAG_FILE_EXISTS            /* 0x00000001 */ (1 << 0)
#define SAVE_FLAG_HAVE_WING_CAP          /* 0x00000002 */ (1 << 1)
#define SAVE_FLAG_HAVE_METAL_CAP         /* 0x00000004 */ (1 << 2)
#define SAVE_FLAG_HAVE_VANISH_CAP        /* 0x00000008 */ (1 << 3)
#define SAVE_FLAG_HAVE_KEY_1             /* 0x00000010 */ (1 << 4)
#define SAVE_FLAG_HAVE_KEY_2             /* 0x00000020 */ (1 << 5)
#define SAVE_FLAG_UNLOCKED_BASEMENT_DOOR /* 0x00000040 */ (1 << 6)
#define SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR /* 0x00000080 */ (1 << 7)
#define SAVE_FLAG_DDD_MOVED_BACK         /* 0x00000100 */ (1 << 8)
#define SAVE_FLAG_MOAT_DRAINED           /* 0x00000200 */ (1 << 9)
#define SAVE_FLAG_UNLOCKED_PSS_DOOR      /* 0x00000400 */ (1 << 10)
#define SAVE_FLAG_UNLOCKED_WF_DOOR       /* 0x00000800 */ (1 << 11)
#define SAVE_FLAG_UNLOCKED_CCM_DOOR      /* 0x00001000 */ (1 << 12)
#define SAVE_FLAG_UNLOCKED_JRB_DOOR      /* 0x00002000 */ (1 << 13)
#define SAVE_FLAG_UNLOCKED_BITDW_DOOR    /* 0x00004000 */ (1 << 14)
#define SAVE_FLAG_UNLOCKED_BITFS_DOOR    /* 0x00008000 */ (1 << 15)
#define SAVE_FLAG_CAP_ON_GROUND          /* 0x00010000 */ (1 << 16)
#define SAVE_FLAG_CAP_ON_KLEPTO          /* 0x00020000 */ (1 << 17)
#define SAVE_FLAG_CAP_ON_UKIKI           /* 0x00040000 */ (1 << 18)
#define SAVE_FLAG_CAP_ON_MR_BLIZZARD     /* 0x00080000 */ (1 << 19)
#define SAVE_FLAG_UNLOCKED_50_STAR_DOOR  /* 0x00100000 */ (1 << 20)
#define SAVE_FLAG_YS_DISABLE_FIXED_WARPS /* 0x00200000 */ (1 << 21)
#define SAVE_FLAG_YS_POKEMON_MOON_MODE   /* 0x00400000 */ (1 << 22)
#define SAVE_FLAG_COLLECTED_TOAD_STAR_1  /* 0x01000000 */ (1 << 24)
#define SAVE_FLAG_COLLECTED_TOAD_STAR_2  /* 0x02000000 */ (1 << 25)
#define SAVE_FLAG_COLLECTED_TOAD_STAR_3  /* 0x04000000 */ (1 << 26)
#define SAVE_FLAG_COLLECTED_MIPS_STAR_1  /* 0x08000000 */ (1 << 27)
#define SAVE_FLAG_COLLECTED_MIPS_STAR_2  /* 0x10000000 */ (1 << 28)

#define SAVE_FLAG_TO_STAR_FLAG(cmd) ((cmd) >> 24)
#define STAR_FLAG_TO_SAVE_FLAG(cmd) ((cmd) << 24)

// Variable for setting a warp checkpoint.

// possibly a WarpDest struct where arg is a union. TODO: Check?
struct WarpCheckpoint {
    /*0x00*/ u8 actNum;
    /*0x01*/ u8 courseNum;
    /*0x02*/ u8 levelID;
    /*0x03*/ u8 areaNum;
    /*0x04*/ u8 warpNode;
};

extern struct WarpCheckpoint gWarpCheckpoint;

extern s8 gMainMenuDataModified;
extern s8 gSaveFileModified;

void save_file_do_save(s32 fileIndex);
void save_file_erase(s32 fileIndex);
BAD_RETURN(s32) save_file_copy(s32 srcFileIndex, s32 destFileIndex);
void save_file_load_all(void);
void save_file_reload(void);
void save_file_collect_star_or_key(s16 coinScore, s16 starIndex);
s32 save_file_exists(s32 fileIndex);
s32 save_file_get_course_star_count(s32 fileIndex, s32 courseIndex);
s32 save_file_masked_get_course_star_count(register s32 fileIndex, register s32 courseIndex, register u8 mask);
s32 get_red_star_count(register s32 fileIndex);
s32 get_green_star_count(register s32 fileIndex);
s32 save_file_get_total_star_count(s32 fileIndex, s32 minCourse, s32 maxCourse);
void save_file_set_flags(u32 flags);
void save_file_clear_flags(u32 flags);
u32 save_file_get_flags(void);
u32 save_file_get_star_flags(s32 fileIndex, s32 courseIndex);
void save_file_set_star_flags(s32 fileIndex, s32 courseIndex, u32 starFlags);
s32 save_file_is_cannon_unlocked(void);
void save_file_set_cannon_unlocked(void);
void save_file_set_cap_pos(s16 x, s16 y, s16 z);
s32 save_file_get_cap_pos(Vec3s capPos);
void save_file_set_sound_mode(u16 mode);
u16 save_file_get_sound_mode(void);
void save_file_move_cap_to_default_location(void);

void disable_warp_checkpoint(void);
void check_if_should_set_warp_checkpoint(struct WarpNode *warpNode);
s32 check_warp_checkpoint(struct WarpNode *warpNode);

#endif // SAVE_FILE_H
