#include <iostream> // std::cout
#include <cmath> // abs
#include <vector>
#include <algorithm>

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

    // We first iterate across the y-axis of the lower segment's height
    int total_height = t[2].y - t[0].y;
    int segment_height = t[1].y - t[0].y;
    for (int y=t[0].y; y<t[1].y; y++) {
        float alpha = (float)(y - t[0].y) / total_height;
        float beta = (float)(y - t[0].y) / segment_height;

        // x-coordinate on A
        int ax = t[0].x + (t[2].x  - t[0].x)*alpha;
        // x-coordinate on lower B segment
        int bx = t[0].x + (t[1].x - t[0].x)*beta;

        // Draw a straight line    
        line(Point(ax, y), Point(bx, y), image, color);
    }  
}

void triangle_filled(std::vector<Point> t, TGAImage& image, TGAColor color) {
    triangle_filled_straight_lines(t, image, color);
}
