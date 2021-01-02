#include "lfsr.h"

#define POLY 0x80000000000019E2

/* get a random number */
uint64_t lfsr(uint64_t *seedp)
{
    uint64_t x = *seedp;

    int fb = x & 1;
    x = x >> 1;
    if (fb) {
        x = x ^ POLY;
    }
    *seedp = x;

    return x;
}
