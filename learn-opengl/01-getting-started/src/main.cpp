#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Window-resize callback
// When the user resizes the window, adjust the OpenGL viewport size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Keep the bottom-left corner at (0, 0)
    glViewport(0, 0, width, height);
}

int main(int argc, char* argv[]) {
    // Wow, I haven't written a C++ main() function in a while...
    // I have forgotten how to do it by-memory
    std::cout << "LearnOpenGL Window" << std::endl;
    
    // Initialize GLFW
    if (glfwInit() == GL_FALSE) {
        std::cerr << "GLFW failed to initialize." << std::endl;
        return 1;
    }
    
    // In general, glfwWindowHint() allows us to configure GLFW
    // All possible configuration options are enums that are prefixed with `GLFW_`
    // We set integer values to the configuration option
    // The function does not return anything. Any invalid values will be validated
    //  when the window is created

    // Set the minimum version of OpenGL that this application supports
    // This affects the type of OpenGL context that is created
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,  4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // Use core-profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window object, which holds the window data 
    int window_width_pixels = 800;
    int window_height_pixels = 600;
    GLFWwindow* window = glfwCreateWindow(
        window_width_pixels,
        window_height_pixels,
        "LearnOpenGL",
        NULL,
        NULL
    );
    if (window == NULL) {
        std::cerr << "Failed to create a window" << std::endl;
        glfwTerminate();
        return 1;
    }
    // Set the window to be the main context
    glfwMakeContextCurrent(window);

    // Initialize GLAD so we obtain the correct function pointers for this OS
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to load GLAD" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Specify window to OpenGL
    // Left x-coordinate of the viewport
    int viewport_x = 0;
    // The bottom y-coordinate of the viewport
    int viewport_y = 0;
    // Create the viewport
    glViewport(0, 0, window_width_pixels, window_height_pixels);

    // Register the window resize callback
    // When the user resizes the window, framebuffer_size_callback() gets called
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Start the render loop
    // This keeps the application running and handles new input
    //  until the application is closed
    while (!glfwWindowShouldClose(window)) {
        // SwapBuffer is a 2D buffer with color values for each pixel in the GLFW window
        // Displays the pixels onto the screen
        glfwSwapBuffers(window);
        
        // Checks for new input (ex. mouse/keyboard input) and updates the state
        // Registered callbacks are executed 
        glfwPollEvents();
    }

    // Application was closed, clean up all GLFW resources
    std::cout << "Terminating application" << std::endl;
    glfwTerminate();

    return 0;
}
