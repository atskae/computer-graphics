#include <iostream> // std::cout
#include <cmath> // abs
#include <vector>
#include <algorithm>
#include <limits>

#include "geometry.h"
#include "tinyrenderer.h"

void line_with_step(Point p0, Point p1, TGAImage& image, TGAColor color) {
    float step = 0.01;
    for (float t=0.0; t<1.0; t+=step) {
        int x = p0.x + (p1.x - p0.x)*t;
        int y = p0.y + (p1.y - p0.y)*t;
        image.set(x, y, color);
    }
}

void line_with_calculated_step(Point p0, Point p1, TGAImage& image, TGAColor color) {
    for (int x=p0.x; x<=p1.x; x++) {
        // Linear interpolation!
        // The ratio of x and the length of the total line segment
        float t = (x-p0.x)/(float)(p1.x-p0.x);
        // (1-t) + t = 1.0
        // Here we take a percentage of y0 and a percentage of y1
        // As we move across the line toward y1, the percentage of y1 increases
        // while the percentage of y0 decreases
        int y = p0.y*(1.0 - t) + p1.y*t;
        image.set(x, y, color);
    }
}

void line_with_swap(Point p0, Point p1, TGAImage& image, TGAColor color) {
    Point pp0 = p0;
    Point pp1 = p1;

    // Choose the axis with the larger range so that more points are sampled
    // Then make sure the that the range is increasing
    if (abs(p0.y - p1.y) > abs(p0.x - p1.x)) {
        // The y-axis has a larger range
        // Ensure we are increasing
        if (pp1.y < pp0.y) {
            pp0 = p1;
            pp1 = p0;
        }

        // Interpolate over the y-axis
        for (int y=pp0.y; y<=pp1.y; y++) {
            float t = (y-pp0.y)/(float)(pp1.y-pp0.y);
            int x = pp0.x*(1.0 - t) + pp1.x*t;
            image.set(x, y, color);
        }

    } else {
        // The x-axis has a larger range
        // Ensure we are increasing
        if (p1.x < p0.x) {
            pp0 = p1;
            pp1 = p0;
        }

        // Interpolate over the x-axis
        for (int x=pp0.x; x<=pp1.x; x++) {
            float t = (x-pp0.x)/(float)(pp1.x-pp0.x);
            int y = pp0.y*(1.0 - t) + pp1.y*t;
            image.set(x, y, color);
        }

    }
}

// Avoids executing multiplication by keeping track of the error
// between the ideal line and pixel
// This implementation is buggy and incorrect!!
void line_no_multiply(Point p0, Point p1, TGAImage& image, TGAColor color) {
    // Choose the axis with the larger range so that more points are sampled
    // Then make sure the that the range is increasing
    if (abs(p0.y - p1.y) > abs(p0.x - p1.x)) {
        // The y-axis has a larger range
        // Ensure we are increasing
        if (p1.y < p0.y) {
            std::swap(p0, p1);
        }

        float slope = (float) (p1.x - p0.x) / (p1.y - p0.y);
        
        // The distance between the bottom edge of a pixel to where
        // the ideal line intersects and leaves the pixel's edge
        // The ideal line's y-value is at a multipe of the slope
        float error = 0;
        
        int x = p0.x;
        for (int y=p0.y; y<=p1.y; y++) {
            image.set(x, y, color);
            error += slope;
            // Check if the distance between the point of intersection
            // and the bottom edge of the pixel is greater than
            // the midpoint of the edge of the pixel
            if (error > 0.5) {
                x += 1;
                error -= 1;
            }
        }         

    } else {
        // The x-axis has a larger range
        // Ensure we are increasing
        if (p1.x < p0.x) {
            std::swap(p0, p1);
        }

        float slope = (float) (p1.y - p0.y) / (p1.x - p0.x);
        float error = 0;
        int y = p0.y;
        for (int x=p0.x; x<=p1.x; x++) {
            image.set(x, y, color);
            error += slope;
            if (error > 0.5) {
                y += 1;
                error -= 1;
            }
        }
    }
}

