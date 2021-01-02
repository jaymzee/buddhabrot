#include "lfsr.h"

/* get a random number */
uint64_t lfsr(uint64_t *seedp)
{
    uint64_t x = *seedp;

    int fb = x & 1;
    x = x >> 1;
    if (fb) {
        x = x ^ LFSR_POLY;
    }
    *seedp = x;

    return x;
}
