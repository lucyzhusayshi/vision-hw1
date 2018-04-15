from uwimg import *

# HW 0: Image Basics
# 1. Getting and setting pixels
im = load_image("data/dog.jpg")
for row in range(im.h):
    for col in range(im.w):
        set_pixel(im, col, row, 0, 0)
save_image(im, "dog_no_red")

# 3. Grayscale image
im = load_image("data/colorbar.png")
graybar = rgb_to_grayscale(im)
save_image(graybar, "graybar")

# 4. Shift Image
im = load_image("data/dog.jpg")
shift_image(im, 0, .4)
shift_image(im, 1, .4)
shift_image(im, 2, .4)
save_image(im, "overflow")

# 5. Clamp Image
clamp_image(im)
save_image(im, "doglight_fixed")

# 6-7. Colorspace and saturation
im = load_image("data/dog.jpg")
rgb_to_hsv(im)
shift_image(im, 1, .2)
clamp_image(im)
hsv_to_rgb(im)
save_image(im, "dog_saturated")

# # 8. Scale Image
# im = load_image("data/dog.jpg")
# rgb_to_hsv(im)
# scale_image(im, 1, 2)
# clamp_image(im)
# hsv_to_rgb(im)
# save_image(im, "dog_scale_saturated")

# # 9a. To HCL
# im = load_image("data/dog.jpg")
# rgb_to_hcl(im)
# save_image(im, "dog_hcl")

# # 9b. Back to RGB
# hcl_to_rgb(im)
# save_image(im, "dog_hcl_back_to_rgb")

# HW1
# 1. Image Resizing
im = load_image("data/dogsmall.jpg")
a = nn_resize(im, im.w*4, im.h*4)
save_image(a, "dog4x-nn")

# im = load_image("data/dogsmall.jpg")
# a = bilinear_resize(im, im.w*4, im.h*4)
# save_image(a, "dog4x-bl")

# im = load_image("data/dog.jpg")
# a = nn_resize(im, im.w//7, im.h//7)
# save_image(a, "dog7th-bl")

im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-box7")

im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
thumb = nn_resize(blur, blur.w//7, blur.h//7)
save_image(thumb, "dogthumb")

im = load_image("data/dog.jpg")
f = make_gaussian_filter(2)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-gauss2")

im = load_image("data/dog.jpg")
f = make_gaussian_filter(2)
lfreq = convolve_image(im, f, 1)
hfreq = im - lfreq
reconstruct = lfreq + hfreq
save_image(lfreq, "low-frequency")
save_image(hfreq, "high-frequency")
save_image(reconstruct, "reconstruct")

im = load_image("data/dog.jpg")
res = sobel_image(im)
mag = res[0]
feature_normalize(mag)
save_image(mag, "magnitude")