// Avoids floating point and division
void line_no_floating_point(Point p0, Point p1, TGAImage& image, TGAColor color) {
    // Choose the axis with the larger range so that more points are sampled
    // Then make sure the that the range is increasing

    if (abs(p0.y - p1.y) > abs(p0.x - p1.x)) {
        // The y-axis has a larger range
        // Ensure we are increasing
        if (p1.y < p0.y) {
            std::swap(p0, p1);
        }

        int dy = p1.y - p0.y;
        int dx = p1.x - p0.x;
        
        // The x values could be decreasing
        // In this case, the step needs to decrease too
        int xi = 1;
        if (dx < 0) {
            xi = -1;
        }
        
        int dy_2 = 2*dy;
        int dx_2 = 2*abs(dx);
        int error = 0;
        int x = p0.x;
        for (int y=p0.y; y<=p1.y; y++) {
            image.set(x, y, color);
            error += dx_2;
            if (error > dy) {
                x += xi;
                error -= dy_2;
            }
        }         

    } else {
        // The x-axis has a larger range
        // Ensure we are increasing
        if (p1.x < p0.x) {
            std::swap(p0, p1);
        }

        int dy = p1.y - p0.y;
        int dx = p1.x - p0.x;

        // The y-values could be decreasing as we increase x
        // So the update step for y need to decrease
        int yi = 1;
        if (dy < 1) {
            yi = -1;
        }
        
        int dx_2 = 2*dx;
        int dy_2 = 2*abs(dy);
        int error = 0;
        int y = p0.y;
        for (int x=p0.x; x<=p1.x; x++) {
            image.set(x, y, color);
            error += dy_2;
            if (error > dx) {
                y += yi;
                error -= dx_2;
            }
        }
    }
}


void line(Point p0, Point p1, TGAImage &image, TGAColor color) { 
    line_no_floating_point(p0, p1, image, color);
}

void line_official(Point p0, Point p1, TGAImage &image, TGAColor color) { 
    int x0 = p0.x;
    int y0 = p0.y;
    int x1 = p1.x;
    int y1 = p1.y;

    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    } 
} 

void triangle(std::vector<Point> t, TGAImage& image, TGAColor color) {
    line(t[0], t[1], image, color);
    line(t[1], t[2], image, color);
    line(t[2], t[0], image, color);
}

// Takes logic from line_no_floating_point() to draw a triangle
// while drawing lines
// This functions assumes a line function that returns all the points
// that were used to draw the line
//void triangle_first_attempt(std::vector<Point> t, TGAImage& image, TGAColor color) {
//    // Sort the triangle's points by y-coordinate
//    std::sort(
//        t.begin(), t.end(),
//        [](Point p0, Point p1) {return p0.y < p1.y;}
//    );
//    Point corner = t[2];
//    Point p0 = t[0];
//    Point p1 = t[1];
//
//    // Get the points that form the base of the triangle
//    std::vector<Point> base = line(p0, p1, image, color);
//    // Draw a line from the top corner of the triangle
//    // to every point in the base
//    for (auto& p: base) {
//        line(corner, p, image, color);
//    }
//}
//
//void triangle_second_attempt(std::vector<Point> t, TGAImage& image, TGAColor color) {
//    // Find the side that is the longest...
//    std::vector<std::vector<Point>> lines;
//    lines.push_back(line(t[0], t[1], image, color));
//    lines.push_back(line(t[1], t[2], image, color));
//    lines.push_back(line(t[2], t[0], image, color));
//    std::sort(
//        lines.begin(), lines.end(), 
//        [](std::vector<Point> a, std::vector<Point> b) { return a.size() < b.size(); }
//    );
//
//    // Longest side
//    std::vector<Point> base = lines[2];
//    
//    // Draw lines from the base to each side
//    for (int li=0; li<2; li++) {
//        std::vector<Point> line_points = lines[li];
//        for (unsigned int i=0; i<line_points.size(); i++) {
//            Point p0 = base[i];
//            Point p1 = line_points[i];
//            line(p0, p1, image, color);
//        }
//    }
//        
//}

void triangle_filled_straight_lines(std::vector<Point> t, TGAImage& image, TGAColor color) {
    // Sort the triangle's points by y-coordinate
    std::sort(
        t.begin(), t.end(),
        [](Point p0, Point p1) {return p0.y < p1.y;}
    );

    int total_height = t[2].y - t[0].y;
    int lower_segment_height = t[1].y - t[0].y;
    int upper_segment_height = t[2].y - t[1].y;
    for (int y=t[0].y; y<=t[2].y; y++) {
        bool is_lower_segment = (y < t[1].y);
        float alpha = (float)(y - t[0].y) / total_height;
        float beta = 0.0;
        
        if (is_lower_segment) beta = (float)(y - t[0].y) / lower_segment_height;
        else beta = (float)(y - t[1].y) / upper_segment_height;

        // x-coordinate on A
        int ax = t[0].x + (t[2].x  - t[0].x)*alpha;
        // x-coordinate on lower B segment
        int bx = 0;
        if (is_lower_segment) bx = t[0].x + (t[1].x - t[0].x)*beta;
        else bx = t[1].x + (t[2].x - t[1].x)*beta;

        // Draw a straight line    
        if (ax > bx) std::swap(ax, bx); 
        for (int x=ax; x<=bx; x++) {
            image.set(x, y, color); 
        }
    }
}

