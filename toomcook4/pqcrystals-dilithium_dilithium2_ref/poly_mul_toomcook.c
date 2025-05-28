
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
    const int32_t *a0 = &a->coeffs[0], *a1 = &a->coeffs[n], *a2 = &a->coeffs[2*n], *a3 = &a->coeffs[3*n];
    const int32_t *b0 = &b->coeffs[0], *b1 = &b->coeffs[n], *b2 = &b->coeffs[2*n], *b3 = &b->coeffs[3*n];

    int32_t ae[7][n], be[7][n];
    int64_t ce[7][2 * n], c[8][2 * n];

    for (int i = 0; i < n; i++) {
        int32_t A0 = a0[i], A1 = a1[i], A2 = a2[i], A3 = a3[i];
        int32_t B0 = b0[i], B1 = b1[i], B2 = b2[i], B3 = b3[i];

        ae[0][i] = A0;
        ae[1][i] = A0 + A1 + A2 + A3;
        ae[2][i] = A0 - A1 + A2 - A3;
        ae[3][i] = A0 + 2*A1 + 4*A2 + 8*A3;
        ae[4][i] = A0 - 2*A1 + 4*A2 - 8*A3;
        ae[5][i] = A0 + 3*A1 + 9*A2 + 27*A3;
        ae[6][i] = A3;

        be[0][i] = B0;
        be[1][i] = B0 + B1 + B2 + B3;
        be[2][i] = B0 - B1 + B2 - B3;
        be[3][i] = B0 + 2*B1 + 4*B2 + 8*B3;
        be[4][i] = B0 - 2*B1 + 4*B2 - 8*B3;
        be[5][i] = B0 + 3*B1 + 9*B2 + 27*B3;
        be[6][i] = B3;
    }

    for (int k = 0; k < 7; k++) {
        schoolbook_64(ce[k], ae[k], be[k]);
    }

    memset(c, 0, sizeof(c));
    for (int i = 0; i < 2 * n; i++) {
        int64_t v0 = ce[0][i], v1 = ce[1][i], v2 = ce[2][i];
        int64_t v3 = ce[3][i], v4 = ce[4][i], v5 = ce[5][i], v6 = ce[6][i];

        int64_t d0 = v0;
        int64_t d6 = v6;
        int64_t s1 = v1 - d0 - d6;
        int64_t s2 = v2 - d0 - d6;
        int64_t s3 = v3 - d0 - 64 * d6;
        int64_t s4 = v4 - d0 - 64 * d6;
        int64_t s5 = v5 - d0 - 729 * d6;

        int64_t d4 = (s3 + s4 - 4 * (s1 + s2)) / 24;
        int64_t d2 = (s1 + s2)/2 - d4;
        int64_t a = (s1 - s2)/2;
        int64_t b = (s3 - s4)/4;
        int64_t c5 = (5*a + (s5 - 9*d2 - 81*d4) - 8*b) / 120;
        int64_t d3 = (b - a - 15*c5)/3;
        int64_t d1 = a - d3 - c5;

        c[0][i] = d0;
        c[1][i] = d1;
        c[2][i] = d2;
        c[3][i] = d3;
        c[4][i] = d4;
        c[5][i] = c5;
        c[6][i] = d6;
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

