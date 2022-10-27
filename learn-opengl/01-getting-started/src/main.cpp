#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Source code for the vertex shader
// Written in OpenGL shader language (GLSL)
const char* vertexShaderSource = "#version 460 core\n" // OpenGL version 4.6 with core-profile functionality
    // The `in` keyword specifies all the inputs (input vertex attributes) to this vertex shader
    // The input to this shader program is a 3D vector `vec3`
    // We name the input variable `aPos`
    // `layout` allows us to define the location of the input variable
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    // We set the return value to `gl_Position`
    // Here we are converting the 3D input coordinate to a 4D output coordinate
    // The fourth coordinate is used for `perspective division`
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    // Null-terminator for string...
    "}\0";

// User input callback
// Checks on every frame (an iteration of the render loop)
// of keyboard inputs, mouse input, etc.
void processInput(GLFWwindow* window) {
    // Check if the Esc key was pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        // On the next iteration of the render loop
        // the window will close
        glfwSetWindowShouldClose(window, true);
        std::cout << "Escape was pressed" << std::endl;
    }
}

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

    // Return codes and buffer for error messages
    int success = 0;
    char infoLog[256] = {0};

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
        // Process user input
        processInput(window);

        /* Rendering */
        
        // Configure the color setting used by glClear()
        // A minty color
        GLfloat red = 0.58f;
        GLfloat green = 0.96f;
        GLfloat blue = 0.84f;
        GLfloat alpha = 1.0f; 
        glClearColor(red, green, blue, alpha);

        // Apply the color to the window's color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Create a 2D triangle
        // We keep the z-coordinate (depth) at 0.0 to create a 2D triangle in a 3D scene
        // Three 3D coordinates
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // one 3D coordinate
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        // Assign a unique ID to the vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // Attached the vertex shader unique ID to the shader source code
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        // Compile the vertex shader source code
        glCompileShader(vertexShader);

        // Check if compilation was successful
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "Failed to compile vertex shader: " << infoLog << std::endl;
            glfwTerminate();
        }

        // Create a vertex buffer object, which stores the vertices
        // that will be sent to the GPU's memory
        unsigned int VBO;
        // Creates a buffer object behind the scenes, and assigns an ID to it
        unsigned int num_buffers = 1;
        glGenBuffers(num_buffers, &VBO);
        // Specify that the newly created buffer object is specifically a vertex buffer object
        // This binds the GL_ARRAY_BUFFER to the one we created, VBO
        // Any operations on the GL_ARRAY_BUFFER will configure our VBO object
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Copy over the vertices data to the VBO
        // The fourth argument specifies how the GPU should manage the data
        // GL_STATIC_DRAW is best for data that doesn't change much and is read many times
        // If the data changes a lot, we'd use GL_DYNAMIC_DRAW
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        /* Rendering end */

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
