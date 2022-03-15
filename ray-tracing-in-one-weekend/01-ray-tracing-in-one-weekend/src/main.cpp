// I haven't written C++ since my youth
// TIL (3/5/22): red+green=yellow
// TIL (3/6/22): The imagick_r SPEC benchmark is ImageMagik
#include <string>
#include <iostream>

#include "vec3.h"
#include "ray.h"
#include "color.h"


// Print the PPM header
void print_ppm_header(std::string& image_format, int image_width, int image_height, int max_color_value) {
    std::cout << image_format << std::endl;
    std::cout << image_width << ' ' << image_height << std::endl;
    std::cout << max_color_value << std::endl;
}

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
    print_ppm_header(image_format, image_width, image_height, max_color_value);

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

// Return the color of the pixel where the ray points to.
// The overall effect is to generate a gradient of blue and white
//  with the color value dependent on the height (y-value) of the coordinate
color ray_color(const ray& r) {
    // Get the height (y-value) to range between -1.0 and 1.0
    vec3 unit_direction = unit_vector(r.direction());
}

void run_ray_tracer() {
    // Image dimensions
    const double aspect_ratio = 16.0 / 9.0; // width to height
    const int image_width = 400; // pixels
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    double viewport_height = 2.0;
    double viewport_width = viewport_height * aspect_ratio;
    // Distance from the eye to the viewport (depth)
    double focal_length = 1.0;

    point3 origin = point3(0, 0, 0);
    vec3 horizontal = vec3(viewport_width, 0, 0);
    vec3 vertical = vec3(0, viewport_height, 0);
    // Negative z-axis goes through the center of the viewport. 
    // To get the lower left corner of the viewport,
    //  we need to go "leftward" (negative) by viewport_width/2
    //  and go "downward" (negative) by viewport_height/2.
    //  The focal length defines how far the origin is from the viewport.
    point3 lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Render
    print_ppm_header("P3", image_width, image_height, 255);
    // Start from the upper-right corner of the image
    for (int j=image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i=0; i<image_width; ++i) {
            // "Squish" u and v to be in the range 0.0 to 1.0
            double u = double(i) / (image_width-1); // Ha, double u
            double v = double(j) / (image_height-1);
            // Create a ray that shoots out from the origin to the position on the viewport (?)
            vec3 direction = lower_left_corner + u*horizontal + v*vertical - origin;
            ray r = ray(origin, direction);
        }
    }
}

int main() {
    // print_ppm_file();
    run_ray_tracer();
    return 0;
}
