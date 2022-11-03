#include <iostream>
#include <cmath>

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
    // The color to send to the fragment shader
    "out vec4 fragmentShaderColor;\n"
    
    "void main()\n"
    "{\n"
    // We set the return value to `gl_Position`
    // Here we are converting the 3D input coordinate to a 4D output coordinate
    // The fourth coordinate is used for `perspective division`
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    // Set the color as the output to the vertex shader
    // This becomes the input to the fragment shader
    // Salmon color
    "   fragmentShaderColor = vec4(1.0f, 0.54f, 0.42f, 1.0f);"  
    // Null-terminator for string...
    "}\0";

// Source code for the fragment shader
// Fragement shader is responsible for calculating the colors
const char* fragmentShaderSource = "#version 460 core\n"
    // Fragment shader's only required output, a vector of size 4
    // Input passed in from the vertex shader
    "in vec4 fragmentShaderColor;\n"

    // Color that is recieved from the CPU
    "uniform vec4 ourColor;\n" 

    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    // Lime-green color
    //"   FragColor = vec4(0.79f, 0.98f, 0.44f, 1.0f);\n"
    // Minty color
    //"   FragColor = vec4(0.58f, 0.96f, 0.84f, 1.0f);\n"
    // Pink 
    //"   FragColor = vec4(0.90f, 0.5f, 0.85f, 1.0f);\n"
    // Use color from the vertex shader
    "   FragColor = ourColor;\n"
    "}\0";

