#ifndef IMAGE_H
#define IMAGE_H

struct image {
    int *buffer;
    int width;
    int height;
    char *comment;
    size_t comment_size;
};

#ifdef __cplusplus
extern "C" {
#endif
void write_image(const struct image *img);
#ifdef __cplusplus
}
#endif

#endif /* IMAGE_H */
