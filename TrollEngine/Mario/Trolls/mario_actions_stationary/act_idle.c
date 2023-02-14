#include "../mario_headers.h"


s32 act_idle(struct MarioState *m) {
    if (m->quicksandDepth > 30.0f) {
        return set_mario_action(m, ACT_IN_QUICKSAND, 0);
    }

    if (m->input & INPUT_IN_POISON_GAS) {
        return set_mario_action(m, ACT_COUGHING, 0);
    }

    if (!(m->actionArg & 1) && m->health < 0x300) {
        return set_mario_action(m, ACT_PANTING, 0);
    }

    if (check_common_idle_cancels(m)) {
        return TRUE;
    }

    if (m->actionState == 3) {
        if ((m->area->terrainType & TERRAIN_MASK) == TERRAIN_SNOW) {
            return set_mario_action(m, ACT_SHIVERING, 0);
        } else {
            return set_mario_action(m, ACT_START_SLEEPING, 0);
        }
    }

    if (m->actionArg & 1) {
        set_mario_animation(m, MARIO_ANIM_STAND_AGAINST_WALL);
    } else {
        switch (m->actionState) {
            case 0:
                set_mario_animation(m, MARIO_ANIM_IDLE_HEAD_LEFT);
                break;

            case 1:
                set_mario_animation(m, MARIO_ANIM_IDLE_HEAD_RIGHT);
                break;

            case 2:
                set_mario_animation(m, MARIO_ANIM_IDLE_HEAD_CENTER);
                break;
        }

        if (is_anim_at_end(m)) {
            // Fall asleep after 10 head turning cycles.
            // act_start_sleeping is triggered earlier in the function
            // when actionState == 3. This happens when Mario's done
            // turning his head back and forth. However, we do some checks
            // here to make sure that Mario would be able to sleep here,
            // and that he's gone through 10 cycles before sleeping.
            // actionTimer is used to track how many cycles have passed.
            if (++m->actionState == 3) {
                f32 deltaYOfFloorBehindMario = m->pos[1] - find_floor_height_relative_polar(m, -0x8000, 60.0f);
                if (deltaYOfFloorBehindMario < -24.0f || 24.0f < deltaYOfFloorBehindMario || m->floor->flags & SURFACE_FLAG_DYNAMIC) {
                    m->actionState = 0;
                } else {
                    // If Mario hasn't turned his head 10 times yet, stay idle instead of going to sleep.
                    /*m->actionTimer++;
                    if (m->actionTimer < 10) {
                        m->actionState = 0;
                    }*/
                }
                
                // Trigger elecwait :trol:
                if (m->floor->type == SURFACE_ELECTRIC)
					return hurt_and_set_mario_action(m, ACT_ELECTRIC_FLOOR_IDLE, 0, 4);
            }
        }
    }

    stationary_ground_step(m);

    return FALSE;
}
