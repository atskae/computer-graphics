#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

struct Vec3 {
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    // No swizzling here...
};

std::ostream& operator << (std::ostream& o, const Vec3& vec) {
    o << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return o;
}

#endif // Header guard
