#ifndef COMMON_H
#define COMMON_H

#include "image.h"

#define SEED 42
#define SPINNER_STR "rendering buddhabrot"

/*
   X0,Y0,X1,Y1  define the coordinate range (lower left to upper right) of
                the region to render
   ESCAPE_MAG   for a conventional mandelbrot rendering is usually 2.0
                (magnitudes greater than this always escape to infinity)
                for the buddhabrot method, using a value of 2.3 with the
                coordinates below avoids a harsh circle near the edge.
*/

extern const double X0;    /* X is the real axis aligned horizontally */
extern const double X1;
extern const double Y0;    /* Y is the imaginary axis aligned vertically */
extern const double Y1;
extern const double ESCAPE_MAG;

#ifdef __cplusplus
extern "C" {
#endif
void render_orbits(const struct image *img, int samples, int max_iter);
#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */
