#include <iostream>
#include <string>

// OpenGL Mathematics library (GLM)
#include <glm/glm.hpp> // data structures (ex. vec4, mat4)
#include <glm/gtc/matrix_transform.hpp> // glm::translate()


void print_vector(std::string info, glm::vec4 vec) {
    std::cout << info << ": " << "(" << vec.x << "," << vec.y << "," << vec.z << ")" << std::endl;
}


int main(int argc, char* argv[]) {
    std::cout << "OpenGL Mathematics library (GLM)" << std::endl;
    
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
    
    print_vector("Original vector", vec);
    print_vector("Transformed vector", result);

    return 0;
}