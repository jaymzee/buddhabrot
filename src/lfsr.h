#ifndef MYRANDOM_H
#define MYRANDOM_H

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif

#define LFSR_MAX 0xFFFFFFFF
#define LFSR_POLY 0xaa2255dd

/* get a random number */
uint64_t lfsr(uint64_t *seedp);

#ifdef __cplusplus
}
#endif

#endif /* MYRANDOM_H */
