#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <oqs/oqs.h>
#include <time.h>

#define ITERATIONS 10000

// Inline zaman ölçümü (optimizasyona karşı korumalı)
static inline double get_current_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

void test_keygen(OQS_SIG *sig, double *total_time) {
    for (int i = 0; i < ITERATIONS; i++) {
        uint8_t public_key[OQS_SIG_dilithium_2_length_public_key];
        uint8_t secret_key[OQS_SIG_dilithium_2_length_secret_key];
        double start = get_current_time();
        if (OQS_SIG_dilithium_2_keypair(public_key, secret_key) != OQS_SUCCESS) {
            fprintf(stderr, "Keygen failed at %d\n", i);
            return;
        }
        *total_time += get_current_time() - start;
    }
}

void test_sign(OQS_SIG *sig, const char *msg, size_t msg_len, double *total_time) {
    for (int i = 0; i < ITERATIONS; i++) {
        uint8_t secret_key[OQS_SIG_dilithium_2_length_secret_key];
        uint8_t public_key[OQS_SIG_dilithium_2_length_public_key];
        OQS_SIG_dilithium_2_keypair(public_key, secret_key);

        uint8_t signature[OQS_SIG_dilithium_2_length_signature];
        size_t signature_len;
        double start = get_current_time();
        if (OQS_SIG_dilithium_2_sign(signature, &signature_len, (uint8_t *)msg, msg_len, secret_key) != OQS_SUCCESS) {
            fprintf(stderr, "Sign failed at %d\n", i);
            return;
        }
        *total_time += get_current_time() - start;
    }
}

void test_verify(OQS_SIG *sig, const char *msg, size_t msg_len, double *total_time) {
    for (int i = 0; i < ITERATIONS; i++) {
        uint8_t secret_key[OQS_SIG_dilithium_2_length_secret_key];
        uint8_t public_key[OQS_SIG_dilithium_2_length_public_key];
        OQS_SIG_dilithium_2_keypair(public_key, secret_key);

        uint8_t signature[OQS_SIG_dilithium_2_length_signature];
        size_t signature_len;
        OQS_SIG_dilithium_2_sign(signature, &signature_len, (uint8_t *)msg, msg_len, secret_key);

        double start = get_current_time();
        if (OQS_SIG_dilithium_2_verify((uint8_t *)msg, msg_len, signature, signature_len, public_key) != OQS_SUCCESS) {
            fprintf(stderr, "Verify failed at %d\n", i);
            return;
        }
        *total_time += get_current_time() - start;
    }
}

int main() {
    printf("--- Dilithium2 Performance Test (Profiling) ---\n");

    OQS_SIG *sig = OQS_SIG_dilithium_2_new();
    if (!sig) {
        fprintf(stderr, "Algorithm not supported!\n");
        return 1;
    }

    const char *message = "Profiling message";
    size_t message_len = strlen(message);
    double total_keygen = 0.0, total_sign = 0.0, total_verify = 0.0;

    test_keygen(sig, &total_keygen);
    test_sign(sig, message, message_len, &total_sign);
    test_verify(sig, message, message_len, &total_verify);

    printf("Averages over %d iterations:\n", ITERATIONS);
    printf("- KeyGen : %.6f s\n", total_keygen / ITERATIONS);
    printf("- Sign   : %.6f s\n", total_sign / ITERATIONS);
    printf("- Verify : %.6f s\n", total_verify / ITERATIONS);

    OQS_SIG_free(sig);
    return 0;
}

