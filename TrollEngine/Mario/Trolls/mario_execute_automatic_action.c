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


s32 act_holding_pole(struct MarioState *m);
s32 act_grab_pole_slow(struct MarioState *m);
s32 act_grab_pole_fast(struct MarioState *m);
s32 act_climbing_pole(struct MarioState *m);
s32 act_top_of_pole_transition(struct MarioState *m);
s32 act_top_of_pole(struct MarioState *m);
s32 act_start_hanging(struct MarioState *m);
s32 act_hanging(struct MarioState *m);
s32 act_hang_moving(struct MarioState *m);
s32 act_ledge_grab(struct MarioState *m);
s32 act_ledge_climb_slow(struct MarioState *m);
s32 act_ledge_climb_down(struct MarioState *m);
s32 act_ledge_climb_fast(struct MarioState *m);
s32 act_grabbed(struct MarioState *m);
s32 act_in_cannon(struct MarioState *m);
s32 act_tornado_twirling(struct MarioState *m);
s32 act_climbing_wall(struct MarioState *m);

s32 mario_execute_automatic_action(struct MarioState *m) {
    s32 cancel;

    if (check_common_automatic_cancels(m)) {
        return TRUE;
    }

    m->quicksandDepth = 0.0f;

    /* clang-format off */
    switch (m->action) {
        case ACT_HOLDING_POLE:           cancel = act_holding_pole(m);           break;
        case ACT_GRAB_POLE_SLOW:         cancel = act_grab_pole_slow(m);         break;
        case ACT_GRAB_POLE_FAST:         cancel = act_grab_pole_fast(m);         break;
        case ACT_CLIMBING_POLE:          cancel = act_climbing_pole(m);          break;
        case ACT_TOP_OF_POLE_TRANSITION: cancel = act_top_of_pole_transition(m); break;
        case ACT_TOP_OF_POLE:            cancel = act_top_of_pole(m);            break;
        case ACT_START_HANGING:          cancel = act_start_hanging(m);          break;
        case ACT_HANGING:                cancel = act_hanging(m);                break;
        case ACT_HANG_MOVING:            cancel = act_hang_moving(m);            break;
        case ACT_LEDGE_GRAB:             cancel = act_ledge_grab(m);             break;
        case ACT_LEDGE_CLIMB_SLOW_1:     cancel = act_ledge_climb_slow(m);       break;
        case ACT_LEDGE_CLIMB_SLOW_2:     cancel = act_ledge_climb_slow(m);       break;
        case ACT_LEDGE_CLIMB_DOWN:       cancel = act_ledge_climb_down(m);       break;
        case ACT_LEDGE_CLIMB_FAST:       cancel = act_ledge_climb_fast(m);       break;
        case ACT_GRABBED:                cancel = act_grabbed(m);                break;
        case ACT_IN_CANNON:              cancel = act_in_cannon(m);              break;
        case ACT_TORNADO_TWIRLING:       cancel = act_tornado_twirling(m);       break;
        case ACT_CLIMBING_WALL:          cancel = act_climbing_wall(m);          break;
    }
    /* clang-format on */

    return cancel;
}
