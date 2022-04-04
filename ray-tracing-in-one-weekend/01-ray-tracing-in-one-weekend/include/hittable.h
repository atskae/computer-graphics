#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

// Result if the ray hits the hittable object at `t`
struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    // True if the ray came from outside the sphere
    // False if the ray came from inside the sphere
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        // If the dot product is negative, the ray and the normal are facing different directions
        //  which means the ray came from outside the sphere
        this->front_face = dot_product(r.direction(), outward_normal) < 0;
        // We always want the ray and the normal to face the opposite direction.
        //  front_face will retaining the information of where the ray came from (outside or inside the sphere)
        this->normal = this->front_face ? outward_normal : -1 * outward_normal;
    }
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
