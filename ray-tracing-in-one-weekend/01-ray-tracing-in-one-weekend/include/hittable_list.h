#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include <memory> // shared_ptr

#include "hittable.h"

using std::shared_ptr;
using std::make_shared;


// A list of hittable objects
// A hittable is any object that can be intersected by a ray
class hittable_list : public hittable {
    public:
        // Class fields
        
        // The shared_ptr is a smart pointer; it will automatically free the memory
        //  of the object (the hittable) when no one points to the object anymore
        // Keeps track of the object with reference counting.
        std::vector<shared_ptr<hittable>> objects;

        // Constructors
        
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { this->add(object); }

        // Class methods
        
        // Remove all objects in the list
        void clear() { this->objects.clear(); }
        // Add a hittable to the list
        void add(shared_ptr<hittable> object) { this->objects.push_back(object); }

        // Indicate that the virtual function will be implemented
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

};

// If any of the objects in the list was hit by the ray, return True,
//  and set the hit_record of the closest object (the lowest t value that hits an object)
bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // Want to find the smallest t value (which is the closest object that was hit)
    double t_closest = t_max;
    bool hit_any_object = false;

    hit_record temp_rec;
    for (const shared_ptr<hittable>& object: this->objects) {
        // Check if there is a hit object that is closer than what we've already seen
        bool is_hit = object->hit(r, t_min, t_closest, temp_rec);
        if (is_hit) {
            hit_any_object = true;
            // Save the hit_record of this closer object
            t_closest = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_any_object;
}

#endif // header guard