std::vector<float> compute_barycentric_coordinates(std::vector<Point>& t, Point p) {
    // Rewrite as variables for readability....
    Point a = t[0];
    Point b = t[1];
    Point c = t[2];

    std::vector<int> v0 = {
        // A -> B
        (b-a).x,
        // A -> C  
        (c-a).x,
        // P -> A 
        (a-p).x
    };

    std::vector<int> v1 = {
        // A -> B
        (b-a).y,
        // A -> C  
        (c-a).y,
        // P -> A 
        (a-p).y
    };

    std::vector<int> cross_product = {
        v0[1]*v1[2] - v0[2]*v1[1],
        v0[2]*v1[0] - v0[0]*v1[2],
        v0[0]*v1[1] - v0[1]*v1[0]
    };

    //// Compute the coefficients
    //// We divide by the z-coordinate since we want the cross product vector to be [u, v, 1]
    //std::vector<float> coefficients;
    //coefficients.push_back((float)cross_product[0]/cross_product[2]);
    //coefficients.push_back((float)cross_product[1]/cross_product[2]);
    //coefficients.push_back(1.0 - (float)(cross_product[0] + cross_product[1])/cross_product[2]);

    // Get the cross product in terms of [u, v, 1]
    // To normalize the z-coordinate, we divide each value by the z-coordinate
    std::vector<float> cross_product_float;
    for (int i=0; i<3; i++) {
        cross_product_float.push_back((float) cross_product[i] / cross_product[2]);
    }

    // Now cross_product_float is of form: [u, v, 1];
    // We can use the cross product vector to then compute
    // the Barycentric coordinates: (1 - u - v, u, v)
    std::vector<float> coefficients;
    coefficients.push_back(1.0 - (cross_product_float[0] + cross_product_float[1]));
    coefficients.push_back(cross_product_float[0]);
    coefficients.push_back(cross_product_float[1]);
    
    return coefficients;
}

std::vector<int> cross(std::vector<int>& v0, std::vector<int>& v1) {
    std::vector<int> cross_product = {
        v0[1]*v1[2] - v0[2]*v1[1],
        v0[2]*v1[0] - v0[0]*v1[2],
        v0[0]*v1[1] - v0[1]*v1[0]
    };

    return cross_product;
}

float vector_magnitude(std::vector<int>& v) {
    int s = 0;
    for (int vi: v) {
        s += vi*vi;
    }
    return sqrt((float)s);
}

std::vector<float> compute_barycentric_coordinates_area(std::vector<Point>& t, Point p) {
    std::vector<float> coefficients;
    float total_area = 0.0;
    for (int i=0; i<3; i++) {
        Point& t1 = t[(i+1)%3];
        Point& t2 = t[(i+2)%3];
        std::vector<int> v0 = {
            p.x - t1.x,
            p.y - t1.y,
            p.z - t1.z
        };
        std::vector<int> v1 {
            t2.x - t1.x,
            t2.y - t1.y,
            t2.z - t1.z
        };

        auto cross_product = cross(v0, v1);
        float area = vector_magnitude(cross_product);
        total_area += area;
        coefficients.push_back(area);
    }
    for (int i=0; i<3; i++) {
        coefficients[i] /= total_area;
    }

    return coefficients;
}


