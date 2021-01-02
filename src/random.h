#ifndef MYRANDOM_H
#define MYRANDOM_H

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif

#define RANDOM_MAX 0xFFFFFFFF

/* get a random number */
uint32_t my_rand(uint32_t *seedp);

#ifdef __cplusplus
}
#endif

#endif /* MYRANDOM_H */
