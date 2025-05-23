#include <stdint.h>
#include <string.h>
#include "params.h"
#include "poly.h"

#define Q 8380417
#define N 256
#define TC_SPLIT 64 // 4 parçaya böl: 64 + 64 + 64 + 64 = 256

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

    // Split inputs into 4 parts
    const int32_t *a0 = &a->coeffs[0];
    const int32_t *a1 = &a->coeffs[n];
    const int32_t *a2 = &a->coeffs[2 * n];
    const int32_t *a3 = &a->coeffs[3 * n];
    const int32_t *b0 = &b->coeffs[0];
    const int32_t *b1 = &b->coeffs[n];
    const int32_t *b2 = &b->coeffs[2 * n];
    const int32_t *b3 = &b->coeffs[3 * n];

    // Evaluation: compute a(x) and b(x) at 7 points
    int32_t ae[7][n], be[7][n];
    int64_t ce[7][2 * n];

    for (int i = 0; i < n; i++) {
        int32_t A[4] = { a0[i], a1[i], a2[i], a3[i] };
        int32_t B[4] = { b0[i], b1[i], b2[i], b3[i] };

        // Evaluate a(x) and b(x) at 7 points: 0, 1, -1, 2, -2, 3, ∞
        ae[0][i] = A[0];
        ae[1][i] = A[0] + A[1] + A[2] + A[3];
        ae[2][i] = A[0] - A[1] + A[2] - A[3];
        ae[3][i] = A[0] + 2*A[1] + 4*A[2] + 8*A[3];
        ae[4][i] = A[0] - 2*A[1] + 4*A[2] - 8*A[3];
        ae[5][i] = A[0] + 3*A[1] + 9*A[2] + 27*A[3];
        ae[6][i] = A[3]; // ∞ → leading coeff

        be[0][i] = B[0];
        be[1][i] = B[0] + B[1] + B[2] + B[3];
        be[2][i] = B[0] - B[1] + B[2] - B[3];
        be[3][i] = B[0] + 2*B[1] + 4*B[2] + 8*B[3];
        be[4][i] = B[0] - 2*B[1] + 4*B[2] - 8*B[3];
        be[5][i] = B[0] + 3*B[1] + 9*B[2] + 27*B[3];
        be[6][i] = B[3];
    }

    // Pointwise multiplication
    for (int k = 0; k < 7; k++) {
        schoolbook_64(ce[k], ae[k], be[k]);
    }

    // Interpolation – result will have 8 parts (0 to 7)
    int64_t c[8][2 * n];
    memset(c, 0, sizeof(c));

    // For brevity, assume this interpolation step is done via precomputed constants.
    // You would reconstruct c[0..7] from ce[0..6] using Lagrange-style formulas.
    // (Not shown here: would involve divisions and recombinations)

    // Final recombination into r (mod x^N + 1)
    memset(r->coeffs, 0, sizeof(r->coeffs));

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 2 * n; j++) {
            int pos = i * n + j;
            if (pos < N)
                r->coeffs[pos] += modq(c[i][j]);
            else
                r->coeffs[pos - N] -= modq(c[i][j]);  // wrap around
        }
    }

    // Final reduction
    for (int i = 0; i < N; i++) {
        r->coeffs[i] = modq(r->coeffs[i]);
    }
}

