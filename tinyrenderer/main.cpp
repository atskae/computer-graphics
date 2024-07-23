#include <vector>
#include <cstdlib> // rand()
#include <limits>

#include "tgaimage.h"
#include "model.h"
#include "tinyrenderer.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue = TGAColor(0, 0,   255,   255);

// Position of light source, in normalized world coordinates
Vec3 lightPos(0, 0, -0.5);

void draw_face() {
	int width = 600;
	int height = 700;
	TGAImage image(width, height, TGAImage::RGB);

	Model model("assets/african_head.obj");
	std::vector<Face>& faces = model.get_faces();
	std::vector<Vec3>& vertices = model.get_vertices();
	std::vector<Vec3>& texture_coordinates = model.get_texture_coordinates();
	std::cout << faces.size() << " faces found" << std::endl; 

	// Compute the color using texture coordinates
	const char* texture_file_name = "assets/african_head_diffuse.tga";
	TGAImage texture_image;
	if(!texture_image.read_tga_file(texture_file_name)) {
		std::cout << "Failed to read texture file: " << texture_file_name << std::endl;
	}	
	texture_image.flip_vertically();
	std::cout << "Read in texture image of size (" << 
		texture_image.get_width() << ","
		<< texture_image.get_height() << ")" << std::endl;
	
	// Holds the largest z-coordinate seen
    // If we encounter a z-coordinate that is lower, we do not have to draw it
	std::vector<std::vector<float>> zbuffer(
        width,
        std::vector<float>(height, std::numeric_limits<float>::lowest())
    );
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
		std::vector<Vec3> worldCoordinates;
		
		// Colors obtained from the texture image
		// Each color is associated to its respective vertex in the triangle
		std::vector<TGAColor> colors;
		
		//// The center of the triangle
		//Vec3 centroid(0.0, 0.0, 0.0);	
		for (int vi=0; vi<3; vi++) {
			VertexIndex vertex_index = face.vertex_indices[vi];
			// Obj format index starts at 1
			Vec3 v = vertices[vertex_index.vertex_index-1];
			worldCoordinates.push_back(v);	

			// Obj format index starts at 1
			Vec3 texture_coordinate = texture_coordinates[vertex_index.texture_coordinate_index-1];
			int image_x = floor(texture_coordinate.x * texture_image.get_width());
			int image_y = floor(texture_coordinate.y * texture_image.get_height());
			//std::cout << "image coordinates (" << image_x << "," << image_y << ")" << std::endl;
			colors.push_back(texture_image.get(image_x, image_y));
			
			// UV mapping for debugging
			//colors.push_back(TGAColor(texture_coordinate.x*255, texture_coordinate.y*255, 0, 255));

			//if (vi == 0) colors.push_back(TGAColor(255, 0, 0, 255));
			//else if (vi == 1) colors.push_back(TGAColor(0, 255, 0, 255));
			//else if (vi == 2) colors.push_back(TGAColor(0, 0, 255, 255));
			//else colors.push_back(TGAColor(0, 0, 0, 255));
			
			//// Accumulate the coordinate, then take the average later
			//// This computes the center coordinate of the triangle
			//centroid.x += v.x;
			//centroid.y += v.y;

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
		
		//// Compute the center
		//centroid.x /= 3;
		//centroid.y /= 3;
		
		//Vec3 lightVector = lightPos - centroid;
		//lightVector.normalize();
		
		//// Arbitrarily choose the first vertex's normal vector...
		//// We subtract 1 because Wave Obj indexing starts at 1	
		//Vec3 normal = normals[face.vertex_indices[1].normal_index-1];
		//normal.normalize();

		// Manually compute the normal vector
		// Compute the cross product of any two edges of the triangle
		Vec3 normal = (worldCoordinates[0] - worldCoordinates[1]) ^ (worldCoordinates[2] - worldCoordinates[1]);
		normal.normalize();

		// Compute the dot product
		//float light_intensity = lightVector*normal;
		Vec3 LightDir(0.0, 0.0, -1.0);
		float light_intensity = LightDir*normal;
		//float light_intensity = Vec3(-1,0,0)*normal;
		// The dot product is negative if the normal vector is facing opposite
		// of the light vector
		TGAColor color = white;
		if (light_intensity > 0) {
			TGAColor face_color(
				color.r*light_intensity,
				color.g*light_intensity,
				color.b*light_intensity,
				255
			);
			triangle_filled(t, worldCoordinates, image, colors, light_intensity, zbuffer);
		}	
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
	std::vector<Vec3> t_world; // unused - just to fit the API
	for (int i=0; i<3; i++) {
		t_world.push_back(Vec3(0,0,0));
	}
	std::vector<std::vector<float>> zbuffer; // unused
	//triangle_filled(t0, t_world, image, blue, 1.0, zbuffer);

	std::vector<Point> t1 = {
		Point(180, 50),
		Point(150, 1),
		Point(70, 180)
	};
	//triangle_filled(t1, t_world, image, white, zbuffer);

	std::vector<Point> t2 = {
		Point(130, 180),
		Point(180, 150),
		Point(120, 160),
	};
	//triangle_filled(t2, t_world, image, green, zbuffer);

	image.flip_vertically();
	image.write_tga_file("output.tga");

}

void draw_using_rasterize() {
	int width = 800;	
	TGAImage render(width, 1, TGAImage::RGB);
	std::vector<int> ybuffer(width, std::numeric_limits<int>::min());

	rasterize(Point(20, 34),   Point(744, 400), render, red,   ybuffer);
    rasterize(Point(120, 434), Point(444, 400), render, green, ybuffer);
    rasterize(Point(330, 463), Point(594, 200), render, blue,  ybuffer);

	render.flip_vertically();
	render.write_tga_file("output.tga");
}

int main(int argc, char** argv) {
	draw_face();	
	//draw_triangle();	
	//draw_using_rasterize();	
	return 0;
}
