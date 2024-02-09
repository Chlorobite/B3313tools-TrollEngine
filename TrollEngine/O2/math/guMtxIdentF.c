#include <ultra64.h>

void guMtxIdentF(f32 mf[4][4]) {
    register f32 *ptr = (f32*)mf;
    register f32 ZERO = 0.0f;
    register f32 ONE = 1.0f;
    
    ptr[0] = ONE;
    ptr[1] = ZERO;
    ptr[2] = ZERO;
    ptr[3] = ZERO;
    ptr[4] = ZERO;
    ptr[5] = ONE;
    ptr[6] = ZERO;
    ptr[7] = ZERO;
    ptr[8] = ZERO;
    ptr[9] = ZERO;
    ptr[10] = ONE;
    ptr[11] = ZERO;
    ptr[12] = ZERO;
    ptr[13] = ZERO;
    ptr[14] = ZERO;
    ptr[15] = ONE;
}
