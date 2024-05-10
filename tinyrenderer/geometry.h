#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

struct Vec3 {
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    // No swizzling here...
    Vec3(float x, float y, float z): x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3 v);
    Vec3 operator-(const Vec3 v);
};

std::ostream& operator << (std::ostream& o, const Vec3& vec) {
    o << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return o;
}

inline Vec3 Vec3::operator+(const Vec3 v) {
    return Vec3(this->x + v.x, this->y + v.y, this->z + v.z);
}

inline Vec3 Vec3::operator-(const Vec3 v) {
    return Vec3(this->x - v.x, this->y - v.y, this->z - v.z);
}

#endif // Header guard
