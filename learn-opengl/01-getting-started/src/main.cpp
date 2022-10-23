#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(int argc, char* argv[]) {
    // Wow, I haven't written a C++ main() function in a while...
    // I have forgotten how to do it by-memory
    std::cout << "I hope glfw and glad compiles!" << std::endl;
    
    // Initialize GLFW
    if (glfwInit() == GL_FALSE) {
        std::cerr << "GLFW failed to initialize." << std::endl;
        return 1;
    }

    return 0;
}
