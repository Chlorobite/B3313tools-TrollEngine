#include "camera_headers.h"

s32 is_range_behind_surface(Vec3f from, Vec3f to, struct Surface *surf, s16 range, s16 surfType) {
    register s32 behindSurface = TRUE;
    register s32 leftBehind = 0;
    register s32 rightBehind = 0;
    f32 checkDist;
    s16 checkPitch;
    s16 checkYaw;
    Vec3f checkPos;

    if (surf != NULL) {
        if (surfType == -1 || surf->type != surfType) {
            if (range == 0) {
                behindSurface = is_behind_surface(to, surf);
            } else {
                vec3f_get_dist_and_angle(from, to, &checkDist, &checkPitch, &checkYaw);
                vec3f_set_dist_and_angle(from, checkPos, checkDist, checkPitch, checkYaw + range);
                leftBehind = is_behind_surface(checkPos, surf);
                vec3f_set_dist_and_angle(from, checkPos, checkDist, checkPitch, checkYaw - range);
                rightBehind = is_behind_surface(checkPos, surf);
                behindSurface = leftBehind * rightBehind;
            }
        }
    }
    return behindSurface;
}
