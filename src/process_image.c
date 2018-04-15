#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

// CHW format

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in

    // use clamp padding strategy if coords out of bounds
    if (x < 0) x = 0;
    if (x >= im.w) x = im.w - 1;
        
    if (y < 0) y = 0;
    if (y >= im.h) y = im.h - 1;

    if (c < 0) c = 0;
    if (c >= im.c) c = im.c - 1;

    int index = x + y*im.w + c*im.w*im.h;
    return im.data[index];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if ((x >= 0 && x < im.w) && (y >= 0 && y < im.h) && (c >= 0 && c < im.c)) {
        int index = x + y*im.w + c*im.w*im.h;
        im.data[index] = v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    
    memcpy(copy.data, im.data, im.h*im.w*im.c * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            int index = x + y*im.w;
            gray.data[index] = 0.299*im.data[index+ 0*im.w*im.h] + 0.587*im.data[index + 1*im.w*im.h] + 0.114*im.data[index+ 2*im.w*im.h];
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    if (c >= 0 && c < im.c) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                im.data[x + y*im.w + c*im.w*im.h] += v;
            }
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.w*im.h*im.c; i++) {
        if (im.data[i] < 0) im.data[i] = 0;
        if (im.data[i] > 1) im.data[i] = 1;
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            int index = x + y*im.w;
            float r = im.data[index + 0*im.w*im.h];
            float g = im.data[index + 1*im.w*im.h];
            float b = im.data[index + 2*im.w*im.h];

            float min = three_way_min(r, g, b);
            float max = three_way_max(r, g, b);
            float c = max - min;

            float v = max;
            float s = (v != 0) ? c / v : 0;
            float h;
            if (c == 0) { h = 0; } 
            else {
                if (v == r) { h = (g-b) / c; }
                else if (v == g) { h = ((b-r) / c) + 2; }
                else /* v == b */ { h = ((r-g) / c) + 4; }
            }
            h /= 6;
            if (h < 0) h++;

            im.data[index + 0*im.w*im.h] = h;
            im.data[index + 1*im.w*im.h] = s;
            im.data[index + 2*im.w*im.h] = v;
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            int index = x + y*im.w;
            float h = im.data[index + 0*im.w*im.h];
            float s = im.data[index + 1*im.w*im.h];
            float v = im.data[index + 2*im.w*im.h];

            float c = s * v;
            float max = v;
            float min = (v == c) ? 0.0 : v - c;

            float r, g, b;
            
            float h_ = h * 6;
            if (c == 0) // all values are the same
                { r = v; g = v; b = v; } 
            else if (h_ > 5 && h_ < 6) 
                { r = max; g = min; b = ((h_ / 6 - 1) * 6 * c - g) * -1; }
            else if (h_ == 5) /* max = b = r and min = g */ 
                { r = max; b = max; g = min; }
            else if (h_ < 5 && h_ > 4) /**/ 
                { b = max; g = min; r = (h_ - 4) * c + g; }
            else if (h_ == 4) /* max = b and min = r = g */ 
                { b = max; r = min; g = min; }
            else if (h_ < 4 && h_ > 3) /**/ 
                {b = max; r = min; g = ((h_ - 4) * c - r) * -1; }
            else if (h_ == 3) /* max = b = g and min = r */ 
                { b = max; g = max; r = min;}
            else if (h_ < 3 && h_ > 2) /**/ 
                { g = max; r = min; b = (h_ - 2) * c + r; }
            else if (h_ == 2) /**/ 
                { g = max; r = min; b = min; }
            else if (h_ < 2 && h_ > 1) /**/ 
                { g = max; b = min; r = ((h_ - 2) * c - b) * -1; }
            else if (h_ == 1) /**/ 
                { r = max; g = max; b = min; }
            else if (h_ < 1 && h_ > 0) /**/ 
                { r = max; b = min; g = h_ * c + b; }
            else /* if (h_ == 0) */ 
                { r = max; g = min; b = min; }

            im.data[index + 0*im.w*im.h] = r;
            im.data[index + 1*im.w*im.h] = g;
            im.data[index + 2*im.w*im.h] = b;

            // if (r < 0 || r > 1 || b < 0 || b > 1 || g < 0 || g > 1) {
            //     printf("(%d, %d): r %f, b %f, g %f", x, y, r, b, g);
            //     printf(" h %f, s %f, v %f, h_ %f, c %f, min %f, max %f \n", h, s, v, h_, c, min, max);
            // }
        }
    }
}