#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "geometry.h"

struct VertexIndex {
    unsigned int vertex_index = 0; // index into the verticies array
    unsigned int texture_coordinate_index = 0; // index into the texture coordinates array
    unsigned int normal_index = 0; // index into the normal vectors array
};

struct Face {
    std::vector<VertexIndex> vertex_indices;
};

class Model {
    public:
        Model(const char* filename);

        // Getters
        std::vector<Vec3>& get_vertices() {
            return this->vertices;
        }

    private:
        std::vector<Vec3> vertices;
        std::vector<Vec3> texture_coordinates;
        std::vector<Vec3> normals;

        std::vector<Face> faces;
};

Model::Model(const char* filename) {
    std::fstream obj_file(filename);
    if (obj_file.is_open()) {
        std::string line;
        while (std::getline(obj_file, line)) {
            std::istringstream iss(line);
            std::vector<std::string> tokens;

            std::string token;
            while (iss >> token) {
                tokens.push_back(token);
            }

            if (tokens.size() > 0) {
                std::string type = tokens[0];
                if (type == "v") {
                    Vec3 vertex;
                    vertex.x = std::stof(tokens[1]);
                    vertex.y = std::stof(tokens[2]);
                    vertex.z = std::stof(tokens[3]);
                    this->vertices.push_back(vertex);
                }
            }
            
        }
        obj_file.close();
    } else {
        std::cout << "Failed to load Obj file: " << filename << std::endl;
    }
}

#endif // Header guard
