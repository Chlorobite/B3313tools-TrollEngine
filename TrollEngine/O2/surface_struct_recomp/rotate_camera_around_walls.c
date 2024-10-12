#include "camera_headers.h"

/**
 * Checks for any walls obstructing Mario from view, and calculates a new yaw that the camera should
 * rotate towards.
 *
 * @param[out] avoidYaw the angle (from Mario) that the camera should rotate towards to avoid the wall.
 *                      The camera then approaches avoidYaw until Mario is no longer obstructed.
 *                      avoidYaw is always parallel to the wall.
 * @param yawRange      how wide of an arc to check for walls obscuring Mario.
 *
 * @return 3 if a wall is covering Mario, 1 if a wall is only near the camera.
 */
s32 rotate_camera_around_walls(struct Camera *c, Vec3f cPos, s16 *avoidYaw, s16 yawRange) {
    struct WallCollisionData colData;
    struct Surface *wall;
    f32 dummyDist, checkDist;
    f32 coarseRadius;
    f32 fineRadius;
    s16 wallYaw, horWallNorm;
    s16 dummyPitch;
    // The yaw of the vector from Mario to the camera.
    s16 yawFromMario;
    s32 status = 0;
    /// The current iteration. The algorithm takes 8 equal steps from Mario back to the camera.
    s32 step = 0;

    vec3f_get_dist_and_angle(sMarioCamState->pos, cPos, &dummyDist, &dummyPitch, &yawFromMario);
    sStatusFlags &= ~CAM_FLAG_CAM_NEAR_WALL;
    colData.offsetY = 100.0f;
    // The distance from Mario to Lakitu
    checkDist = 0.0f;
    /// The radius used to find potential walls to avoid.
    /// @bug Increases to 250.f, but the max collision radius is 200.f
    coarseRadius = 150.0f;
    /// This only increases when there is a wall collision found in the coarse pass
    fineRadius = 100.0f;

    for (step = 0; step < 8; step++) {
        // Start at Mario, move backwards to Lakitu's position
        colData.x = sMarioCamState->pos[0] + ((cPos[0] - sMarioCamState->pos[0]) * checkDist);
        colData.y = sMarioCamState->pos[1] + ((cPos[1] - sMarioCamState->pos[1]) * checkDist);
        colData.z = sMarioCamState->pos[2] + ((cPos[2] - sMarioCamState->pos[2]) * checkDist);
        colData.radius = coarseRadius;
        // Increase the coarse check radius
        camera_approach_f32_symmetric_bool(&coarseRadius, 250.f, 30.f);

        if (find_wall_collisions(&colData) != 0) {
            wall = colData.walls[colData.numWalls - 1];

            // If we're over halfway from Mario to Lakitu, then there's a wall near the camera, but
            // not necessarily obstructing Mario
            if (step >= 5) {
                sStatusFlags |= CAM_FLAG_CAM_NEAR_WALL;
                if (status <= 0) {
                    status = 1;
                    wall = colData.walls[colData.numWalls - 1];
                    // wallYaw is parallel to the wall, not perpendicular
                    wallYaw = atan2s(wall->normal.z, wall->normal.x) + DEGREES(90);
                    // Calculate the avoid direction. The function returns the opposite direction so add 180
                    // degrees.
                    *avoidYaw = calc_avoid_yaw(yawFromMario, wallYaw) + DEGREES(180);
                }
            }

            colData.x = sMarioCamState->pos[0] + ((cPos[0] - sMarioCamState->pos[0]) * checkDist);
            colData.y = sMarioCamState->pos[1] + ((cPos[1] - sMarioCamState->pos[1]) * checkDist);
            colData.z = sMarioCamState->pos[2] + ((cPos[2] - sMarioCamState->pos[2]) * checkDist);
            colData.radius = fineRadius;
            // Increase the fine check radius
            camera_approach_f32_symmetric_bool(&fineRadius, 200.f, 20.f);

            if (find_wall_collisions(&colData) != 0) {
                wall = colData.walls[colData.numWalls - 1];
                horWallNorm = atan2s(wall->normal.z, wall->normal.x);
                wallYaw = horWallNorm + DEGREES(90);
                // If Mario would be blocked by the surface, then avoid it
                if ((is_range_behind_surface(sMarioCamState->pos, cPos, wall, yawRange, SURFACE_WALL_MISC) == 0)
                    && (is_mario_behind_surface(c, wall) == TRUE)
                    // Also check if the wall is tall enough to cover Mario
                    && (is_surf_within_bounding_box(wall, -1.f, 150.f, -1.f) == FALSE)) {
                    // Calculate the avoid direction. The function returns the opposite direction so add 180
                    // degrees.
                    *avoidYaw = calc_avoid_yaw(yawFromMario, wallYaw) + DEGREES(180);
                    camera_approach_s16_symmetric_bool(avoidYaw, horWallNorm, yawRange);
                    status = 3;
                    step = 8;
                }
            }
        }
        checkDist += 0.125f;
    }

    return status;
}
