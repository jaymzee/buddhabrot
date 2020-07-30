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

double X0 = -1.70;
double X1 =  0.75;
double Y0 = -1.50;
double Y1 =  1.50;
double ESCAPE_MAG = 2.3;

void add_image_comments(const struct image *img,
                        int argc, char *argv[],
                        int samples, int max_iter)
{
    char buf[80];

    strcpy_s(img->comment, img->comment_size, "# cmdline:");
    for (int i = 0; i < argc; i++) {
        sprintf_s(buf, sizeof(buf), " %s", argv[i]);
        strcat_s(img->comment, img->comment_size, buf);
    }
    sprintf_s(buf, sizeof(buf), "\n# size: %d x %d",
              img->width, img->height);
    strcat_s(img->comment, img->comment_size, buf);
    sprintf_s(buf, sizeof(buf), "\n# samples: %d", samples);
    strcat_s(img->comment, img->comment_size, buf);
    sprintf_s(buf, sizeof(buf), "\n# max iterations: %d", max_iter);
    strcat_s(img->comment, img->comment_size, buf);
    sprintf_s(buf, sizeof(buf), "\n# seed: %d", SEED);
    strcat_s(img->comment, img->comment_size, buf);
    sprintf_s(buf, sizeof(buf), "\n# escape magnitude: %g", ESCAPE_MAG);
    strcat_s(img->comment, img->comment_size, buf);
    sprintf_s(buf, sizeof(buf), "\n# coordinates: %g + %gi to %g + %gi",
              X0,Y0,X1,Y1);
    strcat_s(img->comment, img->comment_size, buf);
}

void arg_error(const char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}

int main(int argc, char *argv[])
{
    struct image img = { NULL, 16, 16, NULL };
    int max_iter = 20;
    int samples = 1000;

    fprintf(stderr, "processing commandline args...");

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
        if (strncmp("-e", argv[i], 2) == 0) {
            ESCAPE_MAG = atof(argv[i] + 2);
        }
        if (strncmp("-r", argv[i], 2) == 0) {
            char buf[128], *p, *r[4], *next_token;
            strncpy_s(buf, sizeof(buf), argv[i] + 2, _TRUNCATE);
            p = strtok_s(buf, ",", &next_token);
            for (int j = 0; j < 4; j++) {
                if (p == NULL) {
                    arg_error("range should be bottom left to top right"
                              ", example: -r-2.0,-1.25,1.0,1.25");
                }
                r[j] = p;
                p = strtok_s(NULL, ",", &next_token);
            }
            X0 = atof(r[0]);
            Y0 = atof(r[1]);
            X1 = atof(r[2]);
            Y1 = atof(r[3]);
        }
    }
    fprintf(stderr, "done\n");

    fprintf(stderr, "allocating resources...");
    img.buffer = calloc(img.width * img.height, sizeof(*img.buffer));
    img.comment = malloc(4096); /* 4K is plenty of room for comments */
    img.comment_size = 4096;
    fprintf(stderr, "done\n");

    fprintf(stderr, "adding comments to image...");
    add_image_comments(&img, argc, argv, samples, max_iter);
    fprintf(stderr, "done\n");

    render_orbits(&img, samples, max_iter);

    fprintf(stderr, "writing image...");
    write_image(&img);
    fprintf(stderr, "done\n");

    fprintf(stderr, "freeing resources...");
    free(img.buffer);
    free(img.comment);
    fprintf(stderr, "done\n");

    return 0;
}

