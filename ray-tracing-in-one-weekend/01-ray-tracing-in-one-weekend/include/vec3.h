#ifndef VEC3_H
#define VEC3_H

// cmath puts all names in the `std` namespace
//  ex. std::pow(), std::sqrt()
#include <cmath>

class vec3 {
    public:
        // Constructors
        vec3(): e{0, 0, 0} {}
        vec3(double e0, double e1, double e2): e{e0, e1, e2} {}

        // Getter methods 
        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }
        
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
        vec3& operator+=(vec3& v) {
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

    public:
        double e[3];
};

// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3; // RGB color

// Header guard
#endif
