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

// Comparator function for a hittable using its bounding boxes
// If a<b, return True
bool box_compare(
    const shared_ptr<hittable> a,
    const shared_ptr<hittable> b,
    int axis,
) {
    // Obtain the bounding boxes of each hittable
    aabb box_a, box_b;
    bool a_valid, b_valid;
    a_valid = a.bounding_box(0, 0, box_a);
    b_valid = b.bounding_box(0, 0, box_b);

    // TODO: handle the case where both are invalid, or either one of them is invalid
    if (!a_valid || !b_valid) {
        std::cerr << "No bounding box in bvh_node constructor" << std::endl;
    }

    // Make the comparison with the axis value of the bounding boxes
    return box_a.min()[axis] < box_b.min()[axis];
}

// Axis-specific comparison functions
// We have to create these since the comparison function
//  can only take in the operands as arguments
bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}
bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}
bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

/*
    BVH constructor

    1. Randomly choose an axis (x,y,z)
    2. Sort the objects/primitives/hittables
    3. Split the sorted list in half and put each half into its own subtree (the child nodes)
*/
bvh_node::bvh_node(
    std::vector<shared_ptr<hittable>>& src_objects,
    size_t start, size_t end, double time0, double time1
) {
    // Create a copy of the objects (since we will modify its order)
    auto objects = src_objects;
    
    // 0, 1, 2 => x, y, z
    int axis = random_int(0, 2);

    // Sort the objects along the axis
    switch(axis) {
        case 0:
            std::sort(objects.at(start), objects.at(end), box_x_compare);
            break;
        case 1:
            std::sort(objects.at(start), objects.at(end), box_y_compare);
            break;
        default:
            std::sort(objects.at(start), objects.at(end), box_z_compare);
            break;
    }

    // TODO Put each half in the left/right sub-trees
}

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
