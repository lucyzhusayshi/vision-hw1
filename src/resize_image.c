#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in

    // Nearest neighbor interpolation
    // Round to closest int, not type cast because C truncates towards 0
    int x_ = roundf(x);
    int y_ = roundf(y);
    return get_pixel(im, x_, y_, c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in 
    // newcoord = a*oldcoord + b
    image resized = make_image(w, h, im.c);
    float x_scalar = ((float)im.w)/ w;
    float y_scalar = ((float)im.h)/ h;
    float x_b = 0.5*x_scalar - 0.5;
    float y_b = 0.5*y_scalar - 0.5;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int pixel = x + y*w;
            for (int z = 0; z < im.c; z++) {
                float x_ = x_scalar*x + x_b;
                float y_ = y_scalar*y + y_b;
                resized.data[pixel + z*w*h] = nn_interpolate(im, x_, y_, z);
            }
        }
    }
    return resized;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    int left = floorf(x);
    int right = ceilf(x);
    int top = floorf(y);
    int bottom = ceilf(y);

    float q1 = (bottom - y)*get_pixel(im, left, top, c) + (y - top)*get_pixel(im, left, bottom, c);
    float q2 = (bottom - y)*get_pixel(im, right, top, c) + (y - top)*get_pixel(im, right, bottom, c);

    float q = (x - left)*q2 + (right - x)*q1;
    return q;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    image resized = make_image(w, h, im.c);
    float x_scalar = ((float)im.w)/ w;
    float y_scalar = ((float)im.h)/ h;
    float x_b = 0.5*x_scalar - 0.5;
    float y_b = 0.5*y_scalar - 0.5;
    
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int pixel = x + y*w;
            for (int z = 0; z < im.c; z++) {
                float x_ = x_scalar*x + x_b;
                float y_ = y_scalar*y + y_b;
                resized.data[pixel + z*w*h] = bilinear_interpolate(im, x_, y_, z);
            }
        }
    }
    return resized;
}

