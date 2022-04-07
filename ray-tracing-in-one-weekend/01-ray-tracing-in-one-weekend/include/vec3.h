#ifndef VEC3_H
#define VEC3_H

// cmath puts all names in the `std` namespace
//  ex. std::pow(), std::sqrt()
#include <cmath>

class vec3 {
    public:
        // Class fields
        double e[3];
        
        // Class methods

        // Constructors
        vec3(): e{0, 0, 0} {}
        vec3(double e0, double e1, double e2): e{e0, e1, e2} {}

        // Getter methods 
        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        double r() const { return this->x(); }
        double g() const { return this->y(); }
        double b() const { return this->z(); }

        // Operator overloading

        // Negative vec3 (note that this is not subtraction)
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        
        // Subscript/array indexing
        // Returns a copy of the value
        double operator[](int index) const { return e[index]; }
        // Returns a reference to the value (modifies the object)
        double& operator[](int index) { return e[index]; }

        // Sum two vectors together
        // vec3 v0 = vec3();
        // vec3 v1 = vec3();
        // v0 += v1; // v0 = v0 + v1
        vec3& operator+=(const vec3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];

            // this = pointer; *this = the object 
            // Since the return type is a reference to the object (vec3&)
            //  a reference is returned with *this
            return *this;
        }

        // Multiply a vector by a scalar
        // vec3 v = vec3();
        // v *= 5;
        vec3& operator*=(const double scalar) {
            e[0] *= scalar;
            e[1] *= scalar;
            e[2] *= scalar;
            return *this;
        }

        // Divide a vector by a scalar
        vec3& operator/=(const double denominator) {
            return *this *= 1/denominator;
        }

        // Math utilities

        // Compute the sum of the squares
        double length_squared() const {
            double length_sq = 0.0;
            for (int i=0; i<3; i++) {
                length_sq += pow(e[i], 2);
            }
            return length_sq;
        }

        double length() {
            return sqrt(length_squared());
        }
};

// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3; // RGB color

/*
    vec3 Utility functions
*/

// Pretty-print
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << "vec3(" << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << ")";
}

// Operator overloads
// Note that these return a new vec3 object

// Sum two vectors (vl=left, vr=right)
inline vec3 operator+(const vec3& vl, const vec3& vr) {
    return vec3(
        vl.e[0] + vr.e[0],
        vl.e[1] + vr.e[1],
        vl.e[2] + vr.e[2]
    );
}

// Subtract two vectors
inline vec3 operator-(const vec3& vl, const vec3& vr) {
    return vec3(
        vl.e[0] - vr.e[0],
        vl.e[1] - vr.e[1],
        vl.e[2] - vr.e[2]
    );
}

// Multiply two vectors
inline vec3 operator*(const vec3& vl, const vec3& vr) {
    return vec3(
        vl.e[0] * vr.e[0],
        vl.e[1] * vr.e[1],
        vl.e[2] * vr.e[2]
    );
}

// Multply a vector by a scalar as a left argument
inline vec3 operator*(const double scalar, const vec3& vr) {
    return vec3(
        scalar * vr.e[0],
        scalar * vr.e[1],
        scalar * vr.e[2]
    );
}

// Multply a vector by a scalar as a right argument
inline vec3 operator*(const vec3& vl, const double scalar) {
    return scalar * vl;
}

// Divide a vector by a scalar
inline vec3 operator/(vec3 v, double scalar) {
    return (1/scalar) * v;
}

// Compute the dot product
inline double dot_product(vec3 vl, vec3 vr) {
    double dot_product = 0.0;
    for (int i=0; i<3; i++) {
        dot_product += (vl.e[i] * vr.e[i]);
    }
    return dot_product;
}

// Compute the cross product
inline vec3 cross(vec3 vl, vec3 vr) {
    return vec3(
        vl.e[1] * vr.e[2] - vl.e[2] * vr.e[1],
        vl.e[2] * vr.e[0] - vl.e[0] * vr.e[2],
        vl.e[0] * vr.e[1] - vl.e[1] * vr.e[0]
    );
}

// Compute the unit vector (length=1)
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// Header guard
#endif
