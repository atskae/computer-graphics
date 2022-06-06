#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"

// The representation of a bounding box hierarchy (BVH)
// A container of hittables organized in a tree hierarchy
// Since this tree can answer the query "does this ray hit you?", it is a hittable
class bvh_node : public hittable {
    public:
        // Child pointers to any hittable (bvh_nodes, spheres, hittable_list, etc)
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        // The bounding box of this node
        aabb box;

        // Constructors
        bvh_node();
        // A wrapper to unpack the list of hittables and the list size
        bvh_node(
            const hittable_list& list, double time0, double time1
        ): bvh_node(list.objects, 0, list.objects.size(), time0, time1) {}
        // The actual constructor logic
        bvh_node(
            const std::vector<shared_ptr<hittable>>& source_objects,
            size_t start, size_t end, double time0, double time1
        );

        // Virtual functions to override
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

bool bvh::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // TODO: implement
}

bool bvh::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = this->box;
    return true;
}

#endif // header guard
