#include <vector>

#include "tgaimage.h"
#include "model.h"
#include "tinyrenderer.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main(int argc, char** argv) {
	int width = 500;
	int height = 700;
	TGAImage image(width, height, TGAImage::RGB);
	
	Model model("obj/african_head.obj");
	std::vector<Face>& faces = model.get_faces();
	std::vector<Vec3>& vertices = model.get_vertices();
	std::cout << faces.size() << " faces found" << std::endl; 
	for (auto& face: faces) {
		unsigned int num_vertices = face.vertex_indices.size();
		for (unsigned int i=0; i<num_vertices; i++) {
			VertexIndex vi_0 = face.vertex_indices[i];
			// Module has precedence over +
			VertexIndex vi_1 = face.vertex_indices[(i+1) % num_vertices];
			Vec3& v0 = vertices[vi_0.vertex_index];
			Vec3& v1 = vertices[vi_1.vertex_index];

			// Normalize the vectors so that they map into a canvas
			// of size width x height
			// The coordinates in the Obj file have a range of [-1, 1]
			// We add +1 to the coordinate so that the range is positive
			// which becomes: [0, 2]
			// We then divide by 2 so that the range becomes: [0, 1]
			// which is now normalized
			// Then we multiply by width for x (height for y)
			// so that the coordinate is a value between [0, width]
			// ([0, height] for y)
			int x0 = (v0.x + 1)/2 * width;
			int y0 = (v0.y + 1)/2 * height;
			int x1 = (v1.x + 1)/2 * width;
			int y1 = (v1.y + 1)/2 * height;

			line(Point(x0, y0), Point(x1, y1), image, white);
		}
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}
