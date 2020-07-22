#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include "common.h"
#include "image.h"

double complex get_random_c(void)
{
    double rre = (double)rand() / RAND_MAX;
    double rim = (double)rand() / RAND_MAX;
    return (rre * (X1 - X0) + X0) + (rim * (Y1 - Y0) + Y0) * I;
}

void render_orbits(const struct image *img, int samples, int max_iter)
{
    int *const buf = img->buffer;
    const int w = img->width;
    const int h = img->height;
    const int chunk = samples / 100;
    double complex z, c;

    srand(42);    /* seed random number generator */
    fprintf(stderr, "rendering     ");
    for (int n = 0; n < samples; n++) {
        if (chunk > 0 && (n % chunk == 0)) {
            /* display progress bar */
            fprintf(stderr, "\b\b\b\b%3d%%", (int)(100.0 * n / samples));
        }
        c = get_random_c();
        /* find out if z escapes to infinity for this c */
        z = 0.0;
        for (int i = 0; i < max_iter; i++) {
            z = z * z + c;
            if (cabs(z) > ESCAPE_MAG) {
                /* this c escaped so reiterate the sequence but write orbit to image this time */
                z = 0.0;
                while (cabs(z) <= ESCAPE_MAG) {
                    z = z * z + c;
                    /* map complex z back to image coordinates */
                    int x = (creal(z) - X0) / (X1 - X0) * w;
                    int y = (cimag(z) - Y0) / (Y1 - Y0) * h;
                    if (x >= 0 && x < w && y >= 0 && y < h) {
                        buf[y * w + x]++;
                    }
                }
                break;
            }
        }
    }
    fprintf(stderr, "\b\b\b\b100%%\n");
}
