#ifndef TINYRENDERER_H
#define TINYRENDERER_H

#include <iostream>

#include "tgaimage.h"

struct Point {
    int x = 0;
    int y = 0;
    int z = 0;

    Point(int x, int y): x(x), y(y) {}
};

inline std::ostream& operator << (std::ostream& o, const Point& p) {
    o << "Point(" << p.x << ", " << p.y << ", " << p.z << ")";
    return o;
}
    
/* Draws a line segment from p0 to p1 */

// Uses an arbitrary step value between 0 and 1
// Colors a line of pixels spaced by `step`
void line_with_step(Point p0, Point p1, TGAImage& image, TGAColor color);

// Calculates the number of pixels needed to fill the line
void line_with_calculated_step(Point p0, Point p1, TGAImage& image, TGAColor color);

// Swaps the coordinates so that drawing a line from p0 to p1
// contains points that increase going from p0 to p1
void line_with_swap(Point p0, Point p1, TGAImage& image, TGAColor color);

// Same as line_with_swap() but optimized for performance by removing multiplies
void line_no_multiply(Point p0, Point p1, TGAImage& image, TGAColor color);

// Same as line_With_swap_optmized() but removes floating point and division
// Also passes by reference
// Multiplies used are only by 2, so they are efficient (only require a left-shift)
void line_no_floating_point(Point p0, Point p1, TGAImage& image, TGAColor color);

// Current implementation used 
void line(Point p0, Point p1, TGAImage &image, TGAColor color);

// tinyrenderer's implementation
void line_official(Point p0, Point p1, TGAImage &image, TGAColor color);

#endif // Header guard
