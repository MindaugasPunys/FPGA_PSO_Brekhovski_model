#pragma once

#include "common_defs.hpp"

template <typename T> struct sConstaints_t {
    T min;
    T def;
    T max;
    ap_fixed_32p16 inertia;
    ap_fixed_32p16 personal_weight;
    ap_fixed_32p16 global_weight;
};

typedef struct sModelArgsMinMax_t {
    sConstaints_t<ap_fixed_32p16> alfa0;
    sConstaints_t<ap_fixed_64p32> freq0;
    sConstaints_t<ap_fixed_32p16> c2;
    sConstaints_t<ap_fixed_32p16> n;
    sConstaints_t<ap_fixed_32p16> ro2;
    sConstaints_t<ap_fixed_64p32> h;
} sModelArgsMinMax_t;

/* PRNG */
/* Xorshift16 */
static uint16_t RNG16 = 12345;
uint16_t xorshift16() {
    RNG16 ^= (RNG16 << 7);
    RNG16 ^= (RNG16 >> 9);
    RNG16 ^= (RNG16 << 8);
    return RNG16;
}
ap_fixed_32p16 PRNG_16() {
    ap_fixed_32p16 randomFloat = (xorshift16() & 0x7FFF);
    randomFloat = randomFloat >> 15;
    return randomFloat;
}

/* Xorshift32 */
static uint32_t RNG32 = 123456789;
uint32_t xorshift32() {
    RNG32 ^= (RNG32 << 13);
    RNG32 ^= (RNG32 >> 17);
    RNG32 ^= (RNG32 << 5);
    return RNG32;
}
ap_fixed_32p16 PRNG_32() {
    ap_fixed_32p16 randomFloat = (xorshift32() & 0x7FFFFFFF);
    randomFloat = randomFloat >> 31;
    return randomFloat;
}

/* Xorshift64 */
static uint64_t RNG64 = 123456789123456789;
uint64_t xorshift64() {
    RNG64 ^= (RNG64 << 13);
    RNG64 ^= (RNG64 >> 7);
    RNG64 ^= (RNG64 << 17);
    return RNG64;
}
ap_fixed_32p16 PRNG_64() {
    ap_fixed_32p16 randomFloat = (xorshift64() & 0x7FFFFFFFFFFFFFFF);
    randomFloat = randomFloat >> 63;
    return randomFloat;
}

void pso_util_print(const char *text, int i, sModelArgs args) {
    printf("%s[%-4d]: alfa0=%-10f, freq0=%-10f, c2=%-10f, n=%-10f, ro2=%-10f, h=%-10f\n", text, i,
           (float)args.alfa0, (float)args.freq0, (float)args.c2, (float)args.n,
           (float)args.ro2, (float)args.h);
}

void pso_util_print(const char *text, sModelArgs args) {
    printf("[PSO] %s: alfa0=%-10f, freq0=%-10f, c2=%-10f, n=%-10f, ro2=%-10f, h=%-10f\n", text,
           (float)args.alfa0, (float)args.freq0, (float)args.c2, (float)args.n,
           (float)args.ro2, (float)args.h);
}
