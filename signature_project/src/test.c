#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <oqs/oqs.h>

#define MLEN 32
#define NTESTS 1000

static inline uint64_t get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t) ts.tv_sec * 1000000000ull + ts.tv_nsec;
}

int main() {
    OQS_SIG *sig = OQS_SIG_dilithium_2_new();
    if (sig == NULL) {
        printf("Dilithium2 not supported!\n");
        return 1;
    }

    uint8_t msg[MLEN] = {0};
    uint8_t sig_buf[OQS_SIG_dilithium_2_length_signature];
    uint8_t pk[OQS_SIG_dilithium_2_length_public_key];
    uint8_t sk[OQS_SIG_dilithium_2_length_secret_key];
    size_t sig_len;

    uint64_t total_keygen = 0, total_sign = 0, total_verify = 0;

    for (int i = 0; i < NTESTS; i++) {
        uint64_t start, end;

        // Keygen
        start = get_time_ns();
        OQS_SIG_keypair(sig, pk, sk);
        end = get_time_ns();
        total_keygen += (end - start);

        // Sign
        start = get_time_ns();
        OQS_SIG_sign(sig, sig_buf, &sig_len, msg, MLEN, sk);
        end = get_time_ns();
        total_sign += (end - start);

        // Verify
        start = get_time_ns();
        int valid = OQS_SIG_verify(sig, msg, MLEN, sig_buf, sig_len, pk);
        end = get_time_ns();
        total_verify += (end - start);

        if (valid != OQS_SUCCESS) {
            printf("Signature verification failed at iteration %d!\n", i);
            return 1;
        }
    }

    printf("=== Dilithium2 %d Iterations ===\n", NTESTS);
    printf("Keypair avg: %.2f µs\n", total_keygen / (double)NTESTS / 1000.0);
    printf("Sign    avg: %.2f µs\n", total_sign / (double)NTESTS / 1000.0);
    printf("Verify  avg: %.2f µs\n", total_verify / (double)NTESTS / 1000.0);

    OQS_SIG_free(sig);
    return 0;
}

