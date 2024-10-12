#include <PR/ultratypes.h>

#include "sm64.h"
#include "behavior_data.h"
#include "mario_actions_automatic.h"
#include "audio/external.h"
#include "area.h"
#include "mario.h"
#include "mario_step.h"
#include "engine/math_util.h"
#include "memory.h"
#include "engine/graph_node.h"
#include "save_file.h"
#include "engine/surface_collision.h"
#include "interaction.h"
#include "camera.h"
#include "level_table.h"
#include "rumble_init.h"

#define POLE_NONE          0
#define POLE_TOUCHED_FLOOR 1
#define POLE_FELL_OFF      2

#define HANG_NONE            0
#define HANG_HIT_CEIL_OR_OOB 1
#define HANG_LEFT_CEIL       2
