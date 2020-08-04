#include <cstdint>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <vector>
#include "common.h"
#include "image.h"
#include "myrandom.h"
#include "spinner.h"

void render_task(const image& img,
                 uint64_t samples,
                 uint64_t max_iter,
                 uint32_t seed,
                 uint64_t* progress)
{
    int *const buf = img.buffer;
    const int w = img.width;
    const int h = img.height;
    double zr, zi, zr2, zi2, cr, ci;

    init_random(seed);

    for (uint64_t n = 0; n < samples; n++) {
        *progress = n + 1;
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
                break; // break from for loop
            }
        }
    }
}

void render_orbits(const struct image *final_img,
                   const uint64_t samples,
                   const uint64_t max_iter)
{
    using std::thread;
    using std::vector;

    const int width = final_img->width;
    const int height = final_img->height;
    const uint64_t samples_per_thread = samples / THREADS;
    vector<thread> threads(THREADS);
    vector<image> render_image(THREADS, *final_img);
    vector<uint64_t> render_progress(THREADS);

    fprintf(stderr, "rendering with %zu threads\n", threads.size());
    fflush(stderr);

    // start worker threads
    for (int n = 0; n < THREADS; n++) {
        // allocate image buffer
        render_image[n].buffer = new int[width * height]();
        // start thread
        threads[n] = thread(render_task,
                            render_image[n],
                            samples_per_thread,
                            max_iter,
                            RANDOM_SEED + n,
                            &render_progress[n]);
    }

    // main thread shows spinner while waiting for worker threads to finish
    init_spinner(SPINNER_STR);
    for (uint64_t total; total < samples_per_thread * THREADS;) {
        total = 0;
        for (auto subtotal : render_progress) {
            total += subtotal;
        }
        update_spinner((double)total / samples);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    finish_spinner(SPINNER_STR);

    // wait for all threads to finish
    for (thread& thread : threads) {
        thread.join();
    }

    // show image from each worker thread
    // for (image& img : render_image) { write_image(&img, stdout); }

    // merge results into final image
    for (image& img : render_image) {
        for (int i = 0; i < width * height; i++) {
            final_img->buffer[i] += img.buffer[i];
        }
        // free image.buffer
        delete[] img.buffer;
    }
    fprintf(stderr, "rendering complete\n");
}
