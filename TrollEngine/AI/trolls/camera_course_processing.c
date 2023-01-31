#include <ultra64.h>

#define INCLUDED_FROM_CAMERA_C

#include "prevent_bss_reordering.h"
#include "sm64.h"
#include "game/camera.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "audio/external.h"
#include "game/mario_misc.h"
#include "game/game_init.h"
#include "game/hud.h"
#include "engine/math_util.h"
#include "game/area.h"
#include "engine/surface_collision.h"
#include "engine/behavior_script.h"
#include "game/level_update.h"
#include "game/ingame_menu.h"
#include "game/mario_actions_cutscene.h"
#include "game/spawn_sound.h"
#include "game/behavior_actions.h"
#include "behavior_data.h"
#include "game/object_list_processor.h"
#include "game/paintings.h"
#include "engine/graph_node.h"
#include "level_table.h"

#include "AI/personalization_helpers.h"

#define CBUTTON_MASK (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS)

extern Vec3f sFixedModeBasePosition;
extern struct CameraFOVStatus sFOVState;
extern struct TransitionInfo sModeTransition;
extern struct PlayerGeometry sMarioGeometry;
extern s16 unusedFreeRoamWallYaw;
extern s16 sAvoidYawVel;
extern s16 sCameraYawAfterDoorCutscene;
extern s16 unusedSplinePitch;
extern s16 unusedSplineYaw;
extern struct HandheldShakePoint sHandheldShakeSpline[4];
extern s16 sHandheldShakeMag;
extern f32 sHandheldShakeTimer;
extern f32 sHandheldShakeInc;
extern s16 sHandheldShakePitch;
extern s16 sHandheldShakeYaw;
extern s16 sHandheldShakeRoll;
extern u32 unused8033B30C;
extern u32 unused8033B310;
extern s16 sSelectionFlags;
extern s16 unused8033B316;
extern s16 s2ndRotateFlags;
extern s16 unused8033B31A;
extern s16 sCameraSoundFlags;
extern u16 sCButtonsPressed;
extern s16 sCutsceneDialogID;
extern struct LakituState gLakituState;
extern s16 unused8033B3E8;
extern s16 sAreaYaw;
extern s16 sAreaYawChange;
extern s16 sLakituDist;
extern s16 sLakituPitch;
extern f32 sZoomAmount;
extern s16 sCSideButtonYaw;
extern s16 sBehindMarioSoundTimer;
extern f32 sZeroZoomDist;
extern s16 sCUpCameraPitch;
extern s16 sModeOffsetYaw;
extern s16 sSpiralStairsYawOffset;
extern s16 s8DirModeBaseYaw;
extern s16 s8DirModeYawOffset;
extern f32 sPanDistance;
extern f32 sCannonYOffset;
extern struct ModeTransitionInfo sModeInfo;
extern Vec3f sCastleEntranceOffset;
extern u32 sParTrackIndex;
extern struct ParallelTrackingPoint *sParTrackPath;
extern struct CameraStoredInfo sParTrackTransOff;
extern struct CameraStoredInfo sCameraStoreCUp;
extern struct CameraStoredInfo sCameraStoreCutscene;
extern s16 gCameraMovementFlags;
extern s16 sStatusFlags;
extern struct CutsceneSplinePoint sCurCreditsSplinePos[32];
extern struct CutsceneSplinePoint sCurCreditsSplineFocus[32];
extern s16 sCutsceneSplineSegment;
extern f32 sCutsceneSplineSegmentProgress;
extern s16 unused8033B6E8;
extern s16 sCutsceneShot;
extern s16 gCutsceneTimer;
extern struct CutsceneVariable sCutsceneVars[10];
extern s32 gObjCutsceneDone;
extern u32 gCutsceneObjSpawn;
extern struct Camera *gCamera;

extern s16 sYawSpeed;
extern s32 gCurrLevelArea;
extern struct PlayerCameraState *sMarioCamState;
extern f32 gCameraZoomDist;

extern struct CameraTrigger *sCameraTriggers[LEVEL_COUNT + 1];

