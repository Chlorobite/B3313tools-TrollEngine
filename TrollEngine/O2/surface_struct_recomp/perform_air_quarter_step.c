#include "mario_step_headers.h"

s32 perform_air_quarter_step(struct MarioState *m, Vec3f intendedPos, u32 stepArg) {
    s16 wallDYaw;
    Vec3f nextPos;
    struct Surface *upperWall;
    struct Surface *lowerWall;
    struct Surface *ceil;
    struct Surface *floor;
    f32 ceilHeight;
    f32 floorHeight;
    f32 waterLevel;

    vec3f_copy(nextPos, intendedPos);

    upperWall = resolve_and_return_wall_collisions(nextPos, 150.0f, 50.0f);
    lowerWall = resolve_and_return_wall_collisions(nextPos, 30.0f, 50.0f);

    floorHeight = find_floor(nextPos[0], nextPos[1], nextPos[2], &floor);
    ceilHeight = vec3f_find_ceil(nextPos, floorHeight, &ceil);

    waterLevel = find_water_level(nextPos[0], nextPos[2]);

    m->wall = NULL;

    //! The water pseudo floor is not referenced when your intended qstep is
    // out of bounds, so it won't detect you as landing.

    if (floor == NULL) {
        if (nextPos[1] <= m->floorHeight) {
            m->pos[1] = m->floorHeight;
            return AIR_STEP_LANDED;
        }

        m->pos[1] = nextPos[1];
        return AIR_STEP_HIT_WALL;
    }

    if ((m->action & ACT_FLAG_RIDING_SHELL) && floorHeight < waterLevel) {
        floorHeight = waterLevel;
        floor = &gWaterSurfacePseudoFloor;
        floor->originOffset = floorHeight; //! Incorrect origin offset (no effect)
    }

    //! This check uses f32, but findFloor uses short (overflow jumps)
    if (nextPos[1] <= floorHeight) {
        if (ceilHeight - floorHeight > 160.0f) {
            m->pos[0] = nextPos[0];
            m->pos[2] = nextPos[2];
            m->floor = floor;
            m->floorHeight = floorHeight;
        }

        //! When ceilHeight - floorHeight <= 160, the step result says that
        // mario landed, but his movement is cancelled and his referenced floor
        // isn't updated (pedro spots)
        m->pos[1] = floorHeight;
        return AIR_STEP_LANDED;
    }

    if (nextPos[1] + 160.0f > ceilHeight) {
        if (m->vel[1] >= 0.0f) {
            m->vel[1] = 0.0f;

            //! Uses referenced ceiling instead of ceil (ceiling hang upwarp)
            if ((stepArg & AIR_STEP_CHECK_HANG) && m->ceil != NULL
                && m->ceil->type == SURFACE_HANGABLE) {
                return AIR_STEP_GRABBED_CEILING;
            }

            return AIR_STEP_NONE;
        }

        //! Potential subframe downwarp->upwarp?
        if (nextPos[1] <= m->floorHeight) {
            m->pos[1] = m->floorHeight;
            return AIR_STEP_LANDED;
        }

        m->pos[1] = nextPos[1];
        return AIR_STEP_HIT_WALL;
    }

    //! When the wall is not completely vertical or there is a slight wall
    // misalignment, you can activate these conditions in unexpected situations
    if ((stepArg & AIR_STEP_CHECK_LEDGE_GRAB) && upperWall == NULL && lowerWall != NULL) {
        if (check_ledge_grab(m, lowerWall, intendedPos, nextPos)) {
            return AIR_STEP_GRABBED_LEDGE;
        }

        vec3f_copy(m->pos, nextPos);
        m->floor = floor;
        m->floorHeight = floorHeight;
        return AIR_STEP_NONE;
    }

    vec3f_copy(m->pos, nextPos);
    m->floor = floor;
    m->floorHeight = floorHeight;

    if (upperWall != NULL || lowerWall != NULL) {
        m->wall = upperWall != NULL ? upperWall : lowerWall;
        wallDYaw = atan2s(m->wall->normal.z, m->wall->normal.x) - m->faceAngle[1];

        if (m->wall->type == SURFACE_BURNING) {
            return AIR_STEP_HIT_LAVA_WALL;
        }

        if (wallDYaw < -0x6000 || wallDYaw > 0x6000) {
            m->flags |= MARIO_UNKNOWN_30;
            return AIR_STEP_HIT_WALL;
        }
    }

    return AIR_STEP_NONE;
}
