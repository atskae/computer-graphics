#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"


class camera {
    private:
        point3 origin;
        point3 lower_left_corner;
        // Axes
        vec3 horizontal;
        vec3 vertical;
    
    public:
        // Constructor
        camera() {
            // Width to height of the image
            const double aspect_ratio = 16.0 / 9.0;
            
            // Camera
            double viewport_height = 2.0;
            double viewport_width = viewport_height * aspect_ratio;
            // Distance from the eye to the viewport (depth)
            double focal_length = 1.0;
        
            this->origin = point3(0, 0, 0);
            this->horizontal = vec3(viewport_width, 0, 0);
            this->vertical = vec3(0, viewport_height, 0);
            
            // Negative z-axis goes through the center of the viewport. 
            // To get the lower left corner of the viewport,
            //  we need to go "leftward" (negative) by viewport_width/2
            //  and go "downward" (negative) by viewport_height/2.
            //  The focal length defines how far the origin is from the viewport.
            this->lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
        }

        // Create a ray that shoots out from the origin to the position on the viewport (?)
        // (u,v) = position of the pixel in the viewport
        //  where `u` is horizontal, `v` is vertical
        ray get_ray(double u, double v) const {
            vec3 direction = this->lower_left_corner + (u * this->horizontal) + (v * this->vertical) - this->origin;
            return ray(this->origin, direction);
        }
};

#endif // header guard
