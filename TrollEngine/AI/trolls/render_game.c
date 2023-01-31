#include <PR/ultratypes.h>

#include "prevent_bss_reordering.h"
#include "area.h"
#include "sm64.h"
#include "gfx_dimensions.h"
#include "behavior_data.h"
#include "game_init.h"
#include "object_list_processor.h"
#include "engine/surface_load.h"
#include "ingame_menu.h"
#include "screen_transition.h"
#include "mario.h"
#include "mario_actions_cutscene.h"
#include "print.h"
#include "hud.h"
#include "audio/external.h"
#include "area.h"
#include "rendering_graph_node.h"
#include "level_update.h"
#include "engine/geo_layout.h"
#include "save_file.h"
#include "level_table.h"
#include "dialog_ids.h"

extern struct SpawnInfo gPlayerSpawnInfos[1];
extern struct GraphNode *D_8033A160[0x100];

extern struct WarpTransition gWarpTransition;

extern s16 gCurrCourseNum;
extern s16 gCurrActNum;
extern s16 gCurrAreaIndex;
extern s16 gSavedCourseNum;
extern s16 gMenuOptSelectIndex;
extern s16 gSaveOptSelectIndex;

extern struct SpawnInfo *gMarioSpawnInfo;
extern struct GraphNode **gLoadedGraphNodes;
extern struct Area *gAreas;
extern struct Area *gCurrentArea;
extern struct CreditsEntry *gCurrCreditsEntry;
extern Vp *D_8032CE74;
extern Vp *D_8032CE78;
extern s16 gWarpTransDelay;
extern u32 gFBSetColor;
extern u32 gWarpTransFBSetColor;
extern u8 gWarpTransRed;
extern u8 gWarpTransGreen;
extern u8 gWarpTransBlue;
extern s16 gCurrSaveFileNum;
extern s16 gCurrLevelNum;

extern Vp *D_8032CF00;

extern s32 render_frame_count;


// bee moment
#define B_BORDER_HEIGHT 0
void render_game(void) {
    if (gCurrentArea != NULL && !gWarpTransition.pauseRendering) {
        geo_process_root(gCurrentArea->unk04, D_8032CE74, D_8032CE78, gFBSetColor);

        gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_8032CF00));

        gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, B_BORDER_HEIGHT, SCREEN_WIDTH,
                      SCREEN_HEIGHT - B_BORDER_HEIGHT);
        render_hud();

        gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        render_text_labels();
        do_cutscene_handler();
        print_displaying_credits_entry();

        gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, B_BORDER_HEIGHT, SCREEN_WIDTH,
                      SCREEN_HEIGHT - B_BORDER_HEIGHT);
        gMenuOptSelectIndex = render_menus_and_dialogs();
        if (gMenuOptSelectIndex != MENU_OPT_NONE) {
            gSaveOptSelectIndex = gMenuOptSelectIndex;
        }

        if (D_8032CE78 != NULL) {
            make_viewport_clip_rect(D_8032CE78);
        } else {
            gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, B_BORDER_HEIGHT, SCREEN_WIDTH,
                          SCREEN_HEIGHT - B_BORDER_HEIGHT);
        }

        if (gWarpTransition.isActive) {
            if (gWarpTransDelay <= 0) {
                gWarpTransition.isActive = !render_screen_transition(0, gWarpTransition.type, gWarpTransition.time,
                                                                     &gWarpTransition.data);
                if (!gWarpTransition.isActive) {
                    if (gWarpTransition.type & 1) {
                        gWarpTransition.pauseRendering = TRUE;
                    } else {
                        set_warp_transition_rgb(0, 0, 0);
                    }
                }
            } else {
                gWarpTransDelay -= render_frame_count; // EDIT: frameskip engine
            }
        }
    } else {
        render_text_labels();
        if (D_8032CE78 != NULL) {
            clear_viewport(D_8032CE78, gWarpTransFBSetColor);
        } else {
            clear_framebuffer(gWarpTransFBSetColor);
        }
    }

    D_8032CE74 = NULL;
    D_8032CE78 = NULL;
}
