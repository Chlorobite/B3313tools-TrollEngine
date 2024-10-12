#include "camera_headers.h"

/**
 * Checks if `surf` is within the rect prism defined by xMax, yMax, and zMax
 *
 * @param surf surface to check
 * @param xMax absolute-value max size in x, set to -1 to ignore
 * @param yMax absolute-value max size in y, set to -1 to ignore
 * @param zMax absolute-value max size in z, set to -1 to ignore
 */
s32 is_surf_within_bounding_box(struct Surface *surf, f32 xMax, f32 yMax, f32 zMax) {
    // Surface vertex coordinates
    Vec3s sx;
    Vec3s sy;
    Vec3s sz;
    // Max delta between x, y, and z
    s16 dxMax = 0;
    s16 dyMax = 0;
    s16 dzMax = 0;
    // Current deltas between x, y, and z
    f32 dx;
    f32 dy;
    f32 dz;
    s32 i;
    s32 j;
    // result
    s32 smaller = FALSE;

    sx[0] = surf->vertexX[0];
    sx[1] = surf->vertexX[1];
    sx[2] = surf->vertexX[2];
    sy[0] = surf->vertexY[0];
    sy[1] = surf->vertexY[1];
    sy[2] = surf->vertexY[2];
    sz[0] = surf->vertexZ[0];
    sz[1] = surf->vertexZ[1];
    sz[2] = surf->vertexZ[2];

    for (i = 0; i < 3; i++) {
        j = i + 1;
        if (j >= 3) {
            j = 0;
        }
        dx = ABS(sx[i] - sx[j]);
        if (dx > dxMax) {
            dxMax = dx;
        }
        dy = ABS(sy[i] - sy[j]);
        if (dy > dyMax) {
            dyMax = dy;
        }
        dz = ABS(sz[i] - sz[j]);
        if (dz > dzMax) {
            dzMax = dz;
        }
    }
    if (yMax != -1.f) {
        if (dyMax < yMax) {
            smaller = TRUE;
        }
    }
    if (xMax != -1.f && zMax != -1.f) {
        if (dxMax < xMax && dzMax < zMax) {
            smaller = TRUE;
        }
    }
    return smaller;
}
