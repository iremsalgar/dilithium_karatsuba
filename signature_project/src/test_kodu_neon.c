#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "oqs/oqs.h"

#define NTESTS 1000
#define MLEN 32
#define SIG_NAME "Dilithium2"

int main() {
    OQS_SIG *sig = NULL;
    uint8_t public_key[OQS_SIG_dilithium_2_length_public_key];
    uint8_t secret_key[OQS_SIG_dilithium_2_length_secret_key];
    uint8_t message[MLEN];
    uint8_t signature[OQS_SIG_dilithium_2_length_signature];
    size_t signature_len;
    OQS_STATUS rc;
    struct timespec start, end;
    long long elapsed_ns;

    // Algoritma başlatma
    sig = OQS_SIG_new(OQS_SIG_alg_dilithium_2);
    if (sig == NULL) {
        printf("Algoritma desteklenmiyor: %s\n", SIG_NAME);
        return 1;
    }

    // Rastgele mesaj üret
    OQS_randombytes(message, MLEN);

    // KeyPair Performans Testi
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < NTESTS; i++) {
        rc = OQS_SIG_keypair(sig, public_key, secret_key);
        if (rc != OQS_SUCCESS) {
            printf("Keypair oluşturulamadı!\n");
            OQS_SIG_free(sig);
            return 1;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("KeyPair: %.2f ns/op\n", (double)elapsed_ns / NTESTS);

    // İmzalama Performans Testi
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < NTESTS; i++) {
        rc = OQS_SIG_sign(sig, signature, &signature_len, message, MLEN, secret_key);
        if (rc != OQS_SUCCESS) {
            printf("İmzalama başarısız!\n");
            OQS_SIG_free(sig);
            return 1;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("Sign: %.2f ns/op\n", (double)elapsed_ns / NTESTS);

    // Doğrulama Performans Testi
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < NTESTS; i++) {
        rc = OQS_SIG_verify(sig, message, MLEN, signature, signature_len, public_key);
        if (rc != OQS_SUCCESS) {
            printf("Doğrulama başarısız!\n");
            OQS_SIG_free(sig);
            return 1;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("Verify: %.2f ns/op\n", (double)elapsed_ns / NTESTS);

    // Temizlik
    OQS_SIG_free(sig);
    return 0;
}
