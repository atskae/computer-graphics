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

        bool hit(const ray& r, double t_min, double t_max) const {
            // Use t_min and t_max as a running value (updated as we find more min/max values)
            // The two intervals we are always comparing is the current axis's intervals
            //  that are calculated in the loop (t_0, t_1) and (t_min, t_max)
            // For each axis (x,y,z)
            for (int a=0; a<3; a++) { // ... they all say, they love me on E-bay...
                // Calculate where the ray intersects this axis's slab/interval
                // First caluclate the values as intermediate values (_i)
                double t_0_i = (this->minimum[a] - r.origin()[a]) / r.direction()[a];
                double t_1_i = (this->maximum[a] - r.origin()[a]) / r.direction()[a];
                
                // We take the min() for t_0 and max for t_1 so that t_0 < t_1
                // These are the input values of the ray that intersect the slab of this axis
                double t_0 = fmax(t_0_i, t_1_i);
                double t_1 = fmin(t_0_i, t_1_i);

                // Update the minimum and maximum
                t_min = fmin(t_min, t_0);
                t_max = fmin(t_max, t_1);

                if (t_max <= t_min) {
                    return false;
                }
            }
            return true;
        }
};

#endif // header guard
