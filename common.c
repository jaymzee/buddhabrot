#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include "common.h"
#include "image.h"

/*
   X0,Y0,X1,Y1  define the coordinate range (lower left to upper right) of
                the region to render
   ESCAPE_MAG   for a conventional mandelbrot rendering is usually 2.0
                (magnitudes greater than this always escape to infinity)
                for the buddhabrot method, using a value of 2.3 with the
                coordinates below avoids a harsh circle near the edge.
*/

const double X0 = -1.70;
const double X1 =  0.75;
const double Y0 = -1.50;
const double Y1 =  1.50;
const double ESCAPE_MAG = 2.3;

void add_image_comments(const struct image *img,
                        int argc, char *argv[],
                        int samples, int max_iter)
{
    char buf[128];
    strcpy(img->comment, "# cmdline:");
    for (int i = 0; i < argc; i++) {
        strcat(img->comment, " ");
        strcat(img->comment, argv[i]);
    }
    sprintf(buf, "\n# size: %d x %d", img->width, img->height);
    strcat(img->comment, buf);
    sprintf(buf, "\n# samples: %d", samples);
    strcat(img->comment, buf);
    sprintf(buf, "\n# max iterations: %d", max_iter);
    strcat(img->comment, buf);
    sprintf(buf, "\n# seed: %d", SEED);
    strcat(img->comment, buf);
    sprintf(buf, "\n# escape magnitude: %g", ESCAPE_MAG);
    strcat(img->comment, buf);
    sprintf(buf, "\n# coordinates: %g + %gi to %g + %gi", X0,Y0,X1,Y1);
    strcat(img->comment, buf);
}

int main(int argc, char *argv[])
{
    struct image img = { NULL, 16, 16, NULL };
    int max_iter = 20;
    int samples = 1000;

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
    img.comment = malloc(4096);

    render_orbits(&img, samples, max_iter);
    add_image_comments(&img, argc, argv, samples, max_iter);
    write_image(&img);

    free(img.buffer);
    free(img.comment);

    return 0;
}
