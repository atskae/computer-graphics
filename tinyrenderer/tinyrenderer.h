#ifndef TINYRENDERER_H
#define TINYRENDERER_H

#include <iostream>
#include <vector>

#include "tgaimage.h"

#include "geometry.h"

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

void triangle_filled(std::vector<Point> t, std::vector<Vec3> t_world, TGAImage& image, TGAColor color);

// Draws a line given a ybuffer, containing the largest y-values that were already seen
// The y-buffer is used to know which pixels are above other pixels in terms of of height,
// so we know which pixels to render
void rasterize(Point p0, Point p1, TGAImage& image, TGAColor color, std::vector<int>& ybuffer);

#endif // Header guard
