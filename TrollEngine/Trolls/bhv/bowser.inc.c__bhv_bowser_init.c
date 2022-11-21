#include "bhv_headers.h"

extern void (*sBowserActions[])(void);
extern struct SoundState sBowserSoundStates[];
extern s8 sBowserRainbowLight[];
extern s8 sBowserHealth[];

void bhv_bowser_init(void) {
    register struct Object *_o = o;
    register s32 level = _o->oBehParams2ndByte;
    if (level > 2) level = 2;
    // Set "reaction" value
    // It goes true when Bowser is a non-walking state
    _o->oBowserIsReacting = TRUE;
    // Set no transparency opacity
    _o->oOpacity = 255;
    _o->oBowserTargetOpacity = 255;
    // Set health and rainbow light depending of the level
    _o->oBowserRainbowLight = sBowserRainbowLight[level];
    _o->oHealth = sBowserHealth[level];
    // Start camera event, this event is not defined so maybe
    // the "start arena" cutscene was originally called this way
    cur_obj_start_cam_event(_o, CAM_EVENT_BOWSER_INIT);
    _o->oAction = BOWSER_ACT_WAIT;
    // Set eyes status
    _o->oBowserEyesTimer = 0;
    _o->oBowserEyesShut = FALSE;
}

