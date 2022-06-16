#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"

// Abstract base class for a texture
class texture {
    public:
        // (u,v) is the surface coordinate of the ray hit point
        virtual color value(double u, double v, const point3& p) const = 0;
};

// Constant texture
class solid_color : public texture {
    private:
        color color_value;

    public:
        // Constructors
        solid_color() {}
        solid_color(color c): color_value(c) {}
        solid_color(double red, double green, double blue): color_value(color(red, green, blue)) {}

        // Overrided virtual methods
        virtual color value(double u, double v, const point3& p) const override {
            return this->color_value;
        }
};

// Checker texture
class checker_texture: public texture {
    public:
        shared_ptr<texture> odd;
        shared_ptr<texture> even;

        // Constructors
        checker_texture() {}
        
        checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd):
            even(_even), odd(_odd) {}
        
        checker_texture(color _even, color _odd):
            even(make_shared<solid_color>(_even)), odd(make_shared<solid_color>(_odd)) {}
        
        // Implement abstract methods of parent class
        // Use the alternating sign of sine and cosine to create a checkered pattern ?!! (wasssss)
        virtual color value(double u, double v, const point3& p) const override {
            // TODO: why are we multiplying by 10???
            double sines = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());
            // TODO: isn't there a higher chance to be negative than positive???
            // You only need 1 negative value for the entire `sines` to be negative
            // But all coordinates need to be positive for `sines` to be positive
            if (sines < 0) {
                return odd->value(u, v, p);
            } else {
                return even->value(u, v, p);
            }
        }
};

#endif // header guard
