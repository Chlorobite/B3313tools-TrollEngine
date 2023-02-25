#include <ultra64.h>
#include <PR/rcp.h>

#include "sm64.h"
#include "gfx_dimensions.h"
#include "audio/external.h"
#include "buffers/buffers.h"
#include "buffers/gfx_output_buffer.h"
#include "buffers/framebuffers.h"
#include "buffers/zbuffer.h"
#include "engine/level_script.h"
#include "game_init.h"
#include "main.h"
#include "memory.h"
#include "profiler.h"
#include "save_file.h"
#include "seq_ids.h"
#include "sound_init.h"
#include "print.h"
#include "segment2.h"
#include "segment_symbols.h"
#include "rumble_init.h"

#include "src/engine/behavior_script.h"


#define SECONDS_PER_CYCLE 0.00000002133f
// slightly higher than 1/30 to account for slight variations
#define FRAME_SKIP_THRESHOLD 0.035f
// in case we overshoot
#define FRAME_SKIP_ERROR 0.001f
s32 render_frame_count = 1;
s32 last_render_frame_count = 1;

static OSTime _gLastOSTime = 0;
static float _gFrameTime = 0.0f;
float _gLastFrameTime = 0.0f;

static void _calculate_frameTime_from_OSTime(OSTime diff) {
    _gLastFrameTime = diff * SECONDS_PER_CYCLE;
    _gFrameTime += diff * SECONDS_PER_CYCLE;
}

void get_frame_count() {
    OSTime newTime = osGetTime();
    _calculate_frameTime_from_OSTime(newTime - _gLastOSTime);
    
    render_frame_count = 1 + (s32)(_gFrameTime / FRAME_SKIP_THRESHOLD);
    
    if (render_frame_count > 3) {
        // no way the game's actually running sub 10 fps, there was a loading screen
        render_frame_count = 1;
        _gFrameTime = 0.0f;
    }
    else {
        _gFrameTime -= FRAME_SKIP_THRESHOLD * render_frame_count;
        if (_gFrameTime < 0.0f && render_frame_count == 1) _gFrameTime += FRAME_SKIP_ERROR;
    }
    
    // prevent weird spikes
    if (_gLastFrameTime <= FRAME_SKIP_THRESHOLD) {
        render_frame_count = 1;
    }
    
    _gLastOSTime = newTime;
}


s32 troll_lvl_init_or_update(s16 initOrUpdate) {
    register s32 result = 0;
    register s32 i = 0;
    register s32 j = 0;
    
    for (i = 0; i < render_frame_count; i++) {
        if (i == 1) {
            // prevent double taps as a result of inputs not being polled again
            for (j = 0; j < 2; j++) {
                struct Controller *controller = &gControllers[j];

                if (controller->controllerData != NULL) {
                    controller->buttonPressed = 0;
                }
            }
        }
        
        switch (initOrUpdate) {
            case 0:
                render_frame_count = i + 1;
                return init_level(); // run only one frame on init to prevent issues / funny spike
            case 1:
                result = update_level();
                if (result) {
                    // we're leaving, stop processing
                    render_frame_count = i + 1;
                    return result;
                }
        }
    }

    stats_tracking_debug_display();

    return result;
}
