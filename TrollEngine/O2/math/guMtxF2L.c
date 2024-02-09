#include <ultra64.h>

void guMtxF2L(f32 mf[4][4], Mtx *m) {
    int r, c;
    s32 tmp;
    s16 *m1 = (s16*)&m->m[0][0];
    s16 *m2 = (s16*)&m->m[2][0];
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            tmp = mf[r][c] * 65536.0f;
            *m1++ = (tmp >> 0x10) & 0xffff;
            *m2++ = tmp & 0xffff;
        }
    }
}
