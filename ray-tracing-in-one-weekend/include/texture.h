#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"

// Abstract base class for a texture
class texture {
    public:
        // (u,v) is the surface coordinate of the ray hit point
        virtual color value(double u, double v, const point3& p) const = 0;
}

// Constant texture
class solid_color : texture {
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
}

#endif // header guard
