#include <stdio.h>
#include "image.h"

void write_image(const struct image *img)
{
    int *const buf = img->buffer;
    const int w = img->width;
    const int h = img->height;

    puts(img->comment);
    printf("# image size: %d x %d \n", w, h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            printf("%03d ", buf[y * w + x]);
        }
        printf("\n");
    }
}
