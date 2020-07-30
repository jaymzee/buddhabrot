#include <stdlib.h>
#include <complex.h>
#include "image.h"
#include "spinner.h"
#include "common.h"

void render_orbits(const struct image *img, int samples, int max_iter)
{
    int *const buf = img->buffer;
    const int w = img->width;
    const int h = img->height;
    const int samp_chunk = samples / 1000;
    double complex z, c;

    srand(SEED);    /* seed random number generator */
    init_spinner(SPINNER_STR);
    for (int n = 0; n < samples; n++) {
        if (samp_chunk > 0 && (n % samp_chunk == 0)) {
            update_spinner((double)n / samples);
        }
        {
            double rre = (double)rand() / RAND_MAX;
            double rim = (double)rand() / RAND_MAX;
            c = (rre * (X1 - X0) + X0) + (rim * (Y1 - Y0) + Y0) * I;
        }
        /* find out if z escapes to infinity for this c */
        z = 0.0;
        for (int i = 0; i < max_iter; i++) {
            z = z * z + c;
            if (cabs(z) > ESCAPE_MAG) {
                /* this c escaped so reiterate the sequence
                   writing orbit to image this time */
                z = 0.0;
                while (cabs(z) <= ESCAPE_MAG) {
                    z = z * z + c;
                    /* map complex z back to image coordinates */
                    int x = (int)((creal(z) - X0) / (X1 - X0) * w);
                    int y = (int)((cimag(z) - Y0) / (Y1 - Y0) * h);
                    if (x >= 0 && x < w && y >= 0 && y < h) {
                        buf[y * w + x]++;
                    }
                }
                break;
            }
        }
    }
    finish_spinner(SPINNER_STR);
}
