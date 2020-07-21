#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum output { ASCII, PGM };
enum output output_type = ASCII;
int  maxiter = 256;
int  resx = 61;
int  resy = 21;
double x0 = -2.0;
double x1 = 1.0;
double y0 = 1.25;
double y1 = -1.25;

void arg_error(char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}

void proc_args(int argc, char *argv[]) {
    int i, j;
    char buf[80], *p, *r[4];

    for (i = 1; i < argc; i++) {
        if (strncmp("-m", argv[i], 2) == 0) {
            maxiter = atoi(argv[i] + 2);
        }
        if (strcmp("-p", argv[i]) == 0) {
            output_type = PGM;
        }
        if (strncmp("-w", argv[i], 2) == 0) {
            resx = atoi(argv[i] + 2);
        }
        if (strncmp("-h", argv[i], 2) == 0) {
            resy = atoi(argv[i] + 2);
        }
        if (strncmp("-r", argv[i], 2) == 0) {
            strncpy(buf, argv[i] + 2, 80);
            p = strtok(buf, ",");
            for (j = 0; j < 4; j++) {
                if (p == NULL) {
                    arg_error("range should be top left to bottom right"
                              ", example: -r-2.0,1.25,1.0,-1.25");
                } 
                r[j] = p;
                p = strtok(NULL, ",");
            }
            x0 = atof(r[0]);
            x1 = atof(r[2]);
            y0 = atof(r[1]);
            y1 = atof(r[3]);
        }
    }

    if (resx < 5) {
        arg_error("minimum x resolution is 10 pixels");
    }
    if (resy < 5) {
        arg_error("minimum y resolution is 10 pixels");
    }
    if (maxiter < 1 || maxiter > 1000000) {
        arg_error("maximum iterations must be between 1 and 1000000");
    }
}

void write_header(void) {
    if (output_type == ASCII && isatty(STDOUT_FILENO)) {
        /* clear the screen */
        fputs("\x1b[2J\x1b[0;0H", stdout); 
    }
    if (output_type == PGM) {
        printf("P2\n# iterations=%d", maxiter);
        printf(", coordinate range=(%g, %g)", x0, y0);
        printf(" to (%g, %g)\n", x1, y1);
        printf("%d %d\n255\n", resx, resy);
    }
}

void write_pixel(int n) {
    static int count = 0;
    int color;

    if (output_type == ASCII) {
        count += 1;
        if (n == maxiter) {
            putchar('*');
        } else {
            putchar(' ');
        }
        if (count >= resx) {
            putchar('\n');
            count = 0;
        }
    } else { /* PGM limit line length to 70 chars */
        count += 4;
        color = 255 * n / maxiter;
        /* color = 255 - color; */
        if (n == maxiter) {
           color = 0; 
        }

        printf("%3d ", color);
        if (count >= 64) { 
            putchar('\n');
            count = 0;
        }
    }
}

void write_footer(void) {
}

int main(int argc, char *argv[]) {
    double zr, zi, zr2, zi2, x, y, dx, dy;
    int n, j, i; 

    proc_args(argc, argv);
    write_header();

    dx = (x1 - x0) / (resx - 1);
    dy = (y1 - y0) / (resy - 1);
    
    /* (x, -2.0, 1.0, 0.025) (y, -1.25, 1.25, 0.0625) */
    for (y = y0, i = 0; i < resy; y += dy, i++) {
        for(x = x0, j = 0; j < resx; x += dx, j++) {
            zr = zi = zr2 = zi2 = 0.0;
            for (n = 0; n < maxiter; n++) {
                /* z = z^2 + c */
                zi = 2.0 * zr * zi + y;
                zr = zr2 - zi2 + x;
                zr2 = zr * zr;
                zi2 = zi * zi;
                /* test if |z| > 2 */
                if ((zr2 + zi2) > 4)
                    break;
            }
            write_pixel(n);
        }
    }

    write_footer();

    return 0;
}

