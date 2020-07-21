#ifndef COMMON_H
#define COMMON_H

#include "image.h"

/*
   X0,Y0,X1,Y1 define the coordinate range (lower left to upper right) of the region to render
   ESCAPE_MAG for a conventional mandelbrot rendering is usually 2.0 (values outside this always escape to infinity)
   but for buddhabrot method setting this to 2.3 with the coordinates above avoids a harsh circle at the edge
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
