#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.h"
#include "hittable.h"


// A sphere that has its center move linearly from center0 at time0
//  to center1 at time1
// Based off of sphere.h
class moving_sphere : public hittable {
    public:
        // The center of the sphere at time0 and time1, respectively
        point3 center0, center1;
        // Timestamps for the positions of the sphere centers: center0 and center1
        double time0, time1;
        double radius;
        // The material of the sphere
        shared_ptr<material> mat_ptr;

        moving_sphere() {}
        moving_sphere(
            point3 center0, point3 center1, double _time0, double _time1, double radius,
            shared_ptr<material> mat_ptr
        ):
        center0(center0), center(center1),
        time0(_time0), time1(time1),
        radius(radius), mat_ptr(mat_ptr) {};

        // Abstract method to override
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        // Return the center of the sphere at timestamp `time`
        point3 center(double time) const;
};

// Return the center of the sphere at timestamp `time`
point3 moving_sphere::center(double time) const {
    // Find what percent of the full time interval when `time` is at
    // time0 may not be `0`, which is why we have to subtract
    double time_interval = this->time1 - this->time0;
    double time_percent = (time - this->time0) / time_interval;

    // Use the same percentage of the full time interval 
    //  to the distance between center0 and center1
    double full_distance = this->center1 - this->center0;
    double distance = time_percent * full_distance;

    // Move `distance` from center0 to get the center at `time`
    return this->center0 + distance;
}

// Implementation of the virtual function hit()
// Nearly identical to the hit() method in sphere.h, but we get the center at timestamp `time`
bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // Get the current center at the time the ray was shot
    point3 current_center = this->center(r.time());
    vec3 oc = r.origin() - current_center;
    
    // Compute the components of the quadratic formula
    // Recall that when a line intersects a sphere, we can simplify the equations of the line
    //  and the sphere (after setting them equal to know where they intersect)
    //  and we eventually get a quadratic equation !!! (because we can plug in known constants)
    auto a = r.direction().length_squared();
    auto half_b = dot_product(oc, r.direction());
    auto c = oc.length_squared() - this->radius*this->radius;

    // The discriminant tells us how many points does the line intersect the sphere
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false; // no intersections

    auto sqrtd = sqrt(discriminant);

    // We know there is at least one root (point of intersection)
    // Find the closest point that lies in the acceptable range
    // Recall quadratic formula: +/- sqrt(b^2 ....), one root for + and one root for -
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max) {
        // If the root is out of range, try to see if the second root is within range
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max) {
            // Second root is not in range
            return false;
        }
    }

    // A valid intersection is found, populate the hit_record that describes this intersection
    // `t` is the point on the ray where it hit the sphere
    rec.t = root;
    rec.p = r.at(rec.t);
    
    // Normalize the normal vector
    auto outward_normal = (rec.p - current_center) / radius;
    
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = this->mat_ptr;
}

#endif // header guard
