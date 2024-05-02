#ifndef TINYRENDERER_H
#define TINYRENDERER_H

#include <iostream>
#include <vector>

#include "tgaimage.h"

struct Point {
    int x = 0;
    int y = 0;
    int z = 0;

    Point(int x, int y): x(x), y(y) {}
    Point operator+(const Point p);
    Point operator-(const Point p);
};

inline std::ostream& operator << (std::ostream& o, const Point& p) {
    o << "Point(" << p.x << ", " << p.y << ", " << p.z << ")";
    return o;
}

inline Point Point::operator+(const Point p) {
    return Point(this->x + p.x, this->y + p.y);
}

inline Point Point::operator-(const Point p) {
    return Point(this->x - p.x, this->y - p.y);
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

// Draws a triangle
void triangle(std::vector<Point> t, TGAImage& image, TGAColor color);

//// Draws lines from a corner to all the points in the base of the triangle
//void triangle_first_attempt(std::vector<Point> t, TGAImage& image, TGAColor color);
//
//// Draws lines the longest side to the two other sides
//void triangle_second_attempt(std::vector<Point> t, TGAImage& image, TGAColor color);

// Fills a triangle by drawing horizontal straight lines
void triangle_filled_straight_lines(std::vector<Point> t, TGAImage& image, TGAColor color);

// Computes the barycentric coordinates of the triangle to determine whether to
// color in the pixel
void triangle_filled_barycentric_coordinates(std::vector<Point> t, TGAImage& image, TGAColor color);

void triangle_filled(std::vector<Point> t, TGAImage& image, TGAColor color);

#endif // Header guard
