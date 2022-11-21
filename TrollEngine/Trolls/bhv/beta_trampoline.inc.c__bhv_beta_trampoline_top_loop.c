#include "bhv_headers.h"


void SetTrampolineSinkDegree(struct Object* trampoline, float f);

void bhv_beta_trampoline_top_loop(void) {
    // When initialized, spawn the rest of the trampoline
    if (o->oTimer == 0) {
        struct Object *trampolinePart;

        trampolinePart = spawn_object(o, MODEL_TRAMPOLINE_CENTER, bhvBetaTrampolineSpring);
        trampolinePart->oPosY -= 75.0f;

        trampolinePart = spawn_object(o, MODEL_TRAMPOLINE_BASE, bhvStaticObject);
        trampolinePart->oPosY -= 150.0f;
    }

    o->oBetaTrampolineMarioOnTrampoline = gMarioObject->platform == o;

    SetTrampolineSinkDegree(o, o->oHomeY - o->oPosY);
}

