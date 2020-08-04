#include <stdlib.h>
#include <stdint.h>
#include "image.h"
#include "spinner.h"
#include "common.h"

/* render buddhabrot (c implementation) */
void render_orbits(const struct image *img,
                   const uint64_t samples,
                   const uint64_t max_iter)
{
    int *const buf = img->buffer;
    const int w = img->width;
    const int h = img->height;
    const uint64_t samp_chunk = samples / 1000;
    double zr, zi, zr2, zi2, cr, ci;

    srand(RANDOM_SEED);    /* seed random number generator */
    init_spinner(SPINNER_STR);
    for (uint64_t n = 0; n < samples; n++) {
        if (samp_chunk > 0 && (n % samp_chunk == 0)) {
            update_spinner((double)n / samples);
        }
        {
            const double rr = (double)rand() / RAND_MAX;
            const double ri = (double)rand() / RAND_MAX;
            cr = rr * (X1 - X0) + X0;
            ci = ri * (Y1 - Y0) + Y0;
        }
        /* find out if z escapes to infinity for this c */
        zr = zi = zr2 = zi2 = 0.0;
        for (uint64_t i = 0; i < max_iter; i++) {
            /* z = z * z + c */
            zi = 2.0 * zr * zi + ci;
            zr = zr2 - zi2 + cr;
            zr2 = zr * zr;
            zi2 = zi * zi;
            if (zr2 + zi2 > ESCAPE_MAG * ESCAPE_MAG) {
                /* this c escaped so reiterate the sequence
                   writing orbit to image this time */
                zr = zi = zr2 = zi2 = 0.0;
                while (zr2 + zi2 <= ESCAPE_MAG * ESCAPE_MAG) {
                    /* z = z * z + c */
                    zi = 2.0 * zr * zi + ci;
                    zr = zr2 - zi2 + cr;
                    zr2 = zr * zr;
                    zi2 = zi * zi;
                    /* map complex z back to image coordinates */
                    const int x = (int)((zr - X0) / (X1 - X0) * w);
                    const int y = (int)((zi - Y0) / (Y1 - Y0) * h);
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
