#include <thread>
#include <cstdlib>
#include <cstdint>
#include "image.h"
#include "common.h"
#include "myrandom.h"

void render_task(const image *img,
                 const uint64_t samples,
                 const uint64_t max_iter,
                 uint32_t seed)
{
    int *const buf = img->buffer;
    const int w = img->width;
    const int h = img->height;
    double zr, zi, zr2, zi2, cr, ci;

    init_random(seed);

    for (uint64_t n = 0; n < samples; n++) {
        {
            const double rr = (double)get_random(&seed) / RAND_MAX;
            const double ri = (double)get_random(&seed) / RAND_MAX;
            cr = rr * (X1 - X0) + X0;
            ci = ri * (Y1 - Y0) + Y0;
        }
        /* find out if z escapes to infinity for this c */
        zr = zi = zr2 = zi2 = 0.0;
        for (uint64_t i = 0; i < max_iter; i++) {
            /* z = z * z + c */
            zi = 2.0 * zr * zi + ci;
            zr = zr2 - zi2 + cr;
            zr2 = zr * zr;
            zi2 = zi * zi;
            if (zr2 + zi2 > ESCAPE_MAG * ESCAPE_MAG) {
                /* this c escaped so reiterate the sequence
                   writing orbit to image this time */
                zr = zi = zr2 = zi2 = 0.0;
                while (zr2 + zi2 <= ESCAPE_MAG * ESCAPE_MAG) {
                    /* z = z * z + c */
                    zi = 2.0 * zr * zi + ci;
                    zr = zr2 - zi2 + cr;
                    zr2 = zr * zr;
                    zi2 = zi * zi;
                    /* map complex z back to image coordinates */
                    const int x = (int)((zr - X0) / (X1 - X0) * w);
                    const int y = (int)((zi - Y0) / (Y1 - Y0) * h);
                    if (x >= 0 && x < w && y >= 0 && y < h) {
                        buf[y * w + x]++;
                    }
                }
                break;
            }
        }
    }
}

void render_orbits(const struct image *final_img,
                   const uint64_t samples,
                   const uint64_t max_iter)
{
    using std::thread;

    char *comment = final_img->comment;
    const int width = final_img->width;
    const int height = final_img->height;
    const int threads = THREADS;
    
    fprintf(stderr, "starting %d worker threads\n", threads);

    // allocate images and threads
    thread *tsk = new thread[threads];
    image *img = new image[threads];
    for (int n = 0; n < threads; n++) {
        img[n].width = width;
        img[n].height = height;
        img[n].comment = NULL;
        img[n].buffer = new int[width * height]();
        tsk[n] = thread(render_task, &img[n], samples / threads, 
                        max_iter, RANDOM_SEED + n);
    }
    
    // wait for all threads to finish
    for (int n = 0; n < threads; n++) {
        tsk[n].join();
    }

    // show worker images
    //for (int n = 0; n < threads; n++) {
    //    write_image(&img[n], stdout);
    //}

    // merge results into final image
    for (int n = 0; n < threads; n++) {
        for (int i = 0; i < width * height; i++) {
            final_img->buffer[i] += img[n].buffer[i];
        }
    }

    // free images and threads
    delete[] tsk;
    for (int n = 0; n < threads; n++) {
        delete[] img[n].buffer;
    }
    delete[] img;
}
