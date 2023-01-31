#include <PR/ultratypes.h>

#include "area.h"
#include "engine/math_util.h"
#include "game_init.h"
#include "gfx_dimensions.h"
#include "main.h"
#include "memory.h"
#include "print.h"
#include "rendering_graph_node.h"
#include "shadow.h"
#include "sm64.h"

extern s16 gMatStackIndex;
extern Mat4 gMatStack[32];
extern Mtx *gMatStackFixed[32];

/**
 * Animation nodes have state in global variables, so this struct captures
 * the animation state so a 'context switch' can be made when rendering the
 * held object.
 */
struct GeoAnimState {
    /*0x00*/ u8 type;
    /*0x01*/ u8 enabled;
    /*0x02*/ s16 frame;
    /*0x04*/ f32 translationMultiplier;
    /*0x08*/ u16 *attribute;
    /*0x0C*/ s16 *data;
};

// For some reason, this is a GeoAnimState struct, but the current state consists
// of separate global variables. It won't match EU otherwise.
extern struct GeoAnimState gGeoTempState;

extern u8 gCurrAnimType;
extern u8 gCurrAnimEnabled;
extern s16 gCurrAnimFrame;
extern f32 gCurrAnimTranslationMultiplier;
extern u16 *gCurrAnimAttribute;
extern s16 *gCurrAnimData;

extern s32 render_frame_count;

void geo_set_animation_globals(register struct AnimInfo *node, s32 hasAnimation) {
    register struct Animation *anim = node->curAnim;

    if (hasAnimation) {
		// EDIT: frameskip engine, call geo_update_animation_frame for each frame we skipped
        register s32 __i;
        
        for (__i = 0; __i < render_frame_count; __i++)
            node->animFrame = geo_update_animation_frame(node, &node->animFrameAccelAssist);
        if (node->animFrame < 0)
            node->animFrame = 0;
		// END EDIT
    }
    node->animTimer = gAreaUpdateCounter;
    gCurrAnimType = ANIM_TYPE_TRANSLATION;
    if (anim->flags & ANIM_FLAG_HOR_TRANS) {
        gCurrAnimType = ANIM_TYPE_VERTICAL_TRANSLATION;
    } else if (anim->flags & ANIM_FLAG_VERT_TRANS) {
        gCurrAnimType = ANIM_TYPE_LATERAL_TRANSLATION;
    } else if (anim->flags & ANIM_FLAG_6) {
        gCurrAnimType = ANIM_TYPE_NO_TRANSLATION;
    }

    gCurrAnimFrame = node->animFrame;
    gCurrAnimEnabled = (anim->flags & ANIM_FLAG_5) == 0;
    gCurrAnimAttribute = segmented_to_virtual((void *) anim->index);
    gCurrAnimData = segmented_to_virtual((void *) anim->values);

    if (anim->animYTransDivisor == 0) {
        gCurrAnimTranslationMultiplier = 1.0f;
    } else {
        gCurrAnimTranslationMultiplier = (f32) node->animYTrans / (f32) anim->animYTransDivisor;
    }
}
