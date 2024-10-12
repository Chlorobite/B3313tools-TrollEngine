#include <PR/ultratypes.h>

#include "area.h"
#include "actors/common1.h"
#include "audio/external.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "course_table.h"
#include "dialog_ids.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "game_init.h"
#include "interaction.h"
#include "level_update.h"
#include "mario.h"
#include "mario_step.h"
#include "memory.h"
#include "obj_behaviors.h"
#include "object_helpers.h"
#include "save_file.h"
#include "seq_ids.h"
#include "sm64.h"
#include "sound_init.h"
#include "rumble_init.h"

#define INT_GROUND_POUND_OR_TWIRL (1 << 0) // 0x01
#define INT_PUNCH                 (1 << 1) // 0x02
#define INT_KICK                  (1 << 2) // 0x04
#define INT_TRIP                  (1 << 3) // 0x08
#define INT_SLIDE_KICK            (1 << 4) // 0x10
#define INT_FAST_ATTACK_OR_SHELL  (1 << 5) // 0x20
#define INT_HIT_FROM_ABOVE        (1 << 6) // 0x40
#define INT_HIT_FROM_BELOW        (1 << 7) // 0x80

#define INT_ATTACK_NOT_FROM_BELOW                                                 \
    (INT_GROUND_POUND_OR_TWIRL | INT_PUNCH | INT_KICK | INT_TRIP | INT_SLIDE_KICK \
     | INT_FAST_ATTACK_OR_SHELL | INT_HIT_FROM_ABOVE)

#define INT_ANY_ATTACK                                                            \
    (INT_GROUND_POUND_OR_TWIRL | INT_PUNCH | INT_KICK | INT_TRIP | INT_SLIDE_KICK \
     | INT_FAST_ATTACK_OR_SHELL | INT_HIT_FROM_ABOVE | INT_HIT_FROM_BELOW)

#define INT_ATTACK_NOT_WEAK_FROM_ABOVE                                                \
    (INT_GROUND_POUND_OR_TWIRL | INT_PUNCH | INT_KICK | INT_TRIP | INT_HIT_FROM_BELOW)

extern u8 sDelayInvincTimer;
extern s16 sInvulnerable;
u32 interact_coin(struct MarioState *, u32, struct Object *);
u32 interact_water_ring(struct MarioState *, u32, struct Object *);
u32 interact_star_or_key(struct MarioState *, u32, struct Object *);
u32 interact_bbh_entrance(struct MarioState *, u32, struct Object *);
u32 interact_warp(struct MarioState *, u32, struct Object *);
u32 interact_warp_door(struct MarioState *, u32, struct Object *);
u32 interact_door(struct MarioState *, u32, struct Object *);
u32 interact_cannon_base(struct MarioState *, u32, struct Object *);
u32 interact_igloo_barrier(struct MarioState *, u32, struct Object *);
u32 interact_tornado(struct MarioState *, u32, struct Object *);
u32 interact_whirlpool(struct MarioState *, u32, struct Object *);
u32 interact_strong_wind(struct MarioState *, u32, struct Object *);
u32 interact_flame(struct MarioState *, u32, struct Object *);
u32 interact_snufit_bullet(struct MarioState *, u32, struct Object *);
u32 interact_clam_or_bubba(struct MarioState *, u32, struct Object *);
u32 interact_bully(struct MarioState *, u32, struct Object *);
u32 interact_shock(struct MarioState *, u32, struct Object *);
u32 interact_mr_blizzard(struct MarioState *, u32, struct Object *);
u32 interact_hit_from_below(struct MarioState *, u32, struct Object *);
u32 interact_bounce_top(struct MarioState *, u32, struct Object *);
u32 interact_unknown_08(struct MarioState *, u32, struct Object *);
u32 interact_damage(struct MarioState *, u32, struct Object *);
u32 interact_breakable(struct MarioState *, u32, struct Object *);
u32 interact_koopa_shell(struct MarioState *, u32, struct Object *);
u32 interact_pole(struct MarioState *, u32, struct Object *);
u32 interact_hoot(struct MarioState *, u32, struct Object *);
u32 interact_cap(struct MarioState *, u32, struct Object *);
u32 interact_grabbable(struct MarioState *, u32, struct Object *);
u32 interact_text(struct MarioState *, u32, struct Object *);


void mario_handle_special_floors(struct MarioState *m) {
    if ((m->action & ACT_GROUP_MASK) == ACT_GROUP_CUTSCENE) {
        return;
    }

    if (m->floor != NULL) {
        s32 floorType = m->floor->type;

        switch (floorType) {
            case SURFACE_DEATH_PLANE:
            case SURFACE_VERTICAL_WIND:
                check_death_barrier(m);
                break;

            case SURFACE_WARP:
                level_trigger_warp(m, WARP_OP_WARP_FLOOR);
                break;

            case SURFACE_TIMER_START:
                pss_begin_slide(m);
                break;

            case SURFACE_TIMER_END:
                pss_end_slide(m);
                break;
        }

        if (!(m->action & ACT_FLAG_AIR) && !(m->action & ACT_FLAG_SWIMMING)) {
            switch (floorType) {
                case SURFACE_BURNING:
                    check_lava_boost(m);
                    break;
            }
        }
    }
}
