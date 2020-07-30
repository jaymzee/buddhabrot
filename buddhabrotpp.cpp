#include <cstdio>
#include <cstdlib>
#include <complex>
#include "image.h"
#include "spinner.h"
#include "common.h"

void render_orbits(const struct image *img, int samples, int max_iter)
{
    int *const buf = img->buffer;
    const int w = img->width;
    const int h = img->height;
    const int samp_chunk = samples / 100;
    std::complex<double> z, c;

    srand(SEED);
    init_spinner(SPINNER_STR);
    for (int n = 0; n < samples; n++) {
        if (samp_chunk > 0 && (n % samp_chunk == 0)) {
            update_spinner((double)n / samples);
        }
        {
            double rre = (double)rand() / RAND_MAX;
            double rim = (double)rand() / RAND_MAX;
            c.real(rre * (X1 - X0) + X0);
            c.imag(rim * (Y1 - Y0) + Y0);
        }
        /* find out if z escapes to infinity for this c */
        z = 0.0;
        for (int i = 0; i < max_iter; i++) {
            z = z * z + c;
            if (abs(z) > ESCAPE_MAG) {
                /* this c escaped so reiterate the sequence
                   writing orbit to image this time */
                z = 0.0;
                while (abs(z) <= ESCAPE_MAG) {
                    z = z * z + c;
                    /* map complex z back to image coordinates */
                    int x = (int)((z.real() - X0) / (X1 - X0) * w);
                    int y = (int)((z.imag() - Y0) / (Y1 - Y0) * h);
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
