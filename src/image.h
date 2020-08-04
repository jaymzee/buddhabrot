#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>

struct image {
    int *buffer;
    int width;
    int height;
    char *comment;
};

#ifdef __cplusplus
extern "C" {
#endif
void write_image(const struct image *img, FILE *fp);
#ifdef __cplusplus
}
#endif

#endif /* IMAGE_H */
