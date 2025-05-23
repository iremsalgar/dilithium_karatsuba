#include <stdint.h>
#include <string.h>
#include "params.h"
#include "poly.h"

#define KARATSUBA_THRESHOLD 32
#define Q 8380417

// Schoolbook multiplication with 64-bit precision
static void schoolbook_mul(int64_t *r, const int32_t *a, const int32_t *b, size_t n) {
    memset(r, 0, 2 * n * sizeof(int64_t));
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < n; j++)
            r[i + j] += (int64_t)a[i] * b[j];
}

// Recursive Karatsuba, stack-allocated buffers
static void karatsuba_recursive(int64_t *r, const int32_t *a, const int32_t *b, size_t n) {
    if (n <= KARATSUBA_THRESHOLD) {
        schoolbook_mul(r, a, b, n);
        return;
    }

    size_t m = n / 2;

    // Stack-allocated temporary arrays
    int32_t a_sum[m], b_sum[m];
    int64_t z0[2 * m], z1[2 * m], z2[2 * m];

    // Compute a_sum = a0 + a1 and b_sum = b0 + b1
    for (size_t i = 0; i < m; i++) {
        a_sum[i] = a[i] + a[i + m];
        b_sum[i] = b[i] + b[i + m];
    }

    karatsuba_recursive(z0, a, b, m);
    karatsuba_recursive(z2, a + m, b + m, m);
    karatsuba_recursive(z1, a_sum, b_sum, m);

    for (size_t i = 0; i < 2 * n; i++) r[i] = 0;

    for (size_t i = 0; i < 2 * m; i++) {
        r[i] += z0[i];
        r[i + m] += z1[i] - z0[i] - z2[i];
        r[i + 2 * m] += z2[i];
    }
}

// Modular reduction that always returns [0, Q)
static inline int32_t barrett_reduce_q(int64_t a) {
    int32_t res = a % Q;
    if (res < 0) res += Q;
    return res;
}

// Final Dilithium polynomial multiplication with Karatsuba
void poly_mul_karatsuba(poly *r, const poly *a, const poly *b) {
    int64_t tmp[2 * N] = {0};
    karatsuba_recursive(tmp, a->coeffs, b->coeffs, N);

    for (size_t i = 0; i < N; i++) {
        int64_t t = tmp[i] - tmp[i + N];          // Reduce mod x^N + 1
        r->coeffs[i] = barrett_reduce_q(t);      // Reduce mod Q to [0, Q)
    }
}

