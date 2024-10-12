#include <PR/ultratypes.h>

#include "sm64.h"
#include "level_update.h"
#include "memory.h"
#include "engine/math_util.h"
#include "area.h"
#include "save_file.h"
#include "sound_init.h"
#include "engine/surface_collision.h"
#include "interaction.h"
#include "mario.h"
#include "mario_step.h"
#include "camera.h"
#include "audio/external.h"
#include "behavior_data.h"
#include "level_table.h"
#include "rumble_init.h"

#define MIN_SWIM_STRENGTH 160
#define MIN_SWIM_SPEED 16.0f

extern s16 sWasAtSurface;
extern s16 sSwimStrength;
extern s16 sWaterCurrentSpeeds[4];

extern s16 sBobTimer;
extern s16 sBobIncrement;
extern f32 sBobHeight;
