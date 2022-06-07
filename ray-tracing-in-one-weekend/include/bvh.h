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
    // Check if the ray even hits the tree's bounding box
    if (!this->box.hit(r, t_min, t_max)) {
        return false;
    }

    // Since the ray hits the bounding box, do a search through the node's children
    // to find the precise object that was hit
    hit_record temp_hit_record;
    // Check if the ray hits anything within the bounds defined by the left-half
    // Recall that the BVH does not split the space physically in two ordered halves (left and right halves)
    //  ex. the left-child of a BVH node does not literally mean the left-half of the space
    //  The objects referenced by the child node are inside the bounds defined by the parent node
    //  Bounding boxes can overlap
    bool hit_left = this->left.hit(r, t_min, t_max, temp_hit_record);
    
    // If there was an object on the left-half's bounds, use the t value that hit that object
    double t = hit_left ? temp_hit_record.t : t_max;

    bool hit_right = this->right.hit(r, t_min, t, temp_hit_record);

    // Update the return hit_record if an object was hit
    bool object_hit = hit_left || hit_right;
    if (object_hit) {
        rec = temp_hit_record;
    }

    return object_hit;
}

bool bvh::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = this->box;
    return true;
}

#endif // header guard
