#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <stdint.h>
#ifdef __unix__
#include <libgen.h>  /* for basename */
#endif
#include "common.h"
#include "image.h"

/*
   X0,Y0,X1,Y1  define the coordinate range (lower left to upper right) of
                the region to render
   ESCAPE_MAG   for a conventional mandelbrot rendering is usually 2.0
                (magnitudes greater than this always escape to infinity)
                for the buddhabrot method, using a value of 2.2 with the
                coordinates below avoids a harsh circle near the edge.
*/

double X0 = -1.7;
double X1 =  0.9;
double Y0 = -1.3;
double Y1 =  1.3;
double ESCAPE_MAG = 2.2;
int THREADS = 2;    /* Number of worker threads */
/* version baked in during build */
const volatile char VERSION_STR[] = VERSION;
/* executable filename */
char EXEC_NAME[256] = "buddhabrot";

void add_image_comments(const struct image *img,
                        int argc, char *argv[],
                        uint64_t samples,
                        uint64_t max_iter,
                        uint32_t seed)
{
    char buf[80];

    strcpy(img->comment, "# cmdline: ");
    strcat(img->comment, EXEC_NAME);
    for (int i = 1; i < argc; i++) {
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
    sprintf(buf, "\n# seed: %llu", (unsigned long long)seed);
    strcat(img->comment, buf);
    sprintf(buf, "\n# escape magnitude: %g", ESCAPE_MAG);
    strcat(img->comment, buf);
    sprintf(buf, "\n# coordinates: %g + %gi to %g + %gi",
              X0,Y0,X1,Y1);
    strcat(img->comment, buf);
}

void show_usage(void)
{
    fprintf(stderr, "Usage: %s [options] [filename]\n", EXEC_NAME);
    fprintf(stderr, "options:\n");
    fprintf(stderr, "    -i100    maximum number of iterations\n");
    fprintf(stderr, "    -s10000  number of sample points\n");
    fprintf(stderr, "    -d42     random seed\n");
    fprintf(stderr, "    -w1000   width of image\n");
    fprintf(stderr, "    -h1000   height of image\n");
    fprintf(stderr, "    -e2.2    %s\n",
            "magnitude of z considered as escaping to infinity");
    fprintf(stderr, "    -r-1.7,-1.3,0.9,1.3   z coordinate range\n");
    fprintf(stderr, "    -t2      number of worker threads\n");
    fprintf(stderr, "    -v       show version\n");
}

int main(int argc, char *argv[])
{
    struct image img = { NULL, 16, 16, NULL };
    uint64_t max_iter = 20;
    uint64_t samples = 1000;
    uint32_t seed = 42;
    const char *fname = NULL;

    /* extract basename of executable */
    if (argc > 0) {
#ifdef __unix__
        strcpy(EXEC_NAME, basename(argv[0]));
#elif defined(_WIN32)
        _splitpath(argv[0], NULL, NULL, EXEC_NAME, NULL);
#else
        strcpy(EXEC_NAME, argv[0]);
#endif
    }

    /* Usage: */
    if (argc <= 1) {
        show_usage();
        exit(1);
    }

    /* process command line arguments */
    for (int i = 1; i < argc; i++) {
        if (strncmp("-i", argv[i], 2) == 0) {
            max_iter = strtoll(argv[i] + 2, NULL, 10);
        } else if (strncmp("-s", argv[i], 2) == 0) {
            samples = strtoll(argv[i] + 2, NULL, 10);
        } else if (strncmp("-w", argv[i], 2) == 0) {
            img.width = atoi(argv[i] + 2);
        } else if (strncmp("-h", argv[i], 2) == 0) {
            img.height = atoi(argv[i] + 2);
        } else if (strncmp("-d", argv[i], 2) == 0) {
            seed = atoi(argv[i] + 2);
        } else if (strncmp("-e", argv[i], 2) == 0) {
            ESCAPE_MAG = atof(argv[i] + 2);
        } else if (strncmp("-t", argv[i], 2) == 0) {
            THREADS = atoi(argv[i] + 2);
        } else if (strncmp("-r", argv[i], 2) == 0) {
            char buf[128], *p, *r[4], *next_token;
            strncpy(buf, argv[i] + 2, sizeof(buf));
            p = strtok(buf, ",");
            for (int j = 0; j < 4; j++) {
                if (p == NULL) {
                    fprintf(stderr,
                            "range should be bottom left to top right"
                            ", example: -r-2.0,-1.25,1.0,1.25\n");
                    exit(1);
                }
                r[j] = p;
                p = strtok(NULL, ",");
            }
            X0 = atof(r[0]);
            Y0 = atof(r[1]);
            X1 = atof(r[2]);
            Y1 = atof(r[3]);
        } else if (strncmp("-v", argv[i], 2) == 0) {
            fprintf(stderr, "%s version %s\n", EXEC_NAME, VERSION_STR);
            exit(0);
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "unknown option %s\n", argv[i]);
            exit(1);
        } else if (!fname) {
            fname = argv[i];
        } else {
            show_usage();
            exit(1);
        }
    }

    /* allocate resources */
    FILE *outfile = stdout;
    if (fname) {
        outfile = fopen(fname, "w");
        if (!outfile) {
            perror(fname);
            exit(1);
        }
    }
    img.buffer = calloc(img.width * img.height, sizeof(*img.buffer));
    img.comment = malloc(4096); /* 4K is plenty of room for comments */

    /* render image */
    add_image_comments(&img, argc, argv, samples, max_iter, seed);
    render_orbits(&img, samples, max_iter, seed);

    /* write image */
    write_image(&img, outfile);

    /* cleanup */
    if (fname) {
        fclose(outfile);
    }
    free(img.buffer);
    free(img.comment);

    return 0;
}