void triangle_filled_barycentric_coordinates(
    std::vector<Point> t,
    std::vector<Vec3> t_world,
    TGAImage& image,
    float light_intensity,
    std::vector<std::vector<float>>& zbuffer,
    TGAImage& texture_image, 
    std::vector<Vec3> uv_coordinates
) {
    // Compute the bounding box of this triangle
    
    // Sort the triangle's points by x-coordinate
    std::vector<int> x_coordinates = {
        t[0].x, t[1].x, t[2].x
    };
    std::sort(
        x_coordinates.begin(), x_coordinates.end()
    );
    int lowest_x = x_coordinates[0]; 
    int highest_x = x_coordinates[2];
    
    // Sort the triangle's points by y-coordinate
    std::vector<int> y_coordinates = {
        t[0].y, t[1].y, t[2].y
    };
    std::sort(
        y_coordinates.begin(), y_coordinates.end()
    ); 
    int lowest_y = y_coordinates[0]; 
    int highest_y = y_coordinates[2];

    // Clip against the canvas
    // The bottom-left corner of the canvas is (0,0)
    // The top-right corner is (width, height)
    
    if (0 > lowest_x) lowest_x = 0;
    if (0 > lowest_y) lowest_y = 0;

    int image_width = image.get_width();
    int image_height = image.get_height();

    if (image_width < highest_x) highest_x = image_width;
    if (image_height < highest_y) highest_y = image_height;

    // std::cout << "Bounding box: (" << lowest_x << "," << lowest_y << "); (" << highest_x << "," << highest_y << ")" << std::endl;

    // Iterate through all the pixels in the bounding box
    for (int x=lowest_x; x<=highest_x; x++) {
        for (int y=lowest_y; y<=highest_y; y++) {
            // Compute the barycentric coordinates of this point
            std::vector<float> barycentric_coordinates = compute_barycentric_coordinates(t, Point(x, y));
            bool is_inside_triangle = true;
            for (float c: barycentric_coordinates) {
                if (c < 0.0) {
                    is_inside_triangle = false;
                    break;
                }
            }
            if (is_inside_triangle) {
                float z = 0.0;
                // The texture coordinate (u,v) of this *pixel*
                float u = 0.0;
                float v = 0.0;

                std::vector<int> indices = {0, 1, 2};
                for (int i=0; i<3; i++) { // for each vertex
                    z += (t_world[i].z*barycentric_coordinates[indices[i]]);
                    u += (uv_coordinates[i].x * barycentric_coordinates[indices[i]]);
                    v += (uv_coordinates[i].y * barycentric_coordinates[indices[i]]);
                }
                
                // Only color the pixel if its depth is closer to the camera (positive z)
                if (z > zbuffer[x][y]) {
                    zbuffer[x][y] = z; 

                    int texture_image_x = u * texture_image.get_width();
                    int texture_image_y = v * texture_image.get_height(); 
                    TGAColor base_color = texture_image.get(texture_image_x, texture_image_y);
                    bool with_light = true;
                    TGAColor color(
                        base_color.r, // * light_intensity,
                        base_color.g, // * light_intensity,
                        base_color.b, // * light_intensity,
                        255 // opacity
                    );
                    // TGAColor color(
                    //     255 * barycentric_coordinates[indices[0]],
                    //     255 * barycentric_coordinates[indices[1]],
                    //     255 * barycentric_coordinates[indices[2]],
                    //     255 // opacity
                    // );

                    if (with_light) {
                        color = TGAColor(
                            base_color.r * light_intensity,
                            base_color.g * light_intensity,
                            base_color.b * light_intensity,
                            255 // opacity
                        );
                    }
                    image.set(x, y, color);
                } 
                //image.set(180, 100, TGAColor(255,0,0,255));
            } 
        }
    } 

}

void triangle_filled(
    std::vector<Point> t,
    std::vector<Vec3> t_world,
    TGAImage& image,
    float light_intensity,
    std::vector<std::vector<float>>& zbuffer,
    TGAImage& texture_image,
    std::vector<Vec3> uv_coordinates) {
    triangle_filled_barycentric_coordinates(t, t_world, image,
        light_intensity, zbuffer, texture_image, uv_coordinates
    );
}

void rasterize(Point p0, Point p1, TGAImage& image, TGAColor color, std::vector<int>& ybuffer) {
    // Basic linear interpolation (used in line_with_calculated_step())
    
    // Ensure x is increasing
    if (p1.x < p0.x) {
        std::swap(p0, p1);
    }
    
    for (int x=p0.x; x<=p1.x; x++) {
        // Linear interpolation!
        // The ratio of x and the length of the total line segment
        float t = (x-p0.x)/(float)(p1.x-p0.x);
        // (1-t) + t = 1.0
        // Here we take a percentage of y0 and a percentage of y1
        // As we move across the line toward y1, the percentage of y1 increases
        // while the percentage of y0 decreases
        int y = p0.y*(1.0 - t) + p1.y*t;
        
        // If this y value is larger than previously seen, we color the pixel;
        if (x < ybuffer.size() && y > ybuffer[x]) {
            image.set(x, 0, color);
            ybuffer[x] = y;
        }
    }
}
