#include <vector>

#include "tgaimage.h"
#include "tinyrenderer.h"
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);

	int num_iter = 1;
	for (int i=0; i<num_iter; i++) {	
	 	line(Point(13, 20), Point(80, 40), image, white);
	    line(Point(20, 13), Point(40, 80), image, red);
	    line(Point(80, 40), Point(13, 20), image, red);
	}
	Model model("obj/african_head.obj");
	std::vector<Face>& faces = model.get_faces();
	std::cout << faces.size() << " faces found" << std::endl; 
	for (auto& f: faces) {
		std::cout << "Face" << std::endl;
		for (auto vi: f.vertex_indices) {
			std::cout << vi << std::endl;
		}
		std::cout << std::endl;
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}
