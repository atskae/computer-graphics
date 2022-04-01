// I haven't written C++ since my youth
// TIL (3/5/22): red+green=yellow
// TIL (3/6/22): The imagick_r SPEC benchmark is ImageMagik
#include <string>
#include <iostream>

#include "vec3.h"
#include "ray.h"
#include "color.h"

const point3 SPHERE_CENTER = point3(0, 0, -1);
const double SPHERE_RADIUS = 0.5;


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
            write_color(std::cout, pixel);
        }
    }
    std::cerr << "Image generated." << std::endl;
}

// If the ray `r` intersects the sphere twice, with center `center` (a point in the x-y-z coordinate system)
//  and radius `radius`, then return the closest point of intersection. Otherwise, return -1.
// The quadtratic equation (ax^2 + bx + c = 0) we are trying to solve:
//  a = B^2 (where B is b in the ray equation: P(t) = A + t*b)
//  b = (2*B)(A-C) (where C is the sphere center)
//  c = (A-C)(A-C)
// The quadtratic equation in this function was simplified, knowing that b = 2h (b from the quadtratic formula),
//  and where h = B dot (A - C) (from the quadtratic equation we want to solve)
//  b = 2 * (B(A-C))
//  b = 2h -> h = B(A-C)
double hit_sphere(const point3& center, const double radius, const ray& r) {
    // Solve quadratic equation: ax^2 + bx + c = 0
    
    // A - C, where A is from the ray equation: P(t) = A + t*b
    // and C is the center
    vec3 oc = r.origin() - center;
    
    // Equivalent to: a = dot_product(r.direction(), r.direction()); // b^2 in ray function: P(t) = A + t*b
    double a = r.direction().length_squared();
    
    // b = 2h; h = b/2 "half_b"
    double half_b = dot_product(r.direction(), oc);
    
    // oc.length_squared = (A-C) dot (A-C)
    double c = oc.length_squared() - (radius * radius);
    
    // The value under the squared root in the quadtratic formula: b^2 - 4ac
    double discriminant = (half_b*half_b) - (a*c);
    if (discriminant < 0.0) {
        return -1;
    } else {
        // Solve the quadratic equation
        // Return the closest hit point (so only return the negative root)
        return (-half_b - sqrt(discriminant)) / a;
    }
}

// Return the color of the pixel where the ray points to.
// If the ray does not hit the sphere, return the background color.
//  The background is a gradient of blue and white
//    with the color value dependent on the height (y-value) of the coordinate
color ray_color(const ray& r) {
    
    // Obtain where the ray intersects the sphere. t=-1 if no intersection
    double t = hit_sphere(SPHERE_CENTER, SPHERE_RADIUS, r);
    if (t > 0.0) { // Ray hits the sphere
        // Get the normal vector at the point where the ray intersects the sphere
        point3 hit_point = r.at(t); // point of intersection
        
        // Get each component (x, y, z) to be in range -1 to 1 (unit vector)
        // The hit_point - sphere_center gives us the direction from the center to the surface
        //  where the ray intersects the sphere
        vec3 normal = unit_vector(hit_point - SPHERE_CENTER);
        
        // Map each component (x, y, z) to each color channel (R, G, B)
        //  with color channel range 0 to 1
        color sphere_color = 0.5 * color(normal.x()+1, normal.y()+1, normal.z()+1);
        //std::cerr << "Hit point: " << hit_point << std::endl;
        //std::cerr << "Sphere center: " << SPHERE_CENTER << std::endl;
        //std::cerr << "Normal: " << normal << std::endl;
        //std::cerr << "Sphere color: " << sphere_color << std::endl;
        //std::cerr << "---" << std::endl;
        return sphere_color;
    }

    // No intersection of the sphere
    // Return the background color

    // Get the height (y-value) to range between -1.0 and 1.0
    vec3 unit_direction = unit_vector(r.direction());
    // Get t to range 0.0 to 1.0
    t = 0.5 * (unit_direction.y() + 1.0);
    
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
            // Pixel = (u, v), where u is horizontal and v is vertical
            double u = double(i) / (image_width-1); // Ha, double u
            double v = double(j) / (image_height-1);
            // Create a ray that shoots out from the origin to the position on the viewport (?)
            vec3 direction = lower_left_corner + u*horizontal + v*vertical - origin;
            ray r = ray(origin, direction);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
}

int main() {
    // print_ppm_file();
    run_ray_tracer();
    return 0;
}
