#ifndef AARECT_H
#define AARECT_H

// Axis-aligned rectangle

#include "rtweekend.h"
#include "hittable.h"

class xy_rect: public hittable {
    public:
        // "mp"
        shared_ptr<material> material_ptr;
        // Boundaries (lines) on the x-axis
        double x0, x1;
        // Boundaries on the y-axis
        double y0, y1;
        // The depth (z-axis) where the rectangle is
        // "k" // terrible name
        double z_depth;

        // Constructors
        xy_rect() {}
        xy_rect(double _x0, double _x1, double _y0, double _y1, double _depth, shared_ptr<material> _material_ptr):
            x0(_x0), x1(_x1), y0(_y0), y1(_y1), z_depth(_depth), material_ptr(_material_ptr) {}

        // Implement abstract base class virtual methods
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // Since this rectangle is flat (no depth), we need to add some padding in the bounding-box
            //  along the z-axis to give the bounding box some depth
            double padding = 0.0001;
            point3 lower_bounds = point3(this->x0, this->y0, this->z_depth - padding);
            point3 upper_bounds = point3(this->x1, this->y1, this->z_depth + padding);
            output_box = aabb(lower_bounds, upper_bounds);

            // Able to create bounding box, so return true
            return true;
        }
}

#endif // header guard
