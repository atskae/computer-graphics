#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <cmath> // sqrt()

struct Vec3 {
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    // No swizzling here...
    Vec3(float x, float y, float z): x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3 v);
    Vec3 operator-(const Vec3 v);

    // Dot product
    float operator*(const Vec3 v);
    // Cross product
    Vec3 operator^(const Vec3 v);

    // Normalize the vector (length=1)
    void normalize();
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

inline void Vec3::normalize() {
    float length = sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
    this->x /= length;
    this->y /= length;
    this->z /= length;
}

// Compute the dot product
inline float Vec3::operator*(const Vec3 v) {
    return this->x*v.x + this->y*v.y + this->z*v.z;
}

// Compute the cross product
inline Vec3 Vec3::operator^(const Vec3 v) {
    return Vec3(
        this->y*v.z - this->z*v.y,
        this->z*v.x - this->x*v.z,
        this->x*v.y - this->y*v.x
    );
}

#endif // Header guard
