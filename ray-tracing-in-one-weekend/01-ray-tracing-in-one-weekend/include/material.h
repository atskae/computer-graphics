#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

// Forward declaration (tells the C++ compiler that the actual definition
// is going to be defined in a different file)
struct hit_record;

// The material defines how the incident (incoming) ray is reflected/absored.
// (how the ray interacts with the surface)
// If the ray is scattered, the class defines how much the ray is attenuated
// (how much the light influences the hittable object) 
class material {
    public:
        // = 0 means pure virtual function
        // Return true if the reflected ray is outside the surface ???
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};

// Diffuse materials (ray is randomly scattered)
class lambertian: public material {
    public:
        // Class fields

        // Defines the ratio of reflected light over the amount of incident light
        // Denotes how bright a surface is
        color albedo;
        
        // Constructors
        lambertian(const color& a): albedo(a) {}

        // Implement abstract methods

        // Reflect in random direction (not influenced by the incoming ray)
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 scatter_direction = rec.normal + random_unit_vector();
            
            // If the vector is close to zero (which could result in undefined behavior later)
            // then use the normal vector
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }
            
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

// The angle between the incoming ray and the normal will be equal to
//  the angle between the reflected ray and the normal
//  (the law of reflection)
class metal : public material {
    public:
        // Class fields
        color albedo;
        // How perturbed the relfected ray is
        // Value is between 0.0 and 1.0
        // fuzz = radius of a smaller sphere that represents possible ray endpointst
        // Larger the fuzz (larger sphere radius), more "fuzzier" the reflection is
        //  since there is a larger area / possible points the ray can land
        double fuzz;

        // Constructors
        // If the fuzz value is outside the range (0.0, 1.0), then set it to 1.0
        //  to keep the fuzz in range
        metal(const color& a, const double fuzz): albedo(a), fuzz(fuzz < 1 ? fuzz : 1) {}

        // Methods

        // Implement virtual/abstract method
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            // The direction of the reflected ray
            // r_in might not be a unit vector
            vec3 direction = reflect(unit_vector(r_in.direction()), rec.normal);
            
            // Add a "fuzz" effect by changing the direction of the original ray
            // The angle out is now slightly unequal to the angle in
            direction = direction + this->fuzz * random_in_unit_sphere();
            
            // The reflected ray, starting at hit point `rec.p`
            scattered = ray(rec.p, direction);
            attenuation = this->albedo;
        
            bool is_outside_surface = dot_product(scattered.direction(), rec.normal) > 0;
            return is_outside_surface;
        }
};

#endif // header guard
