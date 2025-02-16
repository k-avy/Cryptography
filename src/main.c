#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "library/ecdsa.h"
#include "library/sha2.h"

// Define secp256k1 curve order as a 256-bit integer using an array
static const uint8_t SECP256K1_N[32] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xBA, 0xAE, 0xDC,
    0xE6, 0xAF, 0x48, 0xA0, 0x3B, 0xBF, 0xD2, 0x5E,
    0x8C, 0xD0, 0x36, 0x41, 0x41, 0x00, 0x00, 0x00
};

// Generate a random 32-byte number
void generate_random_bytes(uint8_t *buffer, size_t length) {
    FILE *fp = fopen("/dev/urandom", "rb");
    if (fp == NULL) {
        perror("Failed to open /dev/urandom");
        exit(EXIT_FAILURE);
    }
    fread(buffer, 1, length, fp);
    fclose(fp);
}

// Hash using SHA256
void sha256_hash(const uint8_t *input, size_t length, uint8_t *output) {
    sha256_Raw(input, length, output);
}

// Generate additive shares using Correlated Oblivious Transfer (COT)
void generate_additive_shares(uint8_t *a, uint8_t *b, uint8_t *c, uint8_t *d) {
    uint8_t hash_a[32], hash_b[32];

    // Hash inputs to simulate OT
    sha256_hash(a, 32, hash_a);
    sha256_hash(b, 32, hash_b);

    // Generate additive shares using modular arithmetic
    for (int i = 0; i < 32; i++) {
        uint64_t product = (uint64_t)a[i] * b[i];
        c[i] = (uint8_t)(product % SECP256K1_N[i]);
        d[i] = (uint8_t)((product - c[i]) % SECP256K1_N[i]);
    }
}

// Verify if the additive shares produce the product under the finite field
int verify_shares(uint8_t *a, uint8_t *b, uint8_t *c, uint8_t *d) {
    uint8_t product[32] = {0};
    uint8_t sum[32] = {0};

    // Calculate product (a * b) using modular arithmetic for secp256k1
    uint64_t carry = 0;
    for (int i = 31; i >= 0; i--) {
        __uint128_t val = (__uint128_t)a[i] * b[i] + carry;
        product[i] = (uint8_t)(val % SECP256K1_N[i]);
        carry = val / SECP256K1_N[i];
    }

    // Calculate sum of shares (c + d) with modular arithmetic
    carry = 0;
    for (int i = 31; i >= 0; i--) {
        __uint128_t val = (__uint128_t)c[i] + d[i] + carry;
        sum[i] = (uint8_t)(val % SECP256K1_N[i]);
        carry = val / SECP256K1_N[i];
    }

    // Compare arrays correctly with modular arithmetic applied
    for (int i = 0; i < 32; i++) {
        if (product[i] != sum[i]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    uint8_t a[32], b[32], c[32], d[32];

    // Generate random inputs for multiplicative shares
    generate_random_bytes(a, 32);
    generate_random_bytes(b, 32);

    printf("Input a: ");
    for (int i = 0; i < 32; i++) printf("%02x", a[i]);
    printf("\n");

    printf("Input b: ");
    for (int i = 0; i < 32; i++) printf("%02x", b[i]);
    printf("\n");

    // Generate additive shares
    generate_additive_shares(a, b, c, d);

    printf("Additive share c: ");
    for (int i = 0; i < 32; i++) printf("%02x", c[i]);
    printf("\n");

    printf("Additive share d: ");
    for (int i = 0; i < 32; i++) printf("%02x", d[i]);
    printf("\n");

    // Verify the result
    if (verify_shares(a, b, c, d)) {
        printf("Verification successful: c + d equals a * b under the finite field.\n");
    } else {
        printf("Verification failed: c + d does not equal a * b.\n");
    }

    return 0;
}
