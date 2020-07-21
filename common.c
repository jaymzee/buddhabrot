#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include "common.h"
#include "image.h"

/*
   X0,Y0,X1,Y1 define the coordinate range (lower left to upper right) of the region to render
   ESCAPE_MAG for a conventional mandelbrot rendering is usually 2.0 (values outside this always escape to infinity)
   but for buddhabrot method setting this to 2.3 with the coordinates above avoids a harsh circle at the edge
*/

const double X0 = -1.70;
const double X1 =  0.75;
const double Y0 = -1.50;
const double Y1 =  1.50;
const double ESCAPE_MAG = 2.3;

int main(int argc, char *argv[])
{
    struct image img = { NULL, 16, 16 };
    int max_iter = 20;
    int samples = 1000;

    srand(42); /* initialize random number generator,  sequence returned by rand() */

    /* process command line arguments */
    for (int i = 1; i < argc; i++) {
        if (strncmp("-m", argv[i], 2) == 0) {
            max_iter = atoi(argv[i] + 2);
        }
        if (strncmp("-s", argv[i], 2) == 0) {
            samples = atoi(argv[i] + 2);
        }
        if (strncmp("-w", argv[i], 2) == 0) {
            img.width = atoi(argv[i] + 2);
        }
        if (strncmp("-h", argv[i], 2) == 0) {
            img.height = atoi(argv[i] + 2);
        }
    }

    img.buffer = calloc(img.width * img.height, sizeof(*img.buffer));
    render_orbits(&img, samples, max_iter);
    write_image(&img, samples, max_iter);
    free(img.buffer);

    return 0;
}
