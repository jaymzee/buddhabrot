#ifndef MYRANDOM_H
#define MYRANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

/* initialize random number generator */
void init_random(unsigned int seed);

#ifdef _WIN32
/* get a random number */
int get_random(unsigned int *seedp);
#else
#define get_random rand_r
#endif

#ifdef __cplusplus
}
#endif

#endif /* MYRANDOM_H */
