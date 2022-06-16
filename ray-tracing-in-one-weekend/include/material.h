#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
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

        // If the texture is a solid color,
        //  defines the ratio of reflected light over the amount of incident light
        //  Denotes how bright a surface is
        shared_ptr<texture> albedo;
        
        // Constructors
        lambertian(const color& a): albedo(make_shared<solid_color>(a)) {}
        lambertian(shared_ptr<texture> a): albedo(a) {}
        
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
            
            scattered = ray(rec.p, scatter_direction, r_in.time());
            // If the texture is a solid color, the hit record values are not used
            //  and a solid color is returned
            attenuation = albedo->value(rec.u, rec.v, rec.p);
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
            scattered = ray(rec.p, direction, r_in.time());
            attenuation = this->albedo;
        
            bool is_outside_surface = dot_product(scattered.direction(), rec.normal) > 0;
            return is_outside_surface;
        }
};

// Dielectric (water, glass); materials that refract light
class dielectric : public material {
    public:
        // Class fields
        double ir;
        
        // Constructor
        dielectric(double index_of_refraction): ir(index_of_refraction) {}

        // Implemented virtual functions
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            // Always white, since the glass surface absorbs nothing
            attenuation = color(1.0, 1.0, 1.0);
            
            // If the ray is coming from outside the sphere (ref.front_face=True)
            //  the medium of the incident (incoming) ray is the air,
            //  so its index of refraction is approximated to 1.0
            // Otherwise, the ray is coming from inside the sphere to outside (air)
            //  and the refraction ratio would be this->ir/1.0 (I think)
            double refraction_ratio = rec.front_face ? (1.0/this->ir) : this->ir;
            vec3 unit_direction = unit_vector(r_in.direction());
            
            // Check if this ray can indeed refract (obeys Snell's Law)
            double cos_theta = fmin(dot_product(-1 * unit_direction, rec.normal), 1.0);
            // Using trig identity sin^2(x) + cos^2(x) = 1
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 ray_direction;
            // Why a random double...?
            if (cannot_refract || this->reflectance(cos_theta, refraction_ratio) > random_double()) {
                ray_direction = reflect(unit_direction, rec.normal);
            } else {
                ray_direction = refract(unit_direction, rec.normal, refraction_ratio);
            }
            
            scattered = ray(rec.p, ray_direction, r_in.time());
        
            return true;
        }

    private:
        // Glass has reflectivity that depends on the angle of view(?)
        // Schlick Approximation
        // cosine = cosine of the incident ray's direction (from the normal)
        static double reflectance(double cosine, double refraction_idx) {
            double refraction_idx_air = 1.0;
            double r0 = (refraction_idx_air-refraction_idx) / (refraction_idx_air+refraction_idx);
            r0 = r0*r0;

            // Compute the specular reflection coefficient with Schlick's approximation
            return r0 + (1-r0) * pow(1-cosine, 5);
        }
};

#endif // header guard
