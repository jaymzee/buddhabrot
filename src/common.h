#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include "image.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPINNER_STR "rendering buddhabrot"

/*
   X0,Y0,X1,Y1  define the coordinate range (lower left to upper right) of
                the region to render
   ESCAPE_MAG   for a conventional mandelbrot rendering is usually 2.0
                (magnitudes greater than this always escape to infinity)
                for the buddhabrot method, using a value of 2.3 with the
                coordinates below avoids a harsh circle near the edge.
*/

/* these are set by command line options */
extern double X0;    /* real axis lowest value */
extern double X1;    /* real axis highest value */
extern double Y0;    /* imaginary axis lowest value */
extern double Y1;    /* imaginary axis highest value */
extern double ESCAPE_MAG;
extern uint32_t RANDOM_SEED;
extern int THREADS;
extern const volatile char VERSION_STR[];
extern char EXEC_NAME[];

/* render buddhabrot */
void render_orbits(const struct image *img,
                   uint64_t samples,
                   uint64_t max_iter);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */
