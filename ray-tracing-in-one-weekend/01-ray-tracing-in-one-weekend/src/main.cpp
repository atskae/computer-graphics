// I haven't written C++ since my youth
// TIL (3/5/22): red+green=yellow
// TIL (3/6/22): The imagick_r SPEC benchmark is ImageMagik
#include <string>
#include <iostream>

#include "vec3.h"
#include "color.h"

/*
    Prints a PPM file to standard out
*/
void print_ppm_file() {
    // Image size
    // Number of rows
    const int image_height = 256;
    // Number of columns
    const int image_width = 256;
    // PPM type (full color, ASCII encoding, ...)
    const std::string image_format = "P3";
    // The maximum value for the color intensities (red, green, and blue) of each pixel
    const int max_color_value = 255;

    // PPM header
    std::cout << image_format << std::endl;
    std::cout << image_width << ' ' << image_height << std::endl;
    std::cout << max_color_value << std::endl;

    for (int row=0; row<image_height; row++) {
        std::cerr << "\rRendering line " << row << "/" << (image_height-1) << ' ' << std::endl << std::flush;
        for (int col=0; col<image_width; col++) {
            // Generate a value between 0.0 and 1.0 for each color intensity (R, G, B)
            // This is so we get a proportion of the maximum color value afterwards
            // Subtract 1 from image dimensions since the maximum value is 255
            color pixel = color(
                double(row) / (image_height-1),
                double(col) / (image_width-1),
                0.25
            );
            write_color(std::cout, pixel);
        }
    }
    std::cerr << "Image generated." << std::endl;
}


int main() {
    print_ppm_file();
    return 0;
}
