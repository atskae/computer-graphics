#include <vector>
#include <cstdlib> // rand()

#include "tgaimage.h"
#include "model.h"
#include "tinyrenderer.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue = TGAColor(0, 0,   255,   255);

// Position of light source, in normalized world coordinates
Vec3 lightPos{0.5, -0.5, 0.75};

void draw_face() {
	int width = 600;
	int height = 700;
	TGAImage image(width, height, TGAImage::RGB);

	Model model("obj/african_head.obj");
	std::vector<Face>& faces = model.get_faces();
	std::vector<Vec3>& vertices = model.get_vertices();
	std::vector<Vec3>& normals = model.get_normals();
	std::cout << faces.size() << " faces found" << std::endl; 
	for (unsigned int fi=0; fi<faces.size(); fi++) {
		
		Face& face = faces[fi];
		//std::cout << "Face " << fi << std::endl;
		unsigned int num_vertices = face.vertex_indices.size();
		if (num_vertices != 3) {
			std::cout << "Cannot handle faces that are not triangles" << std::endl;
			continue;
		}

		// Triangle coordinates as screen coordinates
		std::vector<Point> t;
		// The center of the triangle
		Vec3 centroid(0.0, 0.0, 0.0);	
		for (int vi=0; vi<3; vi++) {
			VertexIndex vertex_index = face.vertex_indices[vi];
			// Obj format index starts at 1
			Vec3& v = vertices[vertex_index.vertex_index-1];

			// Accumulate the coordinate, then take the average later
			// This computes the center coordinate of the triangle
			centroid.x += v.x;
			centroid.y += v.y;

			// Convert world coordinates to screen coordinates
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
			int x = (v.x + 1)/2 * width;
			int y = (v.y + 1)/2 * height;
			t.push_back(Point(x, y));
		}
		
		// Compute the center
		centroid.x /= 3;
		centroid.y /= 3;
		
		Vec3 lightVector = lightPos - centroid;
		// Arbitrarily choose the first vertex's normal vector...
		// We subtract 1 because Wave Obj indexing starts at 1	
		Vec3 normal = normals[face.vertex_indices[1].normal_index-1];

		// Compute the dot product
		float light_intensity = lightVector.x*normal.x + lightVector.y*normal.y + lightVector.z*normal.z;

		TGAColor face_color(
			255*light_intensity,
			255*light_intensity,
			255*light_intensity,
			255
		);
		triangle_filled(t, image, face_color);
	}


	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
}

void draw_triangle() {
	int width = 200;
	int height = 200;
	TGAImage image(width, height, TGAImage::RGB);

	std::vector<Point> t0 = {
		Point(10, 70),
		Point(50, 160),
		Point(70, 80)
	};
	triangle_filled(t0, image, blue);

	std::vector<Point> t1 = {
		Point(180, 50),
		Point(150, 1),
		Point(70, 180)
	};
	triangle_filled(t1, image, white);

	std::vector<Point> t2 = {
		Point(130, 180),
		Point(180, 150),
		Point(120, 160),
	};
	triangle_filled(t2, image, green);

	image.flip_vertically();
	image.write_tga_file("output.tga");

}

int main(int argc, char** argv) {
	draw_face();	
	//draw_triangle();	
	return 0;
}
