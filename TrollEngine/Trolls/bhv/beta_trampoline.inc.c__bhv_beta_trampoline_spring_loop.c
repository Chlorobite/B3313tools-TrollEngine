#include "bhv_headers.h"


void bhv_beta_trampoline_spring_loop(void) {
    f32 yDisplacement;

    // Update to be 75 units under the trampoline top
    obj_copy_pos_and_angle(o, o->parentObj);
    obj_copy_graph_y_offset(o, o->parentObj);
    o->oPosY -= 75.0f;

    yDisplacement = o->oPosY - o->oHomeY;

    // Scale the spring
    obj_scale_xyz(o, 1.0f, 1.0f + yDisplacement / 150.0f, 1.0f);
}

f32 g_trampolineSinkDegree = 0;
struct Object* g_currentTrampoline = NULL;

void SetTrampolineSinkDegree(struct Object* trampoline, f32 f) {
    g_currentTrampoline = trampoline;
    if (f <= 1.f) {
        g_currentTrampoline = NULL;
        g_trampolineSinkDegree = 0;
    }
    else 
        g_trampolineSinkDegree = f;
}
