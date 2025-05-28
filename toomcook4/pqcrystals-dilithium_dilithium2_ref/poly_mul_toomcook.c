#include <stdint.h>
#include <string.h>
#include "params.h"
#include "poly.h"

#define Q 8380417
#define N 256
#define TC_SPLIT 64

static inline int32_t modq(int64_t x) {
    int32_t r = x % Q;
    if (r < 0) r += Q;
    return r;
}

static void schoolbook_64(int64_t *r, const int32_t *a, const int32_t *b) {
    memset(r, 0, 2 * TC_SPLIT * sizeof(int64_t));
    for (size_t i = 0; i < TC_SPLIT; i++) {
        for (size_t j = 0; j < TC_SPLIT; j++) {
            r[i + j] += (int64_t)a[i] * b[j];
        }
    }
}

void poly_mul_toomcook(poly *r, const poly *a, const poly *b) {
    const int n = TC_SPLIT;

    const int32_t *a0 = &a->coeffs[0];
    const int32_t *a1 = &a->coeffs[n];
    const int32_t *a2 = &a->coeffs[2 * n];
    const int32_t *a3 = &a->coeffs[3 * n];
    const int32_t *b0 = &b->coeffs[0];
    const int32_t *b1 = &b->coeffs[n];
    const int32_t *b2 = &b->coeffs[2 * n];
    const int32_t *b3 = &b->coeffs[3 * n];

    int32_t ae[7][n], be[7][n];
    int64_t ce[7][2 * n];

    for (int i = 0; i < n; i++) {
        int32_t A[4] = {a0[i], a1[i], a2[i], a3[i]};
        int32_t B[4] = {b0[i], b1[i], b2[i], b3[i]};

        ae[0][i] = A[0];
        ae[1][i] = A[0] + A[1] + A[2] + A[3];
        ae[2][i] = A[0] - A[1] + A[2] - A[3];
        ae[3][i] = A[0] + 2*A[1] + 4*A[2] + 8*A[3];
        ae[4][i] = A[0] - 2*A[1] + 4*A[2] - 8*A[3];
        ae[5][i] = A[0] + 3*A[1] + 9*A[2] + 27*A[3];
        ae[6][i] = A[3];

        be[0][i] = B[0];
        be[1][i] = B[0] + B[1] + B[2] + B[3];
        be[2][i] = B[0] - B[1] + B[2] - B[3];
        be[3][i] = B[0] + 2*B[1] + 4*B[2] + 8*B[3];
        be[4][i] = B[0] - 2*B[1] + 4*B[2] - 8*B[3];
        be[5][i] = B[0] + 3*B[1] + 9*B[2] + 27*B[3];
        be[6][i] = B[3];
    }

    for (int k = 0; k < 7; k++) {
        schoolbook_64(ce[k], ae[k], be[k]);
    }

    int64_t c[8][2 * n];
    memset(c, 0, sizeof(c));

    for (int i = 0; i < 2 * n; i++) {
        int64_t ce0 = ce[0][i];
        int64_t ce1 = ce[1][i];
        int64_t ce2 = ce[2][i];
        int64_t ce3 = ce[3][i];
        int64_t ce4 = ce[4][i];
        int64_t ce5 = ce[5][i];
        int64_t ce6 = ce[6][i];

        int64_t c0 = ce0;
        int64_t c6 = ce6;

        int64_t e1 = ce1 - c0 - c6;
        int64_t e2 = ce2 - c0 - c6;
        int64_t e3 = ce3 - c0 - 64 * c6;
        int64_t e4 = ce4 - c0 - 64 * c6;
        int64_t e5 = ce5 - c0 - 729 * c6;

        int64_t numerator_c4 = (e3 + e4)/8 - (e1 + e2)/2;
        int64_t c4 = numerator_c4 / 3;
        int64_t c2 = (e1 + e2)/2 - c4;

        int64_t A = (e1 - e2)/2;
        int64_t B = (e3 - e4)/4;
        int64_t C = e5 - 9 * c2 - 81 * c4;

        int64_t c5 = (5 * A + C - 8 * B) / 120;
        int64_t c3 = (B - A - 15 * c5) / 3;
        int64_t c1 = A - c3 - c5;

        c[0][i] = c0;
        c[1][i] = c1;
        c[2][i] = c2;
        c[3][i] = c3;
        c[4][i] = c4;
        c[5][i] = c5;
        c[6][i] = c6;
    }

    memset(r->coeffs, 0, sizeof(r->coeffs));

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 2 * n; j++) {
            int pos = i * n + j;
            if (pos < N)
                r->coeffs[pos] += modq(c[i][j]);
            else
                r->coeffs[pos - N] -= modq(c[i][j]);
        }
    }

    for (int i = 0; i < N; i++) {
        r->coeffs[i] = modq(r->coeffs[i]);
    }
}
