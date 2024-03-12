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
