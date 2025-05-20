#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <oqs/oqs.h>

#define ITERATIONS 10000

int main() {
    printf("--- Sign Profiling ---\n");

    OQS_SIG *sig = OQS_SIG_dilithium_2_new();
    if (sig == NULL) {
        printf("Dilithium 2 not supported!\n");
        return 1;
    }

    uint8_t public_key[OQS_SIG_dilithium_2_length_public_key];
    uint8_t secret_key[OQS_SIG_dilithium_2_length_secret_key];
    if (OQS_SIG_dilithium_2_keypair(public_key, secret_key) != OQS_SUCCESS) {
        printf("Keypair generation failed!\n");
        OQS_SIG_free(sig);
        return 1;
    }

    const char *message = "Test message for signing.";
    size_t message_len = strlen(message);

    for (int i = 0; i < ITERATIONS; i++) {
        uint8_t signature[OQS_SIG_dilithium_2_length_signature];
        size_t signature_len;

        if (OQS_SIG_dilithium_2_sign(signature, &signature_len, (uint8_t *)message, message_len, secret_key) != OQS_SUCCESS) {
            printf("Signing failed at iteration %d!\n", i);
            OQS_SIG_free(sig);
            return 1;
        }
    }

    OQS_SIG_free(sig);
    return 0;
}

