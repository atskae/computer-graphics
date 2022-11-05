#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"


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

    // Draw in wireframe mode (only draw outlines of primitive shapes, no fill)
    // Draw both front and back of the primitive shape
    //  and only draw the lines of the primitives.
    // GL_FILL would draw primitives with the fill color
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
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

    /* Render-loop configuration */
    
    // Background color, gray
    GLfloat background_color[] = {
        0.39f, // red
        0.40f, // green
        0.42f, // blue
        1.0f, // alpha
    };

    // 2D triangle
    int startTriangleIndex = 0;
    
    float vertices[] = {
        // CMY triangle
        //// Positions            // Colors
        //-0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.0f, // bottom-left
        //0.0f, 0.5f, 0.0f,       0.0f, 1.0f, 1.0f, // top
        //0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 1.0f, // bottom-right
        // RGB triangle
        // Positions            // Colors
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f, // bottom-left
        0.0f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f, // top
        0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f, // bottom-right
    };

    // Rectangle using an Element Buffer Object (EBO)
    // The z-coordinates are zero to keep it 2D in a 3D space
    float rectangle_vertices[] = {
        0.5f, 0.5f, 0.0f, // top-right
        -0.5f, 0.5f, 0.0f, // top-left
        -0.5f, -0.5f, 0.0f, // bottom-left
        0.5f, -0.5f, 0.0f // bottom-right
    };
    // A list of triangle's (that make up the rectangle) vertices by index
    // This create a rectangle split from top-left to bottom-right
    unsigned int indices[] = {
        0, 1, 3, // top-half triangle
        1, 2, 3 // bottom-half-triangle
    };
    unsigned int numIndices = 6;

    // Vertex attribute object
    // Stores the state of glVertexAttribPointer() and related calls
    // Assigns a unique ID to the VAO object (created behind the scenes)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // Create a vertex buffer object, which stores the vertices
    // that will be sent to the GPU's memory
    unsigned int VBO;
    // Creates a buffer object behind the scenes, and assigns an ID to it
    glGenBuffers(1, &VBO);
    
    // Location of input argument in the vertex shader program
    unsigned int vertexAttributeLocation = 0;

    // Location of the input argument for color in the vertex shader
    unsigned int colorAttributeLocation = 1;

    // Bind the VAO
    // After this bind call, any functions related to vertex buffer objects (VBO) will store
    //  its state inside this VAO
    glBindVertexArray(VAO);
    
    // Specify that the newly created buffer object is specifically a vertex buffer object
    // This binds the GL_ARRAY_BUFFER to the one we created, VBO
    // Any operations on the GL_ARRAY_BUFFER will configure our VBO object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Copy over the triangle vertices data to the VBO
    // The fourth argument specifies how the GPU should manage the data
    // GL_STATIC_DRAW is best for data that doesn't change much and is read many times
    // If the data changes a lot, we'd use GL_DYNAMIC_DRAW
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*18, &vertices, GL_STATIC_DRAW);

    // Specify to OpenGL how to interpret the vertex data in the vertex shader
    // This is applied to the currently active VBO specified in BindBuffer()
    glVertexAttribPointer(
        // The value we specified in our vertex shader, `layout (location = 0)`
        // Where to find the memory location of the vec3 input to the vertex shader
        vertexAttributeLocation,
        // The size of the vertex shader's input (vec3 aPos)
        3,
        // The type of the value in the input vec3
        GL_FLOAT,
        // Indicate whether the data should be normalized ([-1, 1] for signed values, [0, 1] for positive)
        GL_FALSE,
        // Stride, the space between vertex attributes, in bytes
        // If we specify 0, OpenGL tries to figure this out itself
        //  This only works if the data is tightly-packed (no padding between attributes)
        // 3 values for vertex, 3 values for color, total values = 6
        sizeof(float) * 6,
        //0,
        // Where the data starts in the buffer
        // Since the data starts at the beginning of the buffer, we use 0
        (void*)0
    );
    // Enable the vertex attribute in the vertex shader `(location = 0)`
    glEnableVertexAttribArray(vertexAttributeLocation);

    // Configure the color (vertex attribute) in the vertex shader
    glVertexAttribPointer(
        // The value we specified in our vertex shader, `layout (location = 1)`
        // Where to find the memory location of the vec3 input to the vertex shader
        colorAttributeLocation,
        // The size of the vertex shader's input (vec3 aPos)
        3,
        // The type of the value in the input vec3
        GL_FLOAT,
        // Indicate whether the data should be normalized ([-1, 1] for signed values, [0, 1] for positive)
        GL_FALSE,
        // Stride, the space between vertex attributes, in bytes
        // 3 values for vertex, 3 values for color, total values = 6
        sizeof(float) * 6,
        // Where the data starts in the buffer
        // Since the data starts at the beginning of the buffer, we use 0
        (void*)(3 * sizeof(float))
    );
    // Enable the (color) vertex attribute in the vertex shader `(location = 1)`
    glEnableVertexAttribArray(colorAttributeLocation);

    // Element buffer object
    unsigned int EBO;
    // Create one buffer and assign unique ID
    glGenBuffers(1, &EBO);
    
    // Use the rectangle vertices instead
    //glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);
    
    // Do the same for the Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    Shader shaderProgram = Shader(
        "shaders/vertex.glsl",
        "shaders/fragment.glsl"
    );

    // Start the render loop
    // This keeps the application running and handles new input
    //  until the application is closed
    while (!glfwWindowShouldClose(window)) {
        // Process user input
        processInput(window);

        /* Rendering */
        
        // Configure the color setting used by glClear()
        glClearColor(
            background_color[0],
            background_color[1],
            background_color[2],
            background_color[3]
        );

        // Apply the color to the window's color buffer
        glClear(GL_COLOR_BUFFER_BIT);
        
        // (re) activate the EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // Activate the shader program
        shaderProgram.use(); 

        // Add a horizontal offset
        shaderProgram.setFloat("horizontalOffset", 0.0f);

        glBindVertexArray(VAO);
        
        // Zeichnen!
        glDrawArrays(GL_TRIANGLES, startTriangleIndex, 3);

        // Draw a rectangle from the Element Buffer Object that is currently bound
        // The last argument is the starting index of the indices array (...?)
        //glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

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
