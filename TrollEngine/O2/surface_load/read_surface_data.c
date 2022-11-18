#include "surface_load_headers.h"

struct Surface *read_surface_data(s16 *vertexData, s16 **vertexIndices) {
    register struct Surface *surface;
    register f32 x1, y1, z1;
    register f32 x2, y2, z2;
    register f32 x3, y3, z3;
    register f32 maxY, minY;
    register f32 nx, ny, nz;
    f32 mag;
	s16 offset;
	
    offset = 3 * (*vertexIndices)[0];
    x1 = *(vertexData + offset + 0);
    y1 = *(vertexData + offset + 1);
    z1 = *(vertexData + offset + 2);

    offset = 3 * (*vertexIndices)[1];
    x2 = *(vertexData + offset + 0);
    y2 = *(vertexData + offset + 1);
    z2 = *(vertexData + offset + 2);

    offset = 3 * (*vertexIndices)[2];
    x3 = *(vertexData + offset + 0);
    y3 = *(vertexData + offset + 1);
    z3 = *(vertexData + offset + 2);

    // (v2 - v1) x (v3 - v2)
    nx = (y2 - y1) * (z3 - z2) - (z2 - z1) * (y3 - y2);
    ny = (z2 - z1) * (x3 - x2) - (x2 - x1) * (z3 - z2);
    nz = (x2 - x1) * (y3 - y2) - (y2 - y1) * (x3 - x2);
    mag = sqrtf(nx * nx + ny * ny + nz * nz);

    // Could have used min_3 and max_3 for this...
    minY = y1;
    if (y2 < minY) {
        minY = y2;
    }
    if (y3 < minY) {
        minY = y3;
    }

    maxY = y1;
    if (y2 > maxY) {
        maxY = y2;
    }
    if (y3 > maxY) {
        maxY = y3;
    }

    // Checking to make sure no DIV/0
    if (mag < 0.0001f) {
        return NULL;
    }
    nx /= mag;
    ny /= mag;
    nz /= mag;

    surface = alloc_surface();

    surface->vertex1[0] = x1;
    surface->vertex2[0] = x2;
    surface->vertex3[0] = x3;

    surface->vertex1[1] = y1;
    surface->vertex2[1] = y2;
    surface->vertex3[1] = y3;

    surface->vertex1[2] = z1;
    surface->vertex2[2] = z2;
    surface->vertex3[2] = z3;

    surface->normal.x = nx;
    surface->normal.y = ny;
    surface->normal.z = nz;

    surface->originOffset = -(nx * x1 + ny * y1 + nz * z1);

    surface->lowerY = minY - 5.f;
    surface->upperY = maxY + 5.f;

    return surface;
}
