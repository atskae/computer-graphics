#ifndef RTWEEKEND_H
#define RTWEEKEND_H

// Math constants and utilities for ray tracing

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>


// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * (pi/180.0);
}

// Return a random real number in [0, 1)
inline double random_double() {
    double num = rand() / (RAND_MAX + 1.0); // add +1 since excluding 1
    return num;
}

// Return a random real number in [min, max)
inline double random_double(double min, double max) {
    double num = min + (max-min)*random_double();
    //std::cerr << "I'm in rtweekend header duuude: " << num << std::endl;
    return num;
}

// Clamp a value within a range if the value goes out of range
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common Headers
#include "ray.h"
#include "vec3.h"

#endif // header guard
