#ifndef RAY_H
#define RAY_H

#include "vec3.h"

/*
    A ray is a line function (remember y=mx+b?) in 3D space.

    A ray is a function P(t) = A + tb
 * A is the ray origin (starting point)
 * b is the ray direction
 * t is the input argument (a real number; in this program it is a double)
 * Positive/negative values of t will move P along the ray in different directions
 * P is the resulting 3D position along the ray/line
 */
class ray {
public:
// Class fields
// P(t) = A + tb; A=origin, b=direction
point3 orig;         // origin
vec3 dir;         // direction

ray() {
}
ray(const point3& origin, const vec3 direction) : orig(origin), dir(direction) {
}

// Getters
point3 origin() const {
  return orig;
}
vec3 direction() const {
  return dir;
}

// Compute position P(t) = A + tb
point3 at(double t) {
  return orig + t*dir;
}
} // class ray; end

// Header guard
#endif
