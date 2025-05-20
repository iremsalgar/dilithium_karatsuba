#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <string.h> // memcpy için

#include "params.h"
#include "poly.h"

// Q ve R sabitleri (2^32 mod Q)
#define Q 8380417
#define R 4294967296UL // 2^32
#define N 256
#define RmodQ (-4186625) // Dilithium için 2^32 mod Q (negatif temsil)
#define Q32 8380417
#define TEST_ITER 1000

// Eğer negatif ise pozitif moda normalize et
int32_t normalize(int32_t x, int32_t q) {
    if (x < 0) {
        x += q;
    }
    return x;
}

// Mikro saniye hassasiyetli zaman ölçer
static inline double get_time_us() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
}

int main() {
    poly a, a_orig;
    int ok = 1;

    // Orijinal polinomu saklamak için kopya oluştur
    for (int i = 0; i < N; i++) {
        a_orig.coeffs[i] = i; // a[i] = i (örnek polinom)
    }

    // ---------------------------------------------------------------
    // Test: NTT → INTT round-trip
    // ---------------------------------------------------------------
    memcpy(&a, &a_orig, sizeof(poly)); // Orijinal polinomu kopyala
    poly_ntt(&a);
    poly_invntt_tomont(&a);

    // Test: Her a[i] ≟ i × R mod Q
    for (int i = 0; i < N; i++) {
        int32_t got = normalize(a.coeffs[i], Q);
        int64_t tmp = (int64_t)i * R;
        int32_t expected = tmp % Q;
        if (got != expected) {
            printf("Mismatch at index %d: got %d, expected %d\n", i, got, expected);
            ok = 0;
        }
    }

    if (ok) {
        printf("✅ Round-trip NTT → iNTT test passed!\n");
    } else {
        printf("❌ Round-trip test failed!\n");
    }

    // ---------------------------------------------------------------
    // NTT Zaman Ölçümü (Saf NTT)
    // ---------------------------------------------------------------
    double t_start_ntt = get_time_us();
    for (int i = 0; i < TEST_ITER; i++) {
        memcpy(&a, &a_orig, sizeof(poly)); // Her iterasyonda orijinal veriyi yükle
        poly_ntt(&a);
    }
    double t_end_ntt = get_time_us();
    double avg_ntt = (t_end_ntt - t_start_ntt) / TEST_ITER;

    // ---------------------------------------------------------------
    // INTT Zaman Ölçümü (NTT çıktısı önbelleğe alınarak)
    // ---------------------------------------------------------------
    poly ntt_result;
    memcpy(&a, &a_orig, sizeof(poly));
    poly_ntt(&a); // NTT çıktısını hesapla
    memcpy(&ntt_result, &a, sizeof(poly)); // NTT sonucunu sakla

    double t_start_intt = get_time_us();
    for (int i = 0; i < TEST_ITER; i++) {
        memcpy(&a, &ntt_result, sizeof(poly)); // Önbellekteki NTT çıktısını yükle
        poly_invntt_tomont(&a);
    }
    double t_end_intt = get_time_us();
    double avg_intt = (t_end_intt - t_start_intt) / TEST_ITER;

    // ---------------------------------------------------------------
    // Sonuçları Yazdır
    // ---------------------------------------------------------------
    printf("poly_ntt        average time: %.3f µs\n", avg_ntt);
    printf("poly_invntt_tomont average time: %.3f µs\n", avg_intt);

    return 0;
}

