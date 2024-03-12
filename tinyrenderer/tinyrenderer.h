#ifndef TINYRENDERER_H
#define TINYRENDERER_H

#include "tgaimage.h"

struct Point {
    int x = 0.0;
    int y = 0.0;
    int z = 0.0;

    Point(int x, int y): x(x), y(y) {}
};

/* Draws a line segment from p0 to p1 */

// Uses an arbitrary step value between 0 and 1
// Colors a line of pixels spaced by `step`
void line_with_step(Point p0, Point p1, TGAImage& image, TGAColor color);

// Calculates the number of pixels needed to fill the line
void line_with_calculated_step(Point p0, Point p1, TGAImage& image, TGAColor color);

#endif // Header guard
