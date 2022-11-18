#include <ultra64.h>

#define INCLUDED_FROM_CAMERA_C

#include "prevent_bss_reordering.h"
#include "sm64.h"
#include "camera.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "audio/external.h"
#include "mario_misc.h"
#include "game_init.h"
#include "hud.h"
#include "engine/math_util.h"
#include "area.h"
#include "engine/surface_collision.h"
#include "engine/behavior_script.h"
#include "level_update.h"
#include "ingame_menu.h"
#include "mario_actions_cutscene.h"
#include "save_file.h"
#include "object_helpers.h"
#include "print.h"
#include "spawn_sound.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "object_list_processor.h"
#include "paintings.h"
#include "engine/graph_node.h"
#include "level_table.h"

#define CBUTTON_MASK (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS)


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

void lakitu_zoom(f32 rangeDist, s16 rangePitch);
