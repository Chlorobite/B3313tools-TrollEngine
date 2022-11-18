#include "object_collision_headers.h"

s32 detect_object_hitbox_overlap(struct Object *a, struct Object *b) {
    register f32 dx = a->oPosX - b->oPosX;
    register f32 dz = a->oPosZ - b->oPosZ;

    if (a->hitboxRadius + b->hitboxRadius  >  sqrtf(dx * dx + dz * dz)) {
		register f32 sp3C = a->oPosY - a->hitboxDownOffset;
		register f32 sp38 = b->oPosY - b->hitboxDownOffset;

        if (sp3C > (b->hitboxHeight + sp38) || (a->hitboxHeight + sp3C) < sp38 ||
			a->numCollidedObjs >= 4 || b->numCollidedObjs >= 4) {
            return 0;
        }
        
        a->collidedObjs[a->numCollidedObjs] = b;
        b->collidedObjs[b->numCollidedObjs] = a;
        a->collidedObjInteractTypes |= b->oInteractType;
        b->collidedObjInteractTypes |= a->oInteractType;
        a->numCollidedObjs++;
        b->numCollidedObjs++;
        return 1;
    }

    return 0;
}
