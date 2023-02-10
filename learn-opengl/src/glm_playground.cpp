#include <iostream>
#include <string>
#include <iomanip> // setw()

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

    return 0;
}
