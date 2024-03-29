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

        // Generate a random vector, with each axis in range, (0,1]
        inline static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        // Generate a random vector, with each axis in range (min,max]
        inline static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }

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

        // Return true if values in all dimensions are close to zero
        // Used to check if some vector computation may result in undefined behavior (NaN, infinity, etc).
        bool near_zero() const {
            // If the value is less than this, than it is considered "close to zero"
            double threshold = 1e-8;
            // fabs() returns the absolute value of a double
            return (fabs(this->e[0]) < threshold) && (fabs(this->e[1]) < threshold) && (fabs(this->e[1]) < threshold);
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

// Generate a random vector within a unit sphere
// This first generates a random vector in a unit cube,
//  then rejects the vector if its length > 1 (the vector is outside the sphere).
//  Repeat until we get a vector within the sphere.
vec3 random_in_unit_sphere() {
    while(true) {
        vec3 v = vec3::random(-1, 1);
        //if (v.length() >= 1) continue;
        if (v.length_squared() >= 1) continue;
        return v;
    }
}

// Return a random vector of length 1
vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

// Generate a random vector that is pointing in the same
// "hemisphere" (top half of sphere, or bottom half of sphere)
// as the normal vector
vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    // Check if the random vector is pointing in the same hemisphere
    // as the normal
    if (dot_product(normal, in_unit_sphere) > 0.0) {
        // Positive dot product = in the same hemisphere
        return in_unit_sphere;
    } else {
        // Negative dot product = in opposite hemisphere; flip signs
        // to make the vector in the same hemisphere as the normal
        return -1 * in_unit_sphere;
    }
}

// Pick a random point in the unit disk (same as a unit circle with the center at the origin)
// The disk is a circle with the `lookfrom` point of the camera as its center
// Called the "defocus disk", everything outside the disk is blurred 
vec3 random_in_unit_disk() {
    while(true) {
        vec3 p = vec3(random_double(-1, 1), random_double(-1,1), 0);
        if (p.length_squared() >=1) continue;
        return p;
    }
}

// Return the reflected vector, which has the same angle to the normal
// but its direction is reflected
vec3 reflect(const vec3& v, const vec3 normal) {
    // dot(v, normal) is the projection of vector v onto the normal
    //  Since v and the normal are facing in opposite directions, the result
    //  of the dot product will be negative.
    //  We multiply this result by -1 so the intermediate vector b is facing
    //  the same direction as the normal
    vec3 b = -1 * dot_product(v, normal) * normal; // dot() is the projection length, normal gives direction
    
    // v+b would give the direction along the surface
    // v+2b will give the direction of the relfected vector
    return v + 2*b;
}

// Refracts the incident ray (incoming ray) and returns the transmitted (refracted) ray
// eta_i_over_eta_t is the refraction of indices of the medium of the incidient over
//  the index of the medium of the transmitted ray
vec3 refract(const vec3& incident_ray, const vec3& normal, double eta_i_over_eta_t) {
    // fmin() returns the smallest of its arguments
    // The -1 in (-1 * incident_ray) is required because the dot product between
    //  the incident ray and the normal would be negattive (we want cos_theta to be positive)
    double cos_theta = fmin(dot_product(-1 * incident_ray, normal), 1.0);

    // Compute the transmitted ray vector with its two components: parallel and perpendicular
    vec3 transmitted_ray_perpendicular = eta_i_over_eta_t * (incident_ray + cos_theta*normal);
    vec3 transmitted_ray_parallel = -1 * normal * (sqrt( fabs(1 - transmitted_ray_perpendicular.length_squared()) ));

    return transmitted_ray_perpendicular + transmitted_ray_parallel;
}

// Header guard
#endif
