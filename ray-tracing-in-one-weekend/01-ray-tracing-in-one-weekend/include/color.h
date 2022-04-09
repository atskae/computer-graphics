#ifndef COLOR_H
#define COLOR_H

#include "rtweekend.h"
#include "vec3.h"

#include <iostream>

// Write a single pixel's color channel values to standard out
// Squish the value (from "sigmoid squishification" - 3B1B) to range 0-255
// Apply anti-aliasing ??
void write_color(std::ostream &out, color pixel, int samples_per_pixel) {
    double r = pixel.r();
    double g = pixel.g();
    double b = pixel.b();

    // Divide each color channel value by the number of samples
    r /= samples_per_pixel;
    g /= samples_per_pixel;
    b /= samples_per_pixel;

    // Apply gamma 2 correction to account for the distortion
    //  from displaying image on a monitor
    //  This is simply raising each channel by a power 1/gamma
    //  = 1/2 = sqrt()
    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);

    // Write the translated [0, 255] value of each color component
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
    << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
    << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << std::endl;
}

// Header guard
#endif
