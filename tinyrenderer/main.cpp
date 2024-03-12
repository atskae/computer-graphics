#include "tgaimage.h"
#include "tinyrenderer.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);
	//line_with_step(Point(0, 70), Point(70, 30), image, red);
	line_with_calculated_step(Point(13, 20), Point(80, 40), image, white);
	line_with_calculated_step(Point(20, 13), Point(40, 80), image, red);
	// The reverse of the first line, it *should* render exactly the same...
	line_with_calculated_step(Point(80, 40), Point(13, 20), image, red);
	
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}
