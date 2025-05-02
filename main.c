#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "./stbi_images/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stbi_images/stb_image_resize2.h"

int calc_new_img_height(int width, int height, int new_width);
void print_to_ascii(int width, int height, int channels, unsigned char* image);

/*
    argv[1] - width desired for the ascii image
    argv[2] - path to the original image
*/
int main(int argc, char *argv[]) {
    int width, height, channels, new_width, new_height;
    unsigned char *img, *resized_img;
    char *image_path = argv[2];

    img = stbi_load(image_path, &width, &height, &channels, 0);

    // calc new dimensions and resize the image
    new_width = atoi(argv[1]);
    new_height = calc_new_img_height(width, height, new_width);
    resized_img = (unsigned char*) malloc(new_width * new_height * channels);
    stbir_resize_uint8_srgb(img, width, height, width*channels, resized_img, new_width, new_height, new_width*channels, channels);

    print_to_ascii(new_width, new_height, channels, resized_img);

    stbi_image_free(img);
    free(resized_img);
    return 0;
}

int calc_new_img_height(int width, int height, int new_width) {
    return (height * new_width) / width;
}

void print_to_ascii(int width, int height, int channels, unsigned char* image) {
    const char *chars = "@%#*+=-:. ";
    int len = sizeof(chars) - 1;
    char c;

    for(int i = 0; i < width*height*channels; i+=channels) {
        int grayscale_pixel = image[i]*0.299 + image[i+1]*0.587 + image[i+2]*0.144; // use the weighted average method to convert pixels rgb2grayscale
        int index = grayscale_pixel * (len -1) / 255;
        c = chars[index];

        printf("%c", c);

        // verify if the pixel is multiple of the width to break the line
        if(((i/channels)+1) % width == 0) {
            printf("\n");
        }
    }
}
