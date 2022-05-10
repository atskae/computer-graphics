#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"


class camera {
    private:
        // Width to height of the image
        double aspect_ratio;
        
        point3 origin;
        point3 lower_left_corner;
        // Axes
        vec3 horizontal;
        vec3 vertical;
    
    public:
        // Constructor
        camera(
            // Where the camera sits
            point3 lookfrom,
            // What the camera looks at if we drew a straight line
            //  from `lookfrom` to `lookat`
            point3 lookat,
            // "View up vector", which lies on the plane that is orthogonal
            //  to the view direction (`lookfrom` to `lookat`)
            vec3 vup,
            double vfov, // vertical field of view, in degrees
            double aspect_ratio
        ): aspect_ratio(aspect_ratio) {

            double theta = degrees_to_radians(vfov);
            // The height from the triangle starting from the x-z plane with angle theta/2
            // h is a ratio of the distance that the ray is shot from the origin toward the chosen z-plane
            double h = tan(theta/2);

            // Image
            double viewport_height = 2.0 * h;
            double viewport_width = viewport_height * aspect_ratio;
            
            // Compute the orthonormal basis vectors (u, v, w)
            //  that describe the camera's orientation
            
            // A unit vector that points in the direction `lookat` to `lookfrom`
            vec3 w = unit_vector(lookfrom - lookat);
            // The cross product gives us a vector that is orthogonal to both
            //  vup and w
            vec3 u = unit_vector(cross(vup, w)); 
            vec3 v = cross(w, u); // already a unit vector

            this->origin = lookfrom;
            this->horizontal = viewport_width * u;
            this->vertical = viewport_height * v;

            // Negative z-axis goes through the center of the viewport. 
            // To get the lower left corner of the viewport,
            //  we need to go "leftward" (negative) by viewport_width/2
            //  and go "downward" (negative) by viewport_height/2.
            //  The focal length defines how far the origin is from the viewport. 
            // Vector w is on the same line (span?) as `lookfrom` and `lookat`
            //  so from w we can get the focal length
            this->lower_left_corner = this->origin - this->horizontal/2 - this->vertical/2 - w;
        }

        // Create a ray that shoots out from the origin to the position on the viewport (?)
        // (s,t) = position of the pixel in the viewport
        //  where `s` is horizontal, `t` is vertical
        ray get_ray(double s, double t) const {
            vec3 direction = this->lower_left_corner + (s * this->horizontal) + (t * this->vertical) - this->origin;
            return ray(this->origin, direction);
        }
};

#endif // header guard
