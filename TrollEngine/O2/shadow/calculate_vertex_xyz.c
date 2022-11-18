#include "shadow_headers.h"

void calculate_vertex_xyz(s8 index, struct Shadow s, f32 *xPosVtx, f32 *yPosVtx, f32 *zPosVtx,
                          s8 shadowVertexType) {
    register f32 tiltedScale = coss(s.floorTilt * 182.044f) * s.shadowScale;
    register s32 downwardAngle = (s32)(s.floorDownwardAngle * 182.044f);
    register f32 halfScale;
    register f32 halfTiltedScale;
    s8 xCoordUnit, zCoordUnit;

    // This makes xCoordUnit and yCoordUnit each one of -1, 0, or 1.
    get_vertex_coords(index, shadowVertexType, &xCoordUnit, &zCoordUnit);

    halfScale = xCoordUnit * s.shadowScale;
    halfTiltedScale = zCoordUnit * tiltedScale;

    *xPosVtx = ((halfTiltedScale * sins(downwardAngle)) + (halfScale * coss(downwardAngle))) / 2.f + s.parentX;
    *zPosVtx = ((halfTiltedScale * coss(downwardAngle)) - (halfScale * sins(downwardAngle))) / 2.f + s.parentZ;

    if (gShadowAboveWaterOrLava) {
        *yPosVtx = s.floorHeight;
		return;
    }
    
    *yPosVtx = extrapolate_vertex_y_position(s, *xPosVtx, *zPosVtx);
}
