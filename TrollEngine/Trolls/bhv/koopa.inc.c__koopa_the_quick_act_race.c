#include "bhv_headers.h"


void koopa_the_quick_act_race(void) {
    if (obj_begin_race(FALSE)) {
        // Hitbox is slightly larger while racing
        cur_obj_push_mario_away_from_cylinder(180.0f, 300.0f);

        if (cur_obj_follow_path(0) == PATH_REACHED_END) {
            o->oAction = KOOPA_THE_QUICK_ACT_DECELERATE;
        } else {
            f32 downhillSteepness;
            s32 bowlingBallStatus;

            downhillSteepness = 1.0f + sins((s16)(f32) o->oPathedTargetPitch);
            cur_obj_rotate_yaw_toward(o->oPathedTargetYaw, (s32)(o->oKoopaAgility * 150.0f));

            switch (o->oSubAction) {
                case KOOPA_THE_QUICK_SUB_ACT_START_RUN:
                    koopa_walk_start();
                    break;

                case KOOPA_THE_QUICK_SUB_ACT_RUN:
                    koopa_the_quick_animate_footsteps();

                    if (o->parentObj->oKoopaRaceEndpointRaceStatus != 0 && o->oDistanceToMario > 1500.0f
                        && (o->oPathedPrevWaypointFlags & WAYPOINT_MASK_00FF) < 28) {
                        // Move faster if mario has already finished the race or
                        // cheated by shooting from cannon
                        o->oKoopaAgility = 8.0f;
                    } else if (o->oKoopaTheQuickRaceIndex != KOOPA_THE_QUICK_BOB_INDEX) {
                        o->oKoopaAgility = 6.0f;
                    } else {
                        o->oKoopaAgility = 4.0f;
                    }
                    //o->oKoopaAgility *= TRACKER_difficulty_modifier_half;

                    obj_forward_vel_approach(o->oKoopaAgility * 6.0f * downhillSteepness,
                                             o->oKoopaAgility * 0.1f);

                    // Move upward if we hit a wall, to climb it
                    if (o->oMoveFlags & OBJ_MOVE_HIT_WALL) {
                        o->oVelY = 20.0f;
                    }

                    // If we're about to collide with a bowling ball or we hit an
                    // edge, jump
                    bowlingBallStatus = koopa_the_quick_detect_bowling_ball();
                    if (bowlingBallStatus != 0 || (o->oMoveFlags & OBJ_MOVE_HIT_EDGE)) {
                        // If the ball is coming at us from behind, then set speed
                        // to zero to let it move under and past us
                        if (bowlingBallStatus < 0) {
                            o->oForwardVel = 0.0f;
                        }

                        if (bowlingBallStatus != 0
                            || (o->oPathedPrevWaypointFlags & WAYPOINT_MASK_00FF) >= 8) {
                            o->oVelY = 80.0f;
                        } else {
                            o->oVelY = 40.0f;
                        }

                        o->oGravity = -6.0f;
                        o->oSubAction = 2;
                        o->oMoveFlags = 0;

                        cur_obj_init_animation_with_sound(12);
                    }
                    break;

                case KOOPA_THE_QUICK_SUB_ACT_JUMP:
                    // We could perform a goomba double jump if we could deactivate
                    // ktq
                    if (o->oMoveFlags & OBJ_MOVE_MASK_ON_GROUND) {
                        if (cur_obj_init_anim_and_check_if_end(13)) {
                            o->oSubAction--;
                        }

                        koopa_the_quick_detect_bowling_ball();
                    }
            }
        }
    }
}

