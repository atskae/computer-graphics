#include <iostream> // std::cout
#include <cmath> // abs

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
        if (pp1.y < pp0.x) {
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
        int dy_2 = 2*dy;
        int dx_2 = 2*(p1.x - p0.x);
        int error = 0;
        int x = p0.x;
        for (int y=p0.y; y<=p1.y; y++) {
            image.set(x, y, color);
            error += dx_2;
            if (error > dy) {
                x += 1;
                error -= dy_2;
            }
        }         

    } else {
        // The x-axis has a larger range
        // Ensure we are increasing
        if (p1.x < p0.x) {
            std::swap(p0, p1);
        }

        int dx = p1.x - p0.x;
        int dx_2 = 2*dx;
        int dy_2 = 2*(p1.y - p0.y);
        int error = 0;
        int y = p0.y;
        for (int x=p0.x; x<=p1.x; x++) {
            image.set(x, y, color);
            error += dy_2;
            if (error > dx) {
                y += 1;
                error -= dx_2;
            }
        }
    }
}


void line(Point p0, Point p1, TGAImage &image, TGAColor color) { 
    line_no_floating_point(p0, p1, image, color);
}
