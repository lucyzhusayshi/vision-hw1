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
                float q = 0;
                for (int filter_y = 0; filter_y < filter.h; filter_y++) {
                    for (int filter_x = 0; filter_x < filter.w; filter_x++) {
                        float filter_val = filter.data[filter_x + filter_y*filter.w + filter_c*filter.w*filter.h];
                        int y_ = y - filter.h/2 + filter_y;
                        int x_ = x - filter.w/2 + filter_x;
                        q += get_pixel(im, x_, y_, z)*filter_val;
                    }
                }
                convolved.data[x + y*im.w + z*im.w*im.h] = q;
            }
        }
    }

    if (preserve) {
        return convolved;
    } else {
        image flat = make_image(im.w, im.h, 1);
        for (int h = 0; h < im.h; h++) {
            
            for (int w = 0; w < im.w; w++) {
                float q = 0;
                for (int c = 0; c < im.c; c++) {
                     q += convolved.data[w + h*im.w + c*im.w*im.h];
                }
                flat.data[w + h*im.w] = q;
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
// Answer: We should use preserve on sharpen and emboss and not highpass because we want to preserve color for sharpen and emboss. 

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: I had to do some post-processing (mainly clamping) for all filters above for them to look normal because applying the filters would lead to values outside the accepted range and loop to random/unintended colors. 

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
    printf("max %f, min %f, range %f \n", max, min, range);

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
                    if (i == 4276) {printf("-min %f ", im.data[i]);}
                    im.data[i] /= range;

                    if (i == 4276) {printf("/range %f \n", im.data[i]);}
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

    image filter_g_x = make_gx_filter();
    image filter_g_y = make_gy_filter();

    image g_x = convolve_image(im, filter_g_x, 0);
    image g_y = convolve_image(im, filter_g_y, 0);

    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            int i = x + y*im.w;
            res[0].data[i] = sqrtf(pow(g_x.data[i], 2) + pow(g_y.data[i], 2));
            res[1].data[i] = atan2(g_y.data[i], g_x.data[i]);
            // if (g_x.data[i] != 0 && g_y.data[i] == 0) {
            //     res[1].data[i] = atanf(g_x.data[i]/g_y.data[i]);
            // }
            
            
            // if (g_y.data[i] < 0 && g_x.data[i] < 0 && g_x.data[i] < -0.5) {
            //     res[1].data[i] = M_PI;
            // }
            // if (g_y.data[i] < 0 && g_x.data[i] < 0 && g_x.data[i] > -0.8 && g_y.data[i] > -0.00001) {
            //     res[1].data[i] = M_PI;
            // }
            // if (g_y.data[i] >= (float)0.0 && g_y.data[i] <= 0.00000020 && g_x.data[i] > -0.8 && g_x.data[i]) {
            //     res[1].data[i] *= -1;
            // }
            // // } else if (g_y.data[i] < 0 && g_x.data[i] > -1 && g_x.data[i] > ) {

            // }
            // if (i == 8587) {
            //     printf("theta %f x %f y %f30 \n",atan2f(g_y.data[i], g_x.data[i]),g_x.data[i],g_y.data[i]);
            //     printf("x/y %f \n", atan2f(0, 0));
            // }

        }
    }
    return res;
}

image colorize_sobel(image im)
{
    // TODO
    image *sobel = sobel_image(im);
    clamp_image(sobel[0]);
    image preserve = make_image(im.w,im.h,im.c);
    image invert = make_image(im.w,im.h,im.c);
    feature_normalize(sobel[0]);
    for (int xy = 0; xy < im.w*im.h; xy++) {
        for (int i = 0; i < im.c; i++) {
            invert.data[xy+i*im.w*im.h] = 1-im.data[i*im.w*im.h + xy];
            preserve.data[xy+i*im.w*im.h] = sobel[0].data[xy]; 
        }
    }
    clamp_image(preserve);
    image sub = sub_image(preserve, invert);
    clamp_image(sub);
    return sub;
}
