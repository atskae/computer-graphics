#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"


class camera {
    private:
        // Width to height of the image
        double aspect_ratio;
        
        point3 origin;
        point3 lower_left_corner;
        
        // Vectors representing the viewport
        vec3 horizontal;
        vec3 vertical;

        // Orthonormal bases that define the camera orientation
        vec3 u; // points horizontal ("x-axis")
        vec3 v; // points up ("y-axis")
        vec3 w; // `lookat` to `lookfrom` ("z-axis")
        
        double lens_radius;

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
            double aspect_ratio,
            double aperature, // size of the camera hole, controls defocus blur
            double focus_dist // the distance between the projection point and the focus plane
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
            this->w = unit_vector(lookfrom - lookat);
            // The cross product gives us a vector that is orthogonal to both
            //  vup and w
            this->u = unit_vector(cross(vup, w)); 
            this->v = cross(w, u); // already a unit vector

            this->origin = lookfrom;
            // TODO: We still want to keep the viewport proportional to the focus_dist (?)
            // The focus plane also acts as the viewport/image plane (?)
            this->horizontal = focus_dist * viewport_width * u;
            this->vertical = focus_dist * viewport_height * v;

            // Negative z-axis goes through the center of the viewport. 
            // To get the lower left corner of the viewport,
            //  we need to go "leftward" (negative) by viewport_width/2
            //  and go "downward" (negative) by viewport_height/2.
            //  The focal length defines how far the origin is from the viewport. 
            // Vector w is on the same line (span?) as `lookfrom` and `lookat`
            //  so from w we can get the focal length
            // +w is behind the camera, -w is in front of the camera
            this->lower_left_corner = this->origin - this->horizontal/2 - this->vertical/2 - focus_dist*w;

            // Divide by 2 since aperature = diameter
            this->lens_radius = aperature / 2;
        }

        // Create a ray that shoots out from the origin to the position on the viewport (?)
        // (s,t) = position of the pixel in the viewport
        //  where `s` is horizontal, `t` is vertical
        // To create the focus blur effect, we randomize where the ray origin is,
        //  by choosing a point within the area of the lens.
        // To have no focus blur effect (the original camera), the lens radius can simply be set to zero.
        ray get_ray(double s, double t) const {
            // Create a random vector with length of the len's radius
            vec3 rd = this->lens_radius * random_in_unit_disk();
            // `u` is the "x-axis" of the camera
            // `v` is the "y-axis" of the camera
            vec3 offset = this->u * rd.x() + this->v * rd.y();

            // Create a vector from (origin+offset) to the viewport/focus plane
            // Here we are creating a vector using two points
            vec3 direction = (this->lower_left_corner + s*this->horizontal + t*this->vertical) - (this->origin + offset);
            return ray(this->origin + offset, direction);
        }
};

#endif // header guard
