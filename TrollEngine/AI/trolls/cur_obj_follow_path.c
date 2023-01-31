#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "debug.h"
#include "dialog_ids.h"
#include "engine/behavior_script.h"
#include "engine/geo_layout.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "game_init.h"
#include "helper_macros.h"
#include "ingame_menu.h"
#include "interaction.h"
#include "level_table.h"
#include "level_update.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "rendering_graph_node.h"
#include "spawn_object.h"
#include "spawn_sound.h"

#include "AI/stats_tracking.h"
#include "AI/personalization_helpers.h"


#define o gCurrentObject



s32 cur_obj_follow_path(UNUSED s32 unusedArg) {
    struct Waypoint *startWaypoint;
    struct Waypoint *lastWaypoint;
    struct Waypoint *targetWaypoint;
    register f32 prevToNextX, prevToNextY, prevToNextZ;
    register f32 objToNextXZ;
    register f32 objToNextX, objToNextY, objToNextZ;

    if (o->oPathedPrevWaypointFlags == 0) {
        o->oPathedPrevWaypoint = o->oPathedStartWaypoint;
        o->oPathedPrevWaypointFlags = WAYPOINT_FLAGS_INITIALIZED;
    }

    startWaypoint = o->oPathedStartWaypoint;
    lastWaypoint = o->oPathedPrevWaypoint;

    if ((lastWaypoint + 1)->flags != WAYPOINT_FLAGS_END) {
        targetWaypoint = lastWaypoint + 1;
    } else {
        targetWaypoint = startWaypoint;
    }

    o->oPathedPrevWaypointFlags = lastWaypoint->flags | WAYPOINT_FLAGS_INITIALIZED;

	// EDIT: level scale messes with hardcoded positions, so scale the path positions
    prevToNextX = (targetWaypoint->pos[0] - lastWaypoint->pos[0]) * levelScaleH;
    prevToNextY = (targetWaypoint->pos[1] - lastWaypoint->pos[1]) * levelScaleV;
    prevToNextZ = (targetWaypoint->pos[2] - lastWaypoint->pos[2]) * levelScaleH;

    objToNextX = targetWaypoint->pos[0] * levelScaleH - o->oPosX;
    objToNextY = targetWaypoint->pos[1] * levelScaleV - o->oPosY;
    objToNextZ = targetWaypoint->pos[2] * levelScaleH - o->oPosZ;
	// END EDIT
    objToNextXZ = sqrtf(sqr(objToNextX) + sqr(objToNextZ));

    o->oPathedTargetYaw = atan2s(objToNextZ, objToNextX);
    o->oPathedTargetPitch = atan2s(objToNextXZ, -objToNextY);

    // If dot(prevToNext, objToNext) <= 0 (i.e. reached other side of target waypoint)
    if (prevToNextX * objToNextX + prevToNextY * objToNextY + prevToNextZ * objToNextZ <= 0.0f) {
        o->oPathedPrevWaypoint = targetWaypoint;
        if ((targetWaypoint + 1)->flags == WAYPOINT_FLAGS_END) {
            return PATH_REACHED_END;
        } else {
            return PATH_REACHED_WAYPOINT;
        }
    }

    return PATH_NONE;
}
