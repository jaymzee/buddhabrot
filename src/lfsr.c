#include <stdlib.h>
#include "lfsr.h"

#define POLY 0x800000000000000D

/* get a random number */
uint64_t random(uint64_t *seedp)
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
