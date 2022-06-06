#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "ray.h"

// public inheritance: make the parent/base class's public methods public in this child/derived class
//  and protected members in the base class remain protected in the derived class
class sphere : public hittable {
    public:
        // Class fields
        point3 center;
        double radius;
        // The material type of this sphere, which tells us how incident rays
        // that hit the sphere surface will be reflected/absored
        shared_ptr<material> mat_ptr;

        // Constructors
        sphere() {}
        sphere(point3 center, double radius, shared_ptr<material> m):
            center(center), radius(radius), mat_ptr(m) {};

        // Indicate that the virtual method will be implemented by replacing `= 0` with `override`
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

// Implementation of virtual function
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
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // Solve quadratic equation: ax^2 + bx + c = 0
    
    // A - C, where A is from the ray equation: P(t) = A + t*b
    // and C is the center
    vec3 oc = r.origin() - this->center;
    
    // Equivalent to: a = dot_product(r.direction(), r.direction()); // b^2 in ray function: P(t) = A + t*b
    double a = r.direction().length_squared();
    
    // b = 2h; h = b/2 "half_b"
    double half_b = dot_product(r.direction(), oc);
    
    // oc.length_squared = (A-C) dot (A-C)
    double c = oc.length_squared() - (this->radius * this->radius);
    
    // The value under the squared root in the quadtratic formula: b^2 - 4ac
    double discriminant = (half_b*half_b) - (a*c);
    if (discriminant < 0.0) {
        return false;
    } else {
        // Populate the hit_record

        // Solve the quadratic equation and compute the closest `t`
        //  within the range t_min and t_max
        // Compute the first root
        double sqrt_discriminant = sqrt(discriminant);
        double first_root = (-half_b - sqrt_discriminant) / a;
        if (first_root < t_min || first_root > t_max) {
            // The first root was out of range, so check the second root
            double second_root = (-half_b + sqrt_discriminant) / a;
            if (second_root < t_min || second_root > t_max) {
                // Both roots are out of range
                return false;
            } else {
                rec.t = second_root;
            }
        } else {
            rec.t = first_root;
        }

        // Get the normal vector at the point where the ray intersects the sphere
        rec.p = r.at(rec.t); // point of intersection
        
        // Get each component (x, y, z) to be in range -1 to 1 (unit vector)
        // The hit_point - sphere_center gives us the direction from the center to the surface
        //  where the ray intersects the sphere
        vec3 outward_normal = (rec.p - this->center) / radius;
        // Surface-side determination.
        //  Retain the information of where the ray came from (inside or outside the sphere)
        //   and set the normal to be in the opposite direction of the ray.
        rec.set_face_normal(r, outward_normal);

        // Set the material type of this sphere to the hit record
        rec.mat_ptr = this->mat_ptr;

        return true;
    }
}

// Create a bounding box that encapsulates this sphere
bool sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    // The slab of the bounding box for each axis:
    //  (center - radius) to (center + radius)
    output_box = aabb(
        this->center - point3(this->radius, this->radius, this->radius),
        this->center + point3(this->radius, this->radius, this->radius)
    );
    return true; 
}

#endif // header guard
