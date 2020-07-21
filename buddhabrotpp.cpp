#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <complex>
#include "common.h"
#include "image.h"

void render_orbits(const struct image *img, int samples, int max_iter)
{
    int *const buf = img->buffer;
    const int w = img->width;
    const int h = img->height;
    const int chunk = samples / 100;
    std::complex<double> z, c;

    fprintf(stderr, "rendering     ");
    for (int n = 0; n < samples; n++) {
        /* display progress bar */
        if (chunk > 0 && (n % chunk == 0)) {
            fprintf(stderr, "\b\b\b\b%3d%%", (int)(100.0 * n / samples));
        }
        /* get a random sample point for c */
        c.real((double)rand() / RAND_MAX * (X1 - X0) + X0);
        c.imag((double)rand() / RAND_MAX * (Y1 - Y0) + Y0);
        /* find out if z escapes to infinity for this c */
        z = 0;
        for (int i = 0; i < max_iter; i++) {
            z = z * z + c;
            if (abs(z) > ESCAPE_MAG) {
                /* this c escaped so reiterate the sequence but write orbit to image this time */
                z = 0;
                while (abs(z) <= ESCAPE_MAG) {
                    z = z * z + c;
                    /* map complex z back to image coordinates */
                    int x = (z.real() - X0) / (X1 - X0) * w;
                    int y = (z.imag() - Y0) / (Y1 - Y0) * h;
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
