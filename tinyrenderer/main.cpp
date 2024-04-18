#include <vector>

#include "tgaimage.h"
#include "model.h"
#include "tinyrenderer.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue = TGAColor(0, 0,   255,   255);

int main(int argc, char** argv) {
	int width = 700;
	int height = 1000;
	TGAImage image(width, height, TGAImage::RGB);
	//line(Point(300, 631), Point(326, 307), image, white);
	//line_with_swap(Point(326, 307), Point(300, 631), image, red);
	//line_official(Point(300, 631), Point(326, 307), image, blue);
	
	Model model("obj/african_head.obj");
	std::vector<Face>& faces = model.get_faces();
	std::vector<Vec3>& vertices = model.get_vertices();
	std::cout << faces.size() << " faces found" << std::endl; 
	std::vector<Face> sub_faces;
	sub_faces.push_back(faces[2000]);
	//sub_faces.push_back(faces[2001]);
	for (unsigned int fi=0; fi<faces.size(); fi++) {
		Face& face = faces[fi];
		//std::cout << "Face " << fi << std::endl;
		unsigned int num_vertices = face.vertex_indices.size();
		for (unsigned int i=0; i<num_vertices; i++) {
			VertexIndex vi_0 = face.vertex_indices[i];
			// Module has precedence over +
			VertexIndex vi_1 = face.vertex_indices[(i+1) % num_vertices];
			// Obj format index starts at 1
			Vec3& v0 = vertices[vi_0.vertex_index-1];
			Vec3& v1 = vertices[vi_1.vertex_index-1];

			//std::cout << i << ": Drawing a line from " << v0 << " to " << v1 << std::endl;

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

			Point p0(x0, y0);
			Point p1(x1, y1);


			TGAColor line_color = white;
			std::string color_name = "white";
			if (i%3 == 1) {
				line_color = red;
				color_name = "red";
			}
			else if (i%3 == 2) {
				line_color = green;
				color_name = "green";
			}
			
			//std::cout << "Drawing a normalized line from " << p0 << " to " << p1 << " with color: " << color_name << std::endl;

			//line_with_swap(p0, p1, image, line_color);
			
			line_official(p0, p1, image, blue);

			//std::cout << "---" << std::endl;
		}
		//std::cout << "==========" << std::endl;
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}
