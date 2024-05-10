#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#include "geometry.h"

struct VertexIndex {
    unsigned int vertex_index = 0; // index into the verticies array
    unsigned int texture_coordinate_index = 0; // index into the texture coordinates array
    unsigned int normal_index = 0; // index into the normal vectors array
};

std::ostream& operator << (std::ostream& o, const VertexIndex& vi) {
    o << "VertexIndex(vertex_index=" << vi.vertex_index << 
        ", texture_coordinate_index=" << vi.texture_coordinate_index <<
        ", normal_index=" << vi.normal_index << ")";
    return o;
}
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

        std::vector<Vec3>& get_texture_coordinates() {
            return this->texture_coordinates;
        }

        std::vector<Vec3>& get_normals() {
            return this->normals;
        }

        std::vector<Face>& get_faces() {
            return this->faces;
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
                if (type == "v" || type == "vt" || type == "vn") {
                    Vec3 vertex(0.0, 0.0, 0.0);
                    vertex.x = std::stof(tokens[1]);
                    vertex.y = std::stof(tokens[2]);
                    vertex.z = std::stof(tokens[3]);

                    if(type == "v") this->vertices.push_back(vertex);
                    else if (type == "vt") this->texture_coordinates.push_back(vertex);
                    else if (type == "vn") this->normals.push_back(vertex);
                } else if (type == "f") {
                    Face face;
                    for (unsigned int i=1; i<tokens.size(); i++) {
                        std::string token = tokens[i];
                        std::replace(token.begin(), token.end(), '/', ' ');
                        std::string sub_token;
                        std::vector<std::string> sub_tokens;
                        std::stringstream ss(token);
                        while(ss >> sub_token) {
                            sub_tokens.push_back(sub_token);
                        }
                        VertexIndex vi;
                        vi.vertex_index = std::stoi(sub_tokens[0]);
                        vi.texture_coordinate_index = std::stoi(sub_tokens[1]);
                        vi.normal_index = std::stoi(sub_tokens[2]);
                        face.vertex_indices.push_back(vi);
                    }
                    this->faces.push_back(face);
                }
            }
            
        }
        obj_file.close();
    } else {
        std::cout << "Failed to load Obj file: " << filename << std::endl;
    }
}

#endif // Header guard
