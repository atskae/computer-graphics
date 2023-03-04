#include <iostream>
#include <string>
#include <iomanip> // setw()
#include <vector>

// OpenGL Mathematics library (GLM)
#include <glm/glm.hpp> // data structures (ex. vec4, mat4)
#include <glm/gtc/matrix_transform.hpp> // glm::translate()

void print_vec4(std::string info, glm::vec4 vec) {
    std::cout << info << ": " << "(" << vec.x << "," << vec.y << "," << vec.z << "," << vec.w << ")" << std::endl;
}

void print_vec3(std::string info, glm::vec3 vec) {
    std::cout << info << ": " << "(" << vec.x << "," << vec.y << "," << vec.z << ")" << std::endl;
}

// Print in column-major order
void print_matrix(std::string info, glm::mat4 matrix) {
    std::cout << info << std::endl;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            std::cout << std::setw(2) << matrix[j][i] << " ";
        }
        std::cout << std::endl;
    }   
}

int main(int argc, char* argv[]) {
    std::cout << "OpenGL Mathematics library (GLM) version: " << GLM_VERSION << std::endl;
    
    // A vector (1,0,0) with homogenous coordinate (4th dimension = 1)
    // We defined the 3D coordinate as a 4D vector so that we can apply translation
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    
    /* Construct the transformation matrix */
    
    // Create an identity matrix (diagonals all 1s, the rest 0s)
    // If we don't specifiy 1.0f, the matrix will be a null matrix (all zeros)
    //  and all operations using this matrix will result vectors/matrices that are all zeros
    glm::mat4 transformation(1.0f);
    // Takes in a matrix and a translation vector that defines how much translation occurs
    transformation = glm::translate(transformation, glm::vec3(1.0f, 1.0f, 0.0f));

    // Apply the translation operation to our vector!
    glm::vec4 result = transformation * vec;
    
    print_vec4("Original vector", vec);
    print_vec4("Transformed vector", result);

    // Positive z-axis is outside of the screen
    glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(
        cameraPosition - cameraTarget
    );
    print_vec3("Camera direction", cameraDirection);

    // Positive y-axis
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);

    glm::mat4 viewMatrix = glm::lookAt(
        cameraPosition,
        cameraTarget,
        upVector
    );

    print_matrix("viewMatrix (lookAt)", viewMatrix);

    // Pass in elements in column-major order
    glm::mat4 m1(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );

    print_matrix("m1", m1);

    // Pass in by column
    glm::mat4 m2(
        1, 0, 0, 0, // first column
        0, 1, 0, 0, // second column
        0, 0, 1, 0, // third column
        0, 0, -3, 1 // fourth column
    );

    print_matrix("m2", m2);

    glm::mat4 m3 = m1 * m2;

    print_matrix("m3", m3);

    std::cout << "m3 == viewMatrix: " << (m3 == viewMatrix) << std::endl;

    // 3D cube
    // 36 vertices in total
    // 6 faces, 2 triangles per face, 3 vertices per triangle
    std::vector<float> triangle_vertices = {
        // Back face of cube
        // Triangle
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         // Triangle
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        // Front face of cube
        // Triangle
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        // Triangle 
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        // Left face of cube (facing the negative x-axis)
        // Triangle
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        // Triangle
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        // Right-face of cube (facing the positive x-axis)
        // Triangle
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        // Triangle 
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        // Bottom face of cube
        // Triangle
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        // Triangle
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // Top face of cube
        // Triangle
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        // Triangle 
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    }; 


    // For each triangle
    std::cout << "Number of verticies: " << triangle_vertices.size() << std::endl;
    int elements_per_triangle = 15;
    int elements_per_row = 5;
    int num_triangles_seen = 0;
    for (int triangle=0; triangle<triangle_vertices.size(); triangle=triangle+elements_per_triangle) {

        // Compute the starting indices of each point
        // start, middle, end
        int start_indices[3];
        for (int s=0; s<3; s++) {
            int start_index = triangle + (s*elements_per_row);
            start_indices[s] = start_index;
            for (int e=0; e<3; e++) {
                std::cout << triangle_vertices[start_index + e] << " ";
            }
            std::cout << std::endl;
        }
        
        // Create two vectors:
        //  v0: start -> middle
        //  v1: middle -> end
        glm::vec3 triangle_vectors[2] = {};
        for (int v=0; v<2; v++) {
            int start_index = start_indices[v];
            int end_index = start_indices[v+1];
            glm::vec3 point0(
                triangle_vertices[start_index + 0],
                triangle_vertices[start_index + 1],
                triangle_vertices[start_index + 2]
            );
            glm::vec3 point1(
                triangle_vertices[end_index + 0],
                triangle_vertices[end_index + 1],
                triangle_vertices[end_index + 2]
            );
            triangle_vectors[v] = point1 - point0;
        }

        glm::vec3 normal_vector = glm::normalize(
            glm::cross(triangle_vectors[0], triangle_vectors[1])
        );
        
        num_triangles_seen++;
        std::cout << "Triangle " << num_triangles_seen << std::endl;
        print_vec3("Normal vector: ", normal_vector);
        std::cout << "---" << std::endl;

    } // for each triangle ; end

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    print_matrix("model", model);

    glm::vec3 aPos(-0.5f, -0.5f, 0.5f);
    glm::vec3 aNormal(0.0f, 0.0f, 1.0f);
    aNormal = glm::normalize(aNormal);
    glm::vec3 lightPos(0.5f, 0.5f, 2.0f);

    glm::vec3 fragPos = glm::vec3(model * glm::vec4(aPos, 1.0f));
    glm::vec3 lightDir = glm::normalize(lightPos - fragPos);
    
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    print_vec3("aPos", aPos);
    print_vec3("aNormal", aNormal);
    print_vec3("lightPos", lightPos);
    print_vec3("lightDir", lightDir);

    float diffuseStrength = glm::dot(lightDir, aNormal);
    std::cout << "diffuseStrength: " << diffuseStrength << std::endl;

    glm::vec3 diffuse = diffuseStrength * lightColor;
    print_vec3("diffuse", diffuse); 

    return 0;
}
