#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

// Result if the ray hits the hittable object at `t`
struct hit_record {
    point3 p;
    vec3 normal;
    double t;
};


// Abstract class for objects that can be hit by a ray
//  ex. a sphere, a list of spheres
class hittable {
    // Given a ray P(t) = A + tb, return True if the ray hits this hittable object
    //  given an interval of the input `t`
    // method() = 0 means this function is a pure virtual function.
    //  subclasses must implement this method, otherwise they will remain abstract
    //  and cannot be instantiated
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif // header guard
