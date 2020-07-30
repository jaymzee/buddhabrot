#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

enum output { ASCII, PGM, NP };
enum output output_type = ASCII;
int  MAX_ITER = 256;
int  RES_X = 61;
int  RES_Y = 21;
double X0 = -2.0;
double X1 = 1.0;
double Y0 = -1.25;
double Y1 = 1.25;

void arg_error(char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}

void proc_args(int argc, char *argv[]) {
    int i, j;
    char buf[80], *p, *r[4], *next_token;

    for (i = 1; i < argc; i++) {
        if (strncmp("-m", argv[i], 2) == 0) {
            MAX_ITER = atoi(argv[i] + 2);
        }
        if (strncmp("-f", argv[i], 2) == 0) {
            switch(argv[i][2]) {
            case 'p':
                output_type = PGM;
                break;
            case 'n':
                output_type = NP;
                break;
            default:
                output_type = ASCII;
            }
        }
        if (strncmp("-w", argv[i], 2) == 0) {
            RES_X = atoi(argv[i] + 2);
        }
        if (strncmp("-h", argv[i], 2) == 0) {
            RES_Y = atoi(argv[i] + 2);
        }
        if (strncmp("-r", argv[i], 2) == 0) {
            strncpy_s(buf, sizeof(buf), argv[i] + 2, _TRUNCATE);
            p = strtok_s(buf, ",", &next_token);
            for (j = 0; j < 4; j++) {
                if (p == NULL) {
                    arg_error("range should be bottom left to top right"
                              ", example: -r-2.0,1.25,1.0,-1.25");
                }
                r[j] = p;
                p = strtok_s(NULL, ",", &next_token);
            }
            X0 = atof(r[0]);
            X1 = atof(r[2]);
            Y0 = atof(r[1]);
            Y1 = atof(r[3]);
        }
    }

    if (RES_X < 5) {
        arg_error("minimum x resolution is 10 pixels");
    }
    if (RES_Y < 5) {
        arg_error("minimum y resolution is 10 pixels");
    }
    if (MAX_ITER < 1 || MAX_ITER > 1000000) {
        arg_error("maximum iterations must be between 1 and 1000000");
    }
}

void write_header(void) {
    /* clear screen
    if (output_type == ASCII && isatty(STDOUT_FILENO)) {
        fputs("\x1b[2J\x1b[0;0H", stdout);
    }
    */
    if (output_type == PGM) {
        printf("P2\n# iterations=%d", MAX_ITER);
        printf(", coordinate range=(%g, %g)", X0, Y0);
        printf(" to (%g, %g)\n", X1, Y1);
        printf("%d %d\n255\n", RES_X, RES_Y);
    }
    if (output_type == NP) {
        printf("# iterations=%d", MAX_ITER);
        printf(", coordinate range=(%g, %g)", X0, Y0);
        printf(" to (%g, %g)\n", X1, Y1);
    }
}

void write_pixel(int n) {
    static int count = 0;
    int color;

    if (output_type == ASCII) {
        count += 1;
        if (n == MAX_ITER) {
            putchar('*');
        } else {
            putchar(' ');
        }
        if (count >= RES_X) {
            putchar('\n');
            count = 0;
        }
    }
    if (output_type == PGM) {
        /* PGM limit line length to 70 chars */
        count += 4;
        color = 255 * n / MAX_ITER;
        /* color = 255 - color; */
        if (n == MAX_ITER) {
           color = 0;
        }

        printf("%3d ", color);
        if (count >= 64) {
            putchar('\n');
            count = 0;
        }
    }
    if (output_type == NP) {
        count += 1;
        color = n;
        printf("%d ", color);
        if (count >= RES_X) {
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

    dx = (X1 - X0) / (RES_X - 1);
    dy = (Y1 - Y0) / (RES_Y - 1);

    /* (x, -2.0, 1.0, 0.025) (y, -1.25, 1.25, 0.0625) */
    for (y = Y0, i = 0; i < RES_Y; y += dy, i++) {
        for(x = X0, j = 0; j < RES_X; x += dx, j++) {
            zr = zi = zr2 = zi2 = 0.0;
            for (n = 0; n < MAX_ITER; n++) {
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
