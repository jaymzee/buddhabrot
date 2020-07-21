#ifndef IMAGE_H
#define IMAGE_H

struct image {
    int *buffer;
    int width;
    int height;
};

#ifdef __cplusplus
extern "C" {
#endif
void write_image(const struct image *img, int samples, int max_iter);
#ifdef __cplusplus
}
#endif

#endif /* IMAGE_H */
