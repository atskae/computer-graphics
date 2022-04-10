// I haven't written C++ since my youth
// TIL (3/5/22): red+green=yellow
// TIL (3/6/22): The imagick_r SPEC benchmark is ImageMagik
#include <string>
#include <iostream>

#include "rtweekend.h" // vec3, ray

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"


// Print the PPM header
void print_ppm_header(const std::string& image_format, const int image_width, const int image_height, const int max_color_value) {
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
            write_color(std::cout, pixel, 1);
        }
    }
    std::cerr << "Image generated." << std::endl;
}


// Return the color of the pixel where the ray points to.
// If the ray does not hit the sphere, return the background color.
//  The background is a gradient of blue and white
//    with the color value dependent on the height (y-value) of the coordinate
color ray_color(const ray& r, const hittable_list& world, int depth) {
    // Base case
    if (depth <= 0) {
        // Return color that contributes no light.
        //  ¡¡Voy a ganar la liga Pokémooon!!
        //  ¡¡La voy a ganar cueste lo que cuesteee!!
        return color(0,0,0);
    }

    // Obtain where the ray intersects the sphere
    
    hit_record hit_rec = {};
    // Ignore rays that are near zero (ex. t=-0.000001 or t=0.0000001) to reduce "shadow acne"
    bool has_hit = world.hit(r, 0.001, infinity, hit_rec);
    
    // Base case 
    if (!has_hit) {
    
        // No intersection of the sphere
        // Return the background color
    
        // Get the height (y-value) to range between -1.0 and 1.0
        vec3 unit_direction = unit_vector(r.direction());
        // Get t to range 0.0 to 1.0
        double t = 0.5 * (unit_direction.y() + 1.0);
        
        // Scale t to range between 0.0 and 1.0
        // t=0.0 -> white; t=1.0 -> blue; in-between -> blend of white and blue
        // This trick is called a "linear blend", "linear interpolation", or "lerp" for short:
        //      blendedValue = (1-t)*startValue + t*endValue
        // where t goes from 0.0 to 1.0
        
        // Start color (we start at the bottom of the viewport)
        color white = color(1.0, 1.0, 1.0); // All channels (R, G, B) at 100%
        // End color (we end at the top of the viewport)
        color blue = color(0.5, 0.7, 1.0); // Blue channel at 100%, with other colors < 100%
    
        // Blue to white gradient, from top to bottom
        // blendedValue = (1-t)*startValue + t*endValue
        return (1.0-t)*white + t*blue;
    }
    
    // Ray has hit an object in the world
    // Recursively keep reflecting new rays until
    //  we don't hit an object surface or we reach the recursion depth

    // Use the populated hit_record to compute the color
    
    vec3 normal = hit_rec.normal;
    
    // The reflected ray will be a random ray within a unit sphere
    // from the hit point (rec.p)
    // rec.p + normal is the center of the unit sphere *outside the surface*
    //  When we add a random vector to the center, we get a point on the surface
    //  of this unit sphere (=target)
    point3 target = (hit_rec.p + normal) + random_unit_vector();
    // target - rec.p (hit point) gives us the direction from the hit point
    //  to the target
    // target - hit_rec.p is the vector from hit_rec.p to the target point
    ray reflected_ray = ray(hit_rec.p, target - hit_rec.p);

    // Return the sphere color
    // Keep recursively calling ray_color() until we don't hit any object 
    //  or we reach the maximum depth
    return 0.5 * ray_color(reflected_ray, world, depth-1);
    //std::cerr << "Hit point: " << hit_point << std::endl;
    //std::cerr << "Sphere center: " << SPHERE_CENTER << std::endl;
    //std::cerr << "Normal: " << normal << std::endl;
    //std::cerr << "Sphere color: " << sphere_color << std::endl;
    //std::cerr << "---" << std::endl;
    
}

void run_ray_tracer() {
    // Create a world, a list of objects that are hittable by a ray
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5)); // original sphere
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Image attributes 
    const double aspect_ratio = 16.0 / 9.0; // width to height
    const int image_width = 400; // pixels
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    // Maximum number of times a ray can keep reflecting off of a surface
    //  in a row
    const int max_depth = 50;

    // Camera
    const camera cam;

    // Render
    print_ppm_header("P3", image_width, image_height, 255);
    // Start from the upper-right corner of the image
    for (int j=image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i=0; i<image_width; ++i) {
            // Sample pixels around position pixel at position (i, j)
            // Taking the average of these samples creates an anti-aliasing effect
            color pixel_color(0, 0, 0);
            for (int s=0; s<samples_per_pixel; s++) {
                // "Squish" u and v to be in the range 0.0 to 1.0
                // Pixel = (u, v), where u is horizontal and v is vertical
                // Get a random neighboring pixel by adding a random_double() (which has range 0 to 1.0)
                double u = (double(i) + random_double()) / (image_width-1); // Ha, double u
                double v = (double(j) + random_double()) / (image_height-1);
                
                // Get the ray that points from camera origin to (u, v) in the viewport
                ray r = cam.get_ray(u, v);
                // Add this sample's color channel values
                // The average of all samples will be calculated by write_color()
                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
}

int main() {
    // print_ppm_file();
    run_ray_tracer();
    return 0;
}
