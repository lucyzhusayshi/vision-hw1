#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    float total = 0;
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                total += im.data[x + y*im.w + c*im.w*im.h];
            }
        }
    }

    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                float val = total == 0 ? 1.0/(im.w*im.h) : im.data[x + y*im.w + c*im.w*im.h] / total;
                im.data[x + y*im.w + c*im.w*im.h] = val;
            }
        }
    }
}

image make_box_filter(int w)
{
    // TODO
    image filter = make_image(w, w, 1);
    l1_normalize(filter);
    return filter;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    assert(filter.c == 1 || filter.c == im.c);
    image convolved = make_image(im.w, im.h, im.c);

    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            for (int z = 0; z < im.c; z++) {
                int filter_c = filter.c == 1 ? 0 : z;
                for (int filter_y = 0; filter_y < filter.h; filter_y++) {
                    for (int filter_x = 0; filter_x < filter.w; filter_x++) {
                        float filter_val = filter.data[filter_x + filter_y*filter.w + filter_c*filter.w*filter.h];
                        int y_ = y - filter.h/2 + filter_y;
                        int x_ = x - filter.w/2 + filter_x;
                        //if (y_ >= 0 && x_ >= 0 && y_ < im.h && x_ < im.w) {
                            convolved.data[x + y*im.w + z*im.w*im.h] += get_pixel(im, x_, y_, z)*filter_val;
                            //set_pixel(convolved, x, y, z, convolved.data[x + y*im.w + z*im.w*im.h] + im.data[x_ + y_*im.w + z*im.w*im.h]*filter_val);
                        //}
                    }
                }
            }
        }
    }

    if (preserve) {
        return convolved;
    } else {
        image flat = make_image(im.w, im.h, 1);
        for (int h = 0; h < im.h; h++) {
            for (int w = 0; w < im.w; w++) {
                for (int c = 0; c < im.c; c++) {
                    flat.data[w + h*im.w] += convolved.data[w + h*im.w + c*im.w*im.h];
                }
            }
        }
        return flat;
    }
}

image make_highpass_filter()
{
    // TODO
    image filter = make_box_filter(3);
    filter.data[0] = 0;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 4;
    filter.data[5] = -1;
    filter.data[6] = 0;
    filter.data[7] = -1;
    filter.data[8] = 0;
    return filter;
}

image make_sharpen_filter()
{
    // TODO
    image filter = make_box_filter(3);
    filter.data[0] = 0;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 5;
    filter.data[5] = -1;
    filter.data[6] = 0;
    filter.data[7] = -1;
    filter.data[8] = 0;
    return filter;
}

image make_emboss_filter()
{
    // TODO
    image filter = make_box_filter(3);
    filter.data[0] = -2;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 1;
    filter.data[5] = 1;
    filter.data[6] = 0;
    filter.data[7] = 1;
    filter.data[8] = 2;
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    int w = (int)roundf(6 * sigma) % 2 == 0 ? (int)roundf(6 * sigma) + 1 : (int)roundf(6 * sigma);
    image filter = make_box_filter(w);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < w; y++) {
            int mid = w / 2;
            filter.data[x + y*w] = (1/(TWOPI*powf(sigma, 2)))*exp(-(pow(x - mid,2)+pow(y-mid,2))/(2*powf(sigma,2)));
        }
    }
    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    assert(a.h == b.h && a.w == b.w && a.c == b.c);
    // TODO
    image add = make_image(a.w, a.h, a.c);
    for (int x = 0; x < a.w; x++) {
        for (int y = 0; y < a.h; y++) {
            for (int z = 0; z < a.c; z++) {
                int pixel = x + y*a.w + z*a.w*a.h;
                add.data[pixel] = a.data[pixel] + b.data[pixel];
            }
        }
    }
    return add;
}

image sub_image(image a, image b)
{
    assert(a.h == b.h && a.w == b.w && a.c == b.c);
    // TODO
    image sub = make_image(a.w, a.h, a.c);
    for (int x = 0; x < a.w; x++) {
        for (int y = 0; y < a.h; y++) {
            for (int z = 0; z < a.c; z++) {
                int pixel = x + y*a.w + z*a.w*a.h;
                sub.data[pixel] = a.data[pixel] - b.data[pixel];
            }
        }
    }
    return sub;
}

image make_gx_filter()
{
    // TODO
    image filter = make_box_filter(3);
    filter.data[0] = -1;
    filter.data[1] = 0;
    filter.data[2] = 1;
    filter.data[3] = -2;
    filter.data[4] = 0;
    filter.data[5] = 2;
    filter.data[6] = -1;
    filter.data[7] = 0;
    filter.data[8] = 1;
    return filter;
}

image make_gy_filter()
{
    // TODO
    image filter = make_box_filter(3);
    filter.data[0] = -1;
    filter.data[1] = -2;
    filter.data[2] = -1;
    filter.data[3] = 0;
    filter.data[4] = 0;
    filter.data[5] = 0;
    filter.data[6] = 1;
    filter.data[7] = 2;
    filter.data[8] = 1;
    return filter;
}

void feature_normalize(image im)
{
    // TODO
    float max = im.data[0];
    float min = im.data[0];
    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            for (int z = 0; z < im.c; z++) {
                int i = x + y*im.w + z*im.w*im.h;
                max = im.data[i] > max ? im.data[i] : max;
                min = im.data[i] < min ? im.data[i] : min;
            }
        }
    }
    float range = max - min;
    if (range == 0) {
        for (int x = 0; x < im.w; x++) {
            for (int y = 0; y < im.h; y++) {
                for (int z = 0; z < im.c; z++) {
                    int i = x + y*im.w + z*im.w*im.h;
                    im.data[i] = 0;
                }
            }
        }
    } else {
        for (int x = 0; x < im.w; x++) {
            for (int y = 0; y < im.h; y++) {
                for (int z = 0; z < im.c; z++) {
                    int i = x + y*im.w + z*im.w*im.h;
                    im.data[i] -= min;
                    im.data[i] /= range;
                }
            }
        }
    }
}

image *sobel_image(image im)
{
    // TODO
    image *res = calloc(2, sizeof(image));
    res[0] = make_image(im.w, im.h, 1);
    res[1] = make_image(im.w, im.h, 1);

    image g_x = make_gx_filter();
    image g_y = make_gy_filter();

    g_x = convolve_image(im, g_x, 0);
    g_y = convolve_image(im, g_y, 0);

    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            int i = x + y*im.w;
            res[0].data[i] = sqrtf(powf(g_x.data[i], 2) + powf(g_y.data[i], 2));
            // res[1].data[i] = g_x.data[i] == 0 || g_y.data[i] == 0 ? 0 : atan2f(g_y.data[i], g_x.data[i]);
            res[1].data[i] = atan2f(g_x.data[i], g_y.data[i]);
            // if (y==3 && x == 29) {
            // printf("i %d gx %f", i, g_x.data[i]);
            // }
        }
    }
    return res;
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
