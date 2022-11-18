#include "bhv_headers.h"

struct Object *spawn_star(struct Object *sp30, f32 sp34, f32 sp38, f32 sp3C);
void spawn_no_exit_star(f32 sp20, f32 sp24, f32 sp28) {
    struct Object *sp1C = spawn_star(sp1C, sp20, sp24, sp28);
    sp1C->oBehParams2ndByte = 1;
    sp1C->oInteractionSubtype |= INT_SUBTYPE_NO_EXIT;
    sp1C->oBooTurningSpeed |= 0x02; // this is only used by bowser reds, so it's a reds star
}

