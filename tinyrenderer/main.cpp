#include "tgaimage.h"
#include "tinyrenderer.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	line_with_step(Point(0, 70), Point(70, 30), image, red);
	image.write_tga_file("output.tga");
	return 0;
}
