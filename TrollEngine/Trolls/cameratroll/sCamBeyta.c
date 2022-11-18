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

extern s16 sStatusFlags;
extern Vec3f sFixedModeBasePosition;

BAD_RETURN(s32) cam_beyta_castle_fixed(struct Camera *c) {
    if (c->mode != CAMERA_MODE_FIXED) {
        sStatusFlags &= ~CAM_FLAG_SMOOTH_MOVEMENT;
        vec3f_set(sFixedModeBasePosition, 660.f, 160.f, 800.f);
        c->mode = CAMERA_MODE_FIXED;
    }
}
BAD_RETURN(s32) cam_mark_door_special(struct Camera *c) {
    c->doorStatus = DOOR_LEAVING_SPECIAL;
}

#define NULL_TRIGGER                                                                                    \
    { 0, NULL, 0, 0, 0, 0, 0, 0, 0 }
#define ALL_LOBBIES(a,b,c,d,e,f,g,h)                                                                 \
    { 2, a, b, c, d, e, f, g, h },{ 3, a, b, c, d, e, f, g, h },{ 4, a, b, c, d, e, f, g, h },{ 8, a, b, c, d, e, f, g, h }

// so doors work correctly
struct CameraTrigger sCamBeytaLobby[] = {
    // upper double door
    ALL_LOBBIES(cam_beyta_castle_fixed, 0, 430, -650, 140, 100, 100, 0),
    ALL_LOBBIES(cam_mark_door_special, 0, 430, -800, 300, 100, 100, 0),
    // floor level doors
    ALL_LOBBIES(cam_beyta_castle_fixed, 640, 0, -520, 340, 100, 190, 0),
    ALL_LOBBIES(cam_beyta_castle_fixed, -640, 0, -520, 340, 100, 190, 0),
    //  painting rooms
    ALL_LOBBIES(cam_mark_door_special, 0, 0, -800, 1680, 100, 100, 0),
    //  courtyard
    ALL_LOBBIES(cam_mark_door_special, 0, 0, -650, 350, 100, 100, 0),
    // side doors
    ALL_LOBBIES(cam_beyta_castle_fixed, 0, 210, 12, 1680, 100, 100, 0),
    ALL_LOBBIES(cam_mark_door_special, -1800, 210, 12, 100, 100, 150, 0),
    ALL_LOBBIES(cam_mark_door_special, 1800, 210, 12, 100, 100, 150, 0),
    NULL_TRIGGER
};
