#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

// Write a single pixel's color channel values to standard out
// Squish the value (from "sigmoid squishification" - 3B1B) to range 0-255
void write_color(std::ostream &out, color pixel) {
    out << static_cast<int>(255 * pixel.r()) << ' '
    << static_cast<int>(255 * pixel.g()) << ' '
    << static_cast<int>(255 * pixel.b()) << std::endl;
}

// Header guard
#endif
