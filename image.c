#include <stdio.h>
#include "image.h"

void write_image(const struct image *img, FILE *fp)
{
    int *const buf = img->buffer;
    const int w = img->width;
    const int h = img->height;

    fprintf(fp, "%s\n",  img->comment);
    fprintf(fp, "# image size: %d x %d \n", w, h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            fprintf(fp, "%03d ", buf[y * w + x]);
        }
        fprintf(fp, "\n");
    }
}
