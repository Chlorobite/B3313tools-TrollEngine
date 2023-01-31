#include <PR/ultratypes.h>

#include "sm64.h"
#include "game/ingame_menu.h"
#include "engine/graph_node.h"
#include "engine/behavior_script.h"
#include "behavior_data.h"
#include "game/memory.h"
#include "game/object_helpers.h"
#include "game/macro_special_objects.h"
#include "engine/surface_collision.h"
#include "game/mario.h"
#include "game/object_list_processor.h"
#include "engine/surface_load.h"

#include "AI/personalization_helpers.h"


void load_object_collision_model(void) {
    s16 vertexData[600];

    s16 *collisionData = gCurrentObject->collisionData;
    register f32 marioDist = gCurrentObject->oDistanceToMario;
    register f32 tangibleDist = gCurrentObject->oCollisionDistance;

    // On an object's first frame, the distance is set to 19000.0f.
    // If the distance hasn't been updated, update it now.
    if (gCurrentObject->oDistanceToMario == 19000.0f) {
        marioDist = dist_between_objects(gCurrentObject, gMarioObject);
    }

    // If the object collision is supposed to be loaded more than the
    // drawing distance of 4000, extend the drawing range.
    if (gCurrentObject->oCollisionDistance > 4000.0f) {
        gCurrentObject->oDrawingDistance = gCurrentObject->oCollisionDistance;
    }
    
	// ADD: scale distance by object scale (not doing this was causing issues mainly with level scale)
    tangibleDist *= (gCurrentObject->header.gfx.scale[0] + gCurrentObject->header.gfx.scale[1] + gCurrentObject->header.gfx.scale[2]) / 3.0f;
	// END ADD

    // Update if no Time Stop, in range, and in the current room.
    if (!(gTimeStopState & TIME_STOP_ACTIVE) && marioDist < tangibleDist
        && !(gCurrentObject->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM)) {
        collisionData++;
        transform_object_vertices(&collisionData, vertexData);

        // TERRAIN_LOAD_CONTINUE acts as an "end" to the terrain data.
        while (*collisionData != TERRAIN_LOAD_CONTINUE) {
            load_object_surfaces(&collisionData, vertexData);
        }
    }

    if (marioDist < gCurrentObject->oDrawingDistance) {
        gCurrentObject->header.gfx.node.flags |= GRAPH_RENDER_ACTIVE;
    } else {
        gCurrentObject->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
    }
}