s32 is_pos_in_bounds(Vec3f pos, Vec3f center, Vec3f bounds, s16 boundsYaw);
s32 vec3f_compare(Vec3f pos, f32 posX, f32 posY, f32 posZ);
s16 camera_course_processing(register struct Camera *c) {
    s16 level = gCurrLevelNum;
    s16 mode;
    s8 area = gCurrentArea->index;
    // Bounds iterator
    register u32 b;
    // Camera trigger's bounding box
    Vec3f center, bounds;
    u32 insideBounds = FALSE;
    u8 oldMode = c->mode;

    if (c->mode == CAMERA_MODE_C_UP) {
        c->mode = sModeInfo.lastMode;
    }
    check_blocking_area_processing(&c->mode);
    if (level > LEVEL_COUNT + 1) {
        level = LEVEL_COUNT + 1;
    }

    if (sCameraTriggers[level] != NULL) {
        b = 0;

        // Process positional triggers.
        // All triggered events are called, not just the first one.
        while (sCameraTriggers[level][b].event != NULL) {

            // Check only the current area's triggers
            if (sCameraTriggers[level][b].area == area) {
                // Copy the bounding box into center and bounds
				// EDIT: level scale messes with hardcoded positions, so scale the camera trigger center and bounds
                vec3f_scaled_set(center, sCameraTriggers[level][b].centerX,
                                  sCameraTriggers[level][b].centerY,
                                  sCameraTriggers[level][b].centerZ);
                vec3f_scaled_set(bounds, sCameraTriggers[level][b].boundsX,
                                  sCameraTriggers[level][b].boundsY,
                                  sCameraTriggers[level][b].boundsZ);
				// END EDIT

                // Check if Mario is inside the bounds
                if (is_pos_in_bounds(sMarioCamState->pos, center, bounds,
                                                   sCameraTriggers[level][b].boundsYaw) == TRUE) {
                    //! This should be checked before calling is_pos_in_bounds. (It doesn't belong
                    //! outside the while loop because some events disable area processing)
                    if (!(sStatusFlags & CAM_FLAG_BLOCK_AREA_PROCESSING)) {
                        sCameraTriggers[level][b].event(c);
                        insideBounds = TRUE;
                    }
                }
            }

            if ((sCameraTriggers[level])[b].area == -1) {
                // Default triggers are only active if Mario is not already inside another trigger
                if (!insideBounds) {
                    if (!(sStatusFlags & CAM_FLAG_BLOCK_AREA_PROCESSING)) {
                        sCameraTriggers[level][b].event(c);
                    }
                }
            }

            b++;
        }
    }

    // Area-specific camera processing
    if (!(sStatusFlags & CAM_FLAG_BLOCK_AREA_PROCESSING)) {
        switch (gCurrLevelArea) {
            case AREA_WF:
                if (sMarioCamState->action == ACT_RIDING_HOOT) {
                    transition_to_camera_mode(c, CAMERA_MODE_SLIDE_HOOT, 60);
                } else {
                    switch (sMarioGeometry.currFloorType) {
                        case SURFACE_CAMERA_8_DIR:
                            transition_to_camera_mode(c, CAMERA_MODE_8_DIRECTIONS, 90);
                            s8DirModeBaseYaw = DEGREES(90);
                            break;

                        case SURFACE_BOSS_FIGHT_CAMERA:
                            if (gCurrActNum == 1) {
                                set_camera_mode_boss_fight(c);
                            } else {
                                set_camera_mode_radial(c, 60);
                            }
                            break;
                        default:
                            set_camera_mode_radial(c, 60);
                    }
                }
                break;

            case AREA_BBH:
                // if camera is fixed at bbh_room_13_balcony_camera (but as floats)
                if (vec3f_compare(sFixedModeBasePosition, 210.f, 420.f, 3109.f) == TRUE) {
                    if (sMarioCamState->pos[1] < 1800.f) {
                        transition_to_camera_mode(c, CAMERA_MODE_CLOSE, 30);
                    }
                }
                break;

            case AREA_SSL_PYRAMID:
                set_mode_if_not_set_by_surface(c, CAMERA_MODE_OUTWARD_RADIAL);
                break;

            case AREA_SSL_OUTSIDE:
                set_mode_if_not_set_by_surface(c, CAMERA_MODE_RADIAL);
                break;

            case AREA_THI_HUGE:
                break;

            case AREA_THI_TINY:
                surface_type_modes_thi(c);
                break;

            case AREA_TTC:
                set_mode_if_not_set_by_surface(c, CAMERA_MODE_OUTWARD_RADIAL);
                break;

            case AREA_BOB:
                if (set_mode_if_not_set_by_surface(c, CAMERA_MODE_NONE) == 0) {
                    if (sMarioGeometry.currFloorType == SURFACE_BOSS_FIGHT_CAMERA) {
                        set_camera_mode_boss_fight(c);
                    } else {
                        if (c->mode == CAMERA_MODE_CLOSE) {
                            transition_to_camera_mode(c, CAMERA_MODE_RADIAL, 60);
                        } else {
                            set_camera_mode_radial(c, 60);
                        }
                    }
                }
                break;

            case AREA_WDW_MAIN:
                switch (sMarioGeometry.currFloorType) {
                    case SURFACE_INSTANT_WARP_1B:
                        c->defMode = CAMERA_MODE_RADIAL;
                        break;
                }
                break;

            case AREA_WDW_TOWN:
                switch (sMarioGeometry.currFloorType) {
                    case SURFACE_INSTANT_WARP_1C:
                        c->defMode = CAMERA_MODE_CLOSE;
                        break;
                }
                break;

            case AREA_DDD_WHIRLPOOL:
                //! @bug this does nothing
                gLakituState.defMode = CAMERA_MODE_OUTWARD_RADIAL;
                break;

            case AREA_DDD_SUB:
                if ((c->mode != CAMERA_MODE_BEHIND_MARIO)
                    && (c->mode != CAMERA_MODE_WATER_SURFACE)) {
                    if (((sMarioCamState->action & ACT_FLAG_ON_POLE) != 0)
                        || (sMarioGeometry.currFloorHeight > 800.f)) {
                        transition_to_camera_mode(c, CAMERA_MODE_8_DIRECTIONS, 60);

                    } else {
                        if (sMarioCamState->pos[1] < 800.f) {
                            transition_to_camera_mode(c, CAMERA_MODE_FREE_ROAM, 60);
                        }
                    }
                }
                //! @bug this does nothing
                gLakituState.defMode = CAMERA_MODE_FREE_ROAM;
                break;
        }
    }

    sStatusFlags &= ~CAM_FLAG_BLOCK_AREA_PROCESSING;
    if (oldMode == CAMERA_MODE_C_UP) {
        sModeInfo.lastMode = c->mode;
        c->mode = oldMode;
    }
    mode = c->mode;
    return mode;
}
