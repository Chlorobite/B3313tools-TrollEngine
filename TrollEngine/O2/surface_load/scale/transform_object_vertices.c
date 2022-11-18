#include "../surface_load_headers.h"
#include "AI/personalization_helpers.h"

void transform_object_vertices(s16 **data, s16 *vertexData) {
    register s16 *vertices;
    register f32 vx, vy, vz;
    register s32 numVertices;

    Mat4 *objectTransform;
    register Mat4 m;

    objectTransform = &gCurrentObject->transform;

    numVertices = *(*data);
    (*data)++;

    vertices = *data;

    if (gCurrentObject->header.gfx.throwMatrix == NULL) {
        gCurrentObject->header.gfx.throwMatrix = objectTransform;
        obj_build_transform_from_pos_and_angle(gCurrentObject, O_POS_INDEX, O_FACE_ANGLE_INDEX);
    }

    obj_apply_scale_to_matrix(gCurrentObject, m, *objectTransform);

    // Go through all vertices, rotating and translating them to transform the object.
    while (numVertices--) {
        vx = *vertices;
        vy = *(vertices + 1);
        vz = *(vertices + 2);

        //! No bounds check on vertex data
        *vertexData = (s16)((vx * m[0][0] + vy * m[1][0] + vz * m[2][0] + m[3][0]) / levelScaleH);
        *(vertexData + 1) = (s16)((vx * m[0][1] + vy * m[1][1] + vz * m[2][1] + m[3][1]) / levelScaleV);
        *(vertexData + 2) = (s16)((vx * m[0][2] + vy * m[1][2] + vz * m[2][2] + m[3][2]) / levelScaleH);
        
        vertices += 3;
        vertexData += 3;
    }

    *data = vertices;
}
