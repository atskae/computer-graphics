#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "ray.h"
#include "aabb.h"

// Forward declaration of the the material class in material.h
class material;

// Result if the ray hits the hittable object at `t`
struct hit_record {
    point3 p;
    vec3 normal;
    // The type of material that was hit
    shared_ptr<material> mat_ptr;
    double t;
    
    // The location of the surface (the surface coordinate) that the ray hit the object
    // (u,v) is the texture coordiante: the position of the texture that the ray hit (?)
    double u;
    double v;
    
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
    public:
        // Given a ray P(t) = A + tb, return True if the ray hits this hittable object
        //  given an interval of the input `t`
        // method() = 0 means this function is a pure virtual function.
        //  subclasses must implement this method, otherwise they will remain abstract
        //  and cannot be instantiated
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;

        // Compute the bounding box that encloses this hittable
        // Individual primitives (like spheres) become the leaves in the hierarchy
        //  of bounding boxes.
        // If the object is a moving object, the bounding box will encapsulate the object
        //  for its entire time duration (time0 to time1)
        // If True, the bounding box was created. The bounding box will fail to be created
        //  for objects such as infinite planes
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};

#endif // header guard
