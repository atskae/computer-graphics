#ifndef AABB_H
#define ABBB_H

#include "rtweekend.h"

// Axis-aligned bounding box
// Specifically: Axis-aligned bounding rectangular parallelepiped
class aabb {
    public:
        // The intervals of the AABB
        // The minimum value in each axis (x,y,z)
        point3 minimum;
        // The maximum value in each axis (x,y,z)
        point3 maximum;

        // Constructors
        aabb() {}
        aabb(const point3& a, const point3& b): minimum(a), maximum(b) {}

        // Getters
        point3 min() const { return minimum; }
        point3 max() const { return maximum; }

        // Original implementation of ray hitting the AABB
        //bool hit(const ray& r, double t_min, double t_max) const {
        //    // Use t_min and t_max as a running value (updated as we find more min/max values)
        //    // The two intervals we are always comparing is the current axis's intervals
        //    //  that are calculated in the loop (t_0, t_1) and (t_min, t_max)
        //    // For each axis (x,y,z)
        //    for (int a=0; a<3; a++) { // ... they all say, they love me on E-bay...
        //        // Calculate where the ray intersects this axis's slab/interval
        //        // First caluclate the values as intermediate values (_i)
        //        double t_0_i = (this->min()[a] - r.origin()[a]) / r.direction()[a];
        //        double t_1_i = (this->max()[a] - r.origin()[a]) / r.direction()[a];
        //        
        //        // We take the min() for t_0 and max for t_1 so that t_0 < t_1
        //        // These are the input values of the ray that intersect the slab of this axis
        //        double t_0 = fmax(t_0_i, t_1_i);
        //        double t_1 = fmin(t_0_i, t_1_i);

        //        // Update the minimum and maximum
        //        t_min = fmax(t_min, t_0);
        //        t_max = fmin(t_max, t_1);

        //        if (t_max <= t_min) {
        //            return false;
        //        }
        //    }
        //    return true;
        //}
};

#endif // header guard

// Optimized AABB hit method by Andrew Kensler at Pixar (すごい)
// The compiler optimizes this implementation well apparently
inline bool aabb::hit(const ray& r, double t_min, double t_max) const {
    // For each axis
    for (int a=0; a<3; a++) {
        // Reciprocal of the ray's direction
        // Also the denominator of finding the t value that intersects the slabs
        double inverse_direction = 1.0f / r.direction()[a];
        double t0 = (this->min()[a] - r.origin()[a]) * inverse_direction;
        double t1 = (this->max()[a] - r.origin()[a]) * inverse_direction;

        // If the ray was going into the negative direction,
        // we swap the intervals so that t0 < t1
        // Smaaawt.
        if (inverse_direction < 0.0f) {
            std::swap(t0, t1);
        }

        // Update the running min/max
        // If t0 is greater than t_min, set t_min to t0. Otherwise, set t_min to t_min
        // This logic is the same as fmax(t0, t_min), but optimized for the compiler? 🤷‍♀️
        t_min = t0 > t_min ? t0 : t_min;
        // tmin(t_1, t_max)
        t_max = t1 < t_max ? t1 : t_max;

        if (t_max <= t_min) {
            return false;
        }
    } // each axis; end

    return true;
}
