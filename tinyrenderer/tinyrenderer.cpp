#include "tinyrenderer.h"


void line_with_step(Point p0, Point p1, TGAImage& image, TGAColor color) {
    float step = 0.01;
    for (float t=0.0; t<1.0; t+=step) {
        int x = p0.x + (p1.x - p0.x)*t;
        int y = p0.y + (p1.y - p0.y)*t;
        image.set(x, y, color);
    }
}