// Fragment shader source code that colors the object yellow
const char* yellowFragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    // Yellow
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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
    //std::cerr << "I am an error message" << std::endl;

    // Initialize GLFW
    if (glfwInit() == GL_FALSE) {
        std::cerr << "GLFW failed to initialize." << std::endl;
        return 1;
    }

    // Return codes and buffer for error messages
    int success = 0;
    char infoLog[512] = {0};
    
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
    
    GLfloat alpha = 1.0f; 

    // Background color

    //// A minty color
    //GLfloat red = 0.58f;
    //GLfloat green = 0.96f;
    //GLfloat blue = 0.84f;

    // Gray color
    GLfloat red = 0.39f;
    GLfloat green = 0.40f;
    GLfloat blue = 0.42f;

    //// 2D triangle
    int startTriangleIndex = 0;
    
    //// We keep the z-coordinate (depth) at 0.0 to create a 2D triangle in a 3D scene
    //// Three 3D coordinates
    //float vertices[] = {
    //    -0.5f, -0.5f, 0.0f, // bottom-left
    //    0.5f, -0.5f, 0.0f, // bottom-right
    //    0.0f, 0.5f, 0.0f // top
    //};
    //// The index to start in the vertices array
    //unsigned int numVertices = 3;

    // Two triangles
    // Normalized vertices [-1, 1]
    float vertices[] = {
        // First triangle
        -0.6f, 0.0f, 0.0f, // bottom-left
        -0.45f, 0.3f, 0.0f, // top
        -0.3f, 0.0f, 0.0f, // bottom-right
        // Second triangle
        -0.1f, 0.0f, 0.0f, // bottom-right
        0.05f, 0.3f, 0.0f, // top
        0.2f, 0.0f, 0.0f, // bottom-right
    };
    unsigned int numVertices = 6;

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

    // Assign a unique ID to the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach the vertex shader unique ID to the shader source code
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

    // Vertex attribute object
    // Stores the state of glVertexAttribPointer() and related calls
    // Assigns a unique ID to the VAO object (created behind the scenes)
    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);

    // Create a vertex buffer object, which stores the vertices
    // that will be sent to the GPU's memory
    unsigned int VBO[2];
    // Creates a buffer object behind the scenes, and assigns an ID to it
    glGenBuffers(2, VBO);
    
    // Location of input argument in the vertex shader program
    unsigned int vertexAttributeLocation = 0;

    // Set up the VAO and VBO for each triangle 
    for (int i=0; i<2; i++) {
        // Bind the VAO
        // After this bind call, any functions related to vertex buffer objects (VBO) will store
        //  its state inside this VAO
        glBindVertexArray(VAO[i]);
        
        // Specify that the newly created buffer object is specifically a vertex buffer object
        // This binds the GL_ARRAY_BUFFER to the one we created, VBO
        // Any operations on the GL_ARRAY_BUFFER will configure our VBO object
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        
        // Copy over the triangle vertices data to the VBO
        // The fourth argument specifies how the GPU should manage the data
        // GL_STATIC_DRAW is best for data that doesn't change much and is read many times
        // If the data changes a lot, we'd use GL_DYNAMIC_DRAW
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*9, &vertices[i*9], GL_STATIC_DRAW);

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
            sizeof(float) * 3,
            //0,
            // Where the data starts in the buffer
            // Since the data starts at the beginning of the buffer, we use 0
            (void*)0
        );
    
        // Enable the vertex attribute in the vertex shader `(location = 0)`
        glEnableVertexAttribArray(vertexAttributeLocation);
    }

    // Element buffer object
    unsigned int EBO;
    // Create one buffer and assign unique ID
    glGenBuffers(1, &EBO);
    
    // Use the rectangle vertices instead
    //glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);
    
    // Do the same for the Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    const char* fragmentShaderSources[] = {
        fragmentShaderSource,
        yellowFragmentShaderSource
    };
    unsigned int fragmentShaders[2];

    // Compile each fragment shader source code
    for (int i=0; i<2; i++) {
        // Assign a unique ID to the fragment shader
        fragmentShaders[i] = glCreateShader(GL_FRAGMENT_SHADER);
        // Attach the unique ID to the shader source code
        glShaderSource(fragmentShaders[i], 1, &fragmentShaderSources[i], NULL);
        // Compile the fragment shader
        glCompileShader(fragmentShaders[i]);
    
        // Check if compilation was successful
        glGetShaderiv(fragmentShaders[i], GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShaders[i], 512, NULL, infoLog);
            std::cerr << "Failed to compile fragment shader: " << infoLog << std::endl;
            glfwTerminate();
        }
    }    

    // Create two main shader programs
    // Both shader programs use the same vertex shader, but different fragment shaders
    unsigned int shaderPrograms[2];
    for (int i=0; i<2; i++) {
        // Combine the vertex shader and the fragment shader into one main shader program
        shaderPrograms[i] = glCreateProgram();
        if (shaderPrograms[i] == 0) { // So zero means error... ..
            std::cerr << "Failed to get shader program ID" << std::endl;
            glfwTerminate();
        }
    
        glAttachShader(shaderPrograms[i], vertexShader);
        glAttachShader(shaderPrograms[i], fragmentShaders[i]);
        glLinkProgram(shaderPrograms[i]);
    
        // Check if shader linking was successful
        glGetProgramiv(shaderPrograms[i], GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderPrograms[i], 512, NULL, infoLog);
            std::cerr << "Failed to link shader program: " << infoLog << std::endl;
            glfwTerminate();
        }
    }
        
    // We can delete the vertex and fragment shader objects after the final shader program has been linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaders[0]);
    glDeleteShader(fragmentShaders[1]);
    
    // Start the render loop
    // This keeps the application running and handles new input
    //  until the application is closed
    while (!glfwWindowShouldClose(window)) {
        // Process user input
        processInput(window);

        /* Rendering */
        
        // Configure the color setting used by glClear()
        glClearColor(red, green, blue, alpha);

        // Apply the color to the window's color buffer
        glClear(GL_COLOR_BUFFER_BIT);
        
        // (re) activate the EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        
        // Draw each triangle!!!!!
        // Draw a primitive, in this case a triangle
        for (int i=0; i<2; i++) {
            // Set the shader program as the currently active shader program
            glUseProgram(shaderPrograms[i]);

            // Change the color of one triangle on each iteration of the render loop
            if (i==0) {
                // Compute a color here and send the color to the fragment shader
                //  by setting the uniform variable in that shader
                
                // Get the number of seconds that elapsed since GLFW was initialized
                float timeValue = glfwGetTime();

                // Compute a value between [0.0, 1.0]
                // sin() returns a value from [-1, 1], so we have to divide by 2 and add 0.5
                //  to keep the range between 0 and 1
                float redValue = (sin(timeValue) / 2.0f) + 0.5f;

                // Get the index/location of the uniform variable in the shader program
                int vertexColorLocation = glGetUniformLocation(shaderPrograms[i], "ourColor");
                if (vertexColorLocation == -1) {
                    std::cerr << "Could not find location of uniform variable `ourColor`" << std::endl;
                    continue;
                }

                // Set the color to the uniform variable in the active shader
                glUniform4f(vertexColorLocation, redValue, 0.0f, 0.0f, 1.0f);
            }

            // Activate the VAO for this triangle
            // The VAO already has the reference to the VBO which contains this triangle's vertices
            //  so we don't have to activate the VBO again
            glBindVertexArray(VAO[i]);
            // Zeichnen!
            glDrawArrays(GL_TRIANGLES, startTriangleIndex, 3);
        }

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
