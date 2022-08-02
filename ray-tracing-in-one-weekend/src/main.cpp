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
#include "moving_sphere.h"
#include "material.h"


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
    
    // Reflected ray
    ray scattered;
    // How much the incoming ray/light impacts the resulting color 
    color attenuation;

    // If the ray reflects outward from the surface
    if (hit_rec.mat_ptr->scatter(r, hit_rec, attenuation, scattered)) {
        return attenuation * ray_color(scattered, world, depth-1);
    } else {
        // The reflected ray inward (inside the surface), which means
        //  the ray is absorbed (???)
        return color(0,0,0);
    }
}

hittable_list image_texture_sphere(const char* filename) {
    shared_ptr<texture> earth_texture = make_shared<image_texture>(filename);
    shared_ptr<material> earth_surface = make_shared<lambertian>(earth_texture);
    shared_ptr<sphere> textured_sphere = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    return hittable_list(textured_sphere);
}


// Sphere with an earth pattern
hittable_list earth() {
    return image_texture_sphere("images/earthmap.jpeg");
}


// Create a world with two checkered spheres
hittable_list two_spheres() {
    hittable_list objects;
    color even(0.2, 0.3, 0.1);
    color odd(0.9, 0.9, 0.9);
    auto checker = make_shared<checker_texture>(even, odd);
    double radius = 10;
    objects.add(
        make_shared<sphere>(point3(0, -10, 0), radius, make_shared<lambertian>(checker))
    );
    objects.add(
        make_shared<sphere>(point3(0, 10, 0), radius, make_shared<lambertian>(checker))
    );
    return objects;
}

hittable_list tutorial_scene() {
    // Create a world, a list of objects that are hittable by a ray
    hittable_list world;
    
    shared_ptr<material> material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    shared_ptr<material> material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    // Blue sphere
    shared_ptr<material> material_left = make_shared<dielectric>(1.5);
    // Red sphere
    shared_ptr<material> material_right = make_shared<metal>(color(0.8,0.6,0.2), 0.0);

    world.add(
        make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground)
    );

    world.add(
        make_shared<sphere>(point3(0, 0, -1), 0.5, material_center)
    );

    // Hollow sphere
    world.add(
        make_shared<sphere>(point3(-1, 0, -1), 0.5, material_left)
    );

    world.add(
        make_shared<sphere>(point3(-1, 0, -1), -0.45, material_left)
    );

    world.add(
        make_shared<sphere>(point3(1, 0, -1), 0.5, material_right)
    );

    return world;
}

// Generate the cover of Ray Tracing in One Woche!
hittable_list random_scene() {
    hittable_list world;

    // Create the ground as a checkered texture
    color even = color(0.2, 0.3, 0.1);
    color odd = color(0.9, 0.9, 0.9);
    shared_ptr<checker_texture> checker = make_shared<checker_texture>(even, odd);
    shared_ptr<material> material_ground = make_shared<lambertian>(checker);
    world.add(
        make_shared<sphere>(point3(0, -1000, 0), 1000, material_ground)
    );

    // Create many spheres!
    // Iterate over the x and z axes to select where to place the spheres
    double y = 0.2;
    point3 ref(4, 0.2, 0);
    double radius = 0.2;
    // Moving sphere
    double time0 = 0.0;
    double time1 = 1.0;
    for (int x=-11; x<11; x++) {
        for (int z=-11; z<11; z++) {
            // Choose a random delta (<1) from the x and z coordinates
            // random_double() returns [0, 1)
            point3 center(x+0.9*random_double(), y, z+0.9*random_double());

            // Ensure small spheres don't intersect larger spheres to be rendered later? 
            if ((center-ref).length() > 0.9) {
                shared_ptr<material> sphere_material;
                
                // Choose a random value between 0 and 1 (exclusive)
                double choose_material = random_double();
                if (choose_material < 0.8) { // moving sphere
                    // Diffuse
                    // Why multiply ?
                    color albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    // Vertical motion up at some random height within range
                    point3 center1 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(
                        make_shared<moving_sphere>(
                            center, center1, time0, time1, radius, sphere_material
                        )
                    );
                } else { // normal sphere
                    if (choose_material < 0.95) {
                        // Metal
                        color albedo = color::random(0.5, 1);
                        double fuzz = random_double(0, 0.5);
                        sphere_material = make_shared<metal>(albedo, fuzz);
                    } else {
                        sphere_material = make_shared<dielectric>(1.5);
                    }

                    world.add(
                        make_shared<sphere>(center, radius, sphere_material)
                    );
                } // normal sphere; end
            } // generate sphere
        } // z-axis
    } // x-axis

    // Create three giant spheres
    radius = 1.0;
    
    shared_ptr<material> material1 = make_shared<dielectric>(1.5);
    world.add(
        make_shared<sphere>(point3(0,1,0), radius, material1)
    );

    shared_ptr<material> material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(
        make_shared<sphere>(point3(-4,1,0), radius, material2)
    );

    shared_ptr<material> material3 = make_shared<metal>(
        color(0.7, 0.6, 0.5), 0.0 // fuzz
    );
    world.add(
        make_shared<sphere>(point3(4,1,0), radius, material3)
    );

    return world;
}

hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto texture = make_shared<noise_texture>(4);
    objects.add(
        make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(texture))
    );
    objects.add(
        make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(texture))
    );

    return objects;
}

void run_ray_tracer() {
    
    // Image attributes 
    const double aspect_ratio = 16.0 / 9.0; // width to height
    const int image_width = 400; // pixels
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    // Maximum number of times a ray can keep reflecting off of a surface
    //  in a row
    const int max_depth = 50;
    
    // Our scene
    hittable_list world;
    // Camera settings depending on the scene
    point3 lookfrom = point3(13, 2, 3);
    point3 lookat = point3(0,0,0);
    double vfov = 20.0; // vertical field of view, in degrees
    double aperature = 0.0;

    switch(0) {
        case 1: { // Testing out this bracket thing here
            // Textbook cover
            world = random_scene();
            aperature = 0.1;
        } break;
        case 2:
            world = two_spheres();
            break;
        case 3:
            world = two_perlin_spheres();
            break;
        default:
        case 4:
            world = earth();
            //world = image_texture_sphere("images/fur-texture.jpeg");
            break;
    }

    // Camera
    vec3 view_up_vector = vec3(0,1,0);
    double dist_to_focus = 10.0;
    double time0 = 0.0;
    double time1 = 1.0;
    const camera cam(
        lookfrom, lookat, view_up_vector, vfov, aspect_ratio, aperature, dist_to_focus,
        time0, time1
    );

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
