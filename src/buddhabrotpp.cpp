#include <cstdio>
#include <cstdlib>
#include <complex>
#include "image.h"
#include "spinner.h"
#include "common.h"
#include "lfsr.h"

/* render buddhabrot (using c++ complex type) */
void render_orbits(const struct image *img,
                   const uint64_t samples,
                   const uint64_t max_iter,
                   uint64_t seed)
{
    int *const buf = img->buffer;
    const int w = img->width;
    const int h = img->height;
    const uint64_t samp_chunk = samples / 1000;
    std::complex<double> z, c;

    init_spinner(SPINNER_STR);
    for (uint64_t n = 0; n < samples; n++) {
        if (samp_chunk > 0 && (n % samp_chunk == 0)) {
            update_spinner((double)n / samples);
        }
        {
            const double rre = (double)lfsr(&seed) / (double)LFSR_MAX;
            const double rim = (double)lfsr(&seed) / (double)LFSR_MAX;
            c.real(rre * (X1 - X0) + X0);
            c.imag(rim * (Y1 - Y0) + Y0);
        }
        /* find out if z escapes to infinity for this c */
        z = 0.0;
        for (uint64_t i = 0; i < max_iter; i++) {
            z = z * z + c;
            if (abs(z) > ESCAPE_MAG) {
                /* this c escaped so reiterate the sequence
                   writing orbit to image this time */
                z = 0.0;
                while (abs(z) <= ESCAPE_MAG) {
                    z = z * z + c;
                    /* map complex z back to image coordinates */
                    const int x = (int)((z.real() - X0) / (X1 - X0) * w);
                    const int y = (int)((z.imag() - Y0) / (Y1 - Y0) * h);
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
