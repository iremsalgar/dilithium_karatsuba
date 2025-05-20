#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "params.h"
#include "poly.h"


// Basit zaman ölçümü
uint64_t now_ns() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (uint64_t)t.tv_sec * 1000000000 + t.tv_nsec;
}

int main() {
    poly a, a_orig;
    uint64_t start, end;

    // Test verisi oluştur: a(x) = 1 + 2x + 3x^2 + ...
    for (int i = 0; i < N; i++) {
        a.coeffs[i] = i + 1;
        a_orig.coeffs[i] = i + 1;
    }

    // Zaman ölçümü: NTT
    start = now_ns();
    poly_ntt(&a);
    end = now_ns();
    printf("NTT done in %.3f µs\n", (end - start) / 1000.0);

    // Zaman ölçümü: inverse NTT
    start = now_ns();
    poly_invntt_tomont(&a);
    end = now_ns();
    printf("iNTT done in %.3f µs\n", (end - start) / 1000.0);

    // Doğrulama: round-trip kontrolü
    int ok = 1;
    for (int i = 0; i < N; i++) {
        if (a.coeffs[i] != a_orig.coeffs[i]) {
            printf("Mismatch at index %d: got %d, expected %d\n", i, a.coeffs[i], a_orig.coeffs[i]);
            ok = 0;
        }
    }

    if (ok) {
        printf("Round-trip NTT -> iNTT successful ✅\n");
    } else {
        printf("❌ Round-trip failed!\n");
    }

    return 0;
}

