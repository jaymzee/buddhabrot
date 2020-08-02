/* WIN32 only supports the legacy rand function 
   but it seems under WIN32 this function is thread safe
   and probably uses thread local storage */

#ifdef _WIN32
/* initialize random number generator */
void init_random(unsigned int seed) { srand(seed); }

/* get a random number */
int get_random(unsigned int *seedp) { return rand(); }
#else
/* empty stub */
void init_random(unsigned int seed) {}
#endif