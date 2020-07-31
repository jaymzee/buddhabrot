#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <stdint.h>
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

double X0 = -1.70;
double X1 =  0.75;
double Y0 = -1.50;
double Y1 =  1.50;
double ESCAPE_MAG = 2.3;

void add_image_comments(const struct image *img,
                        int argc, char *argv[],
                        uint64_t samples, uint64_t max_iter)
{
    char buf[80];

    strcpy(img->comment, "# cmdline:");
    for (int i = 0; i < argc; i++) {
        sprintf(buf, " %s", argv[i]);
        strcat(img->comment, buf);
    }
    sprintf(buf, "\n# size: %d x %d",
              img->width, img->height);
    strcat(img->comment, buf);
    sprintf(buf, "\n# samples: %llu", (unsigned long long)samples);
    strcat(img->comment, buf);
    sprintf(buf, "\n# max iterations: %llu", (unsigned long long)max_iter);
    strcat(img->comment, buf);
    sprintf(buf, "\n# seed: %d", SEED);
    strcat(img->comment, buf);
    sprintf(buf, "\n# escape magnitude: %g", ESCAPE_MAG);
    strcat(img->comment, buf);
    sprintf(buf, "\n# coordinates: %g + %gi to %g + %gi",
              X0,Y0,X1,Y1);
    strcat(img->comment, buf);
}

void arg_error(const char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    struct image img = { NULL, 16, 16, NULL };
    uint64_t max_iter = 20;
    uint64_t samples = 1000;

    if (argc == 1) {
        fprintf(stderr, "Usage: %s [options]\n", argv[0]);
        fprintf(stderr, "options:\n");
        fprintf(stderr, "    -i100    maximum number of iterations\n");
        fprintf(stderr, "    -s10000  number of sample points\n");
        fprintf(stderr, "    -w1000   width of image\n");
        fprintf(stderr, "    -h1000   height of image\n");
        fprintf(stderr, "    -e2.3    %s\n", 
                "magnitude of z considered as escaping to infinity");
        fprintf(stderr, "    -r-1.7,-1.5,0.75,1.5   z coordinate range\n");
        exit(1);
    }

    /* process command line arguments */
    for (int i = 1; i < argc; i++) {
        if (strncmp("-i", argv[i], 2) == 0) {
            max_iter = strtoll(argv[i] + 2, NULL, 10);
        }
        if (strncmp("-s", argv[i], 2) == 0) {
            samples = strtoll(argv[i] + 2, NULL, 10);
        }
        if (strncmp("-w", argv[i], 2) == 0) {
            img.width = atoi(argv[i] + 2);
        }
        if (strncmp("-h", argv[i], 2) == 0) {
            img.height = atoi(argv[i] + 2);
        }
        if (strncmp("-e", argv[i], 2) == 0) {
            ESCAPE_MAG = atof(argv[i] + 2);
        }
        if (strncmp("-r", argv[i], 2) == 0) {
            char buf[128], *p, *r[4], *next_token;
            strncpy(buf, argv[i] + 2, sizeof(buf));
            p = strtok(buf, ",");
            for (int j = 0; j < 4; j++) {
                if (p == NULL) {
                    arg_error("range should be bottom left to top right"
                              ", example: -r-2.0,-1.25,1.0,1.25");
                }
                r[j] = p;
                p = strtok(NULL, ",");
            }
            X0 = atof(r[0]);
            Y0 = atof(r[1]);
            X1 = atof(r[2]);
            Y1 = atof(r[3]);
        }
    }

    img.buffer = calloc(img.width * img.height, sizeof(*img.buffer));
    img.comment = malloc(4096); /* 4K is plenty of room for comments */

    add_image_comments(&img, argc, argv, samples, max_iter);

    render_orbits(&img, samples, max_iter);

    write_image(&img);

    free(img.buffer);
    free(img.comment);

    return 0;
}

