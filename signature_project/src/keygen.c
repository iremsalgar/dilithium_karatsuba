#include <stdio.h>
#include <stdint.h>
#include <oqs/oqs.h>

#define ITERATIONS 10000

int main() {
    printf("--- Keypair Profiling ---\n");

    OQS_SIG *sig = OQS_SIG_dilithium_2_new();
    if (sig == NULL) {
        printf("Dilithium 2 not supported!\n");
        return 1;
    }

    for (int i = 0; i < ITERATIONS; i++) {
        uint8_t public_key[OQS_SIG_dilithium_2_length_public_key];
        uint8_t secret_key[OQS_SIG_dilithium_2_length_secret_key];

        if (OQS_SIG_dilithium_2_keypair(public_key, secret_key) != OQS_SUCCESS) {
            printf("Keypair generation failed at iteration %d!\n", i);
            OQS_SIG_free(sig);
            return 1;
        }
    }

    OQS_SIG_free(sig);
    return 0;
}

