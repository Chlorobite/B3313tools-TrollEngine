#include "Trolls/bhv/bhv_headers.h"

extern struct ObjectHitbox sKoopaShellHitbox;

void bhv_koopa_shell_loop(void) {
    struct Surface *sp34;

    obj_set_hitbox(o, &sKoopaShellHitbox);
    cur_obj_scale(1.0f);

    switch (o->oAction) {
        case 0:
            cur_obj_update_floor_and_walls();
            cur_obj_if_hit_wall_bounce_away();

            if (o->oInteractStatus & INT_STATUS_INTERACTED) {
                o->oAction++;
            }

            o->oFaceAngleYaw += 0x1000;
            cur_obj_move_standard(-20);
            koopa_shell_spawn_sparkles(10.0f);
            break;

        case 1:
            obj_copy_pos(o, gMarioObject);
            sp34 = cur_obj_update_floor_height_and_get_floor();

            if (absf(find_water_level(o->oPosX, o->oPosZ) - o->oPosY) < 10.0f) {
                koopa_shell_spawn_water_drop();
            } else if (absf(o->oPosY - o->oFloorHeight) < 5.0f) {
                if (sp34 != NULL && sp34->type == 1) {
                    bhv_koopa_shell_flame_spawn();
                } else {
                    koopa_shell_spawn_sparkles(10.0f);
                }
            } else {
                koopa_shell_spawn_sparkles(10.0f);
            }

            o->oFaceAngleYaw = gMarioObject->oMoveAngleYaw;

            if (o->oInteractStatus & INT_STATUS_STOP_RIDING) {
                obj_mark_for_deletion(o);
                spawn_mist_particles();
                o->oAction = 0;
            }
            break;
    }

    o->oInteractStatus = 0;
}
