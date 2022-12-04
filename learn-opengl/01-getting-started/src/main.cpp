#include <iostream>
#include <cmath>
#include <algorithm> // clamp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL mathematics library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // value_ptr()
#include <glm/gtx/string_cast.hpp> // to_string()

// Read in image files
#include "stb_image.h"

#include "shader.h"


// User input callback
// Checks on every frame (an iteration of the render loop)
// of keyboard inputs, mouse input, etc.
void processInput(GLFWwindow* window, Shader& shaderProgram) {
    // Check if the Esc key was pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        // On the next iteration of the render loop
        // the window will close
        glfwSetWindowShouldClose(window, true);
        std::cout << "Escape was pressed" << std::endl;
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        std::cout << "Up key was pressed" << std::endl;
        float mixPercentage = shaderProgram.getFloat("mixPercentage");
        if (mixPercentage >= 0.0f) {
            float newMixPercentage = std::clamp(mixPercentage + 0.01f, 0.0f, 1.0f);
            shaderProgram.setFloat("mixPercentage", newMixPercentage);
            std::cout << "Increased mixPercentage from " << mixPercentage << " to " << newMixPercentage << std::endl;
        }
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        std::cout << "Down key was pressed" << std::endl;
        float mixPercentage = shaderProgram.getFloat("mixPercentage");
        if (mixPercentage >= 0.0f) {
            float newMixPercentage = std::clamp(mixPercentage - 0.01f, 0.0f, 1.0f);
            shaderProgram.setFloat("mixPercentage", newMixPercentage);
            std::cout << "Decreased mixPercentage from " << mixPercentage << " to " << newMixPercentage << std::endl;
        } 
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
    std::cout << "C++ version: " << __cplusplus << std::endl;

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

    // 3D cube
    // 36 vertices in total
    // 6 faces, 2 triangles per face, 3 vertices per triangle
    float vertices[] = {
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

    // Rectangle using an Element Buffer Object (EBO)
    // The z-coordinates are zero to keep it 2D in a 3D space
    float numAwesomeFaces = 1.0f; // per row/column
    float rectangle_vertices[] = {
        // Positions            // Colors               // Texture coordinates
        0.5f, 0.5f, 0.0f,       1.0f, 1.0f, 0.0f,       1.0f, 1.0f,     numAwesomeFaces, numAwesomeFaces,   // top-right
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 1.0f,       1.0f, 0.0f,     numAwesomeFaces, 0.0f,              // bottom-right
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 1.0f,       0.0f, 0.0f,     0.0f, 0.0f,                         // bottom-left
        -0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,       0.0f, 1.0f,     0.0f, numAwesomeFaces               // top-right
    };
    // Number of floats between each vertice data (position, color, texture)
    int verticesStride = 10;
    
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

    // Bind the VAO
    // After this bind call, any functions related to vertex buffer objects (VBO) will store
    //  its state inside this VAO
    glBindVertexArray(VAO);

    // Location of input argument in the vertex shader program
    unsigned int vertexAttributeLocation = 0;

    // Location of the input argument for color in the vertex shader
    unsigned int colorAttributeLocation = 2;

    // Create vertex buffer objects, which stores the vertices
    // that will be sent to the GPU's memory
    unsigned int VBO;
    
    // Creates a buffer object behind the scenes, and assigns an ID to it
    glGenBuffers(1, &VBO);
              
    // Specify that the newly created buffer object is specifically a vertex buffer object
    // This binds the GL_ARRAY_BUFFER to the one we created, VBO
    // Any operations on the GL_ARRAY_BUFFER will configure our VBO object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Copy over the triangle vertices data to the VBO
    // The fourth argument specifies how the GPU should manage the data
    // GL_STATIC_DRAW is best for data that doesn't change much and is read many times
    // If the data changes a lot, we'd use GL_DYNAMIC_DRAW
    //glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
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
        // 3 values for vertex, 3 values for color, 2 values for texture coordinates, total values = 8
        sizeof(float) * verticesStride,
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
        // 3 values for vertex, 3 values for color, 2 for texture, total values = 8
        sizeof(float) * verticesStride,
        // Where the data starts in the buffer
        // Since the data starts at the beginning of the buffer, we use 0
        (void*)(3 * sizeof(float))
    );
    // Enable the (color) vertex attribute in the vertex shader `(location = 1)`
    glEnableVertexAttribArray(colorAttributeLocation);
    
    //// Element buffer object
    //unsigned int EBO;
    //// Create one buffer and assign unique ID
    //glGenBuffers(1, &EBO);
    
    //// Do the same for the Element Buffer Object
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    /* Textures */
    
    unsigned int textureAttributeLocation = 2;
    
    // Read in each texture image
    // Images by default have y=0.0 at the top of the image, but OpenGL expects y=0.0 at the bottom
    // Configure stdbi library to flip the y-axis
    stbi_set_flip_vertically_on_load(true);
    
    const char* textureFilenames[] = {"textures/container.jpg", "textures/awesomeface.png"};
    unsigned int textureIds[] = {0};
    GLenum imageFormats[] = {GL_RGB, GL_RGBA};
    GLint wrappingParam[] = {GL_REPEAT, GL_REPEAT};
    for (int i=0; i<2; i++) {
        // Configure the input argument in our vertex shader
        // to accept the texture coordinates 
        glVertexAttribPointer(
            // (location = 2)
            textureAttributeLocation+i,
            // Number of components per vertex attribute
            // Each texture coordinate is a (s,t) pair
            2,
            // The data type of the texture coordinate
            GL_FLOAT,
            // No need to normalize the data to [-1, 1]
            GL_FALSE,
            // The number of bytes between the first element of each texture coordinate
            //  aka the stride
            sizeof(float) * verticesStride,
            // Pointer offset to the first texture coordinate in `rectangle_vertices`
            // 6 is where texture coordinates start in general in the rectangle_vertices array
            // + 2*i will choose either texture1 (i=0) or texture2 (i=1)
            (void*)(sizeof(float)*(6 + 2*i))
        );
        glEnableVertexAttribArray(textureAttributeLocation+i);

        const char* textureFilename = textureFilenames[i];
        int width, height, numColorChannels;
        // Reads in the image, and computes the width, height, and number of color channels in the image
        unsigned char* textureImageData = stbi_load(textureFilename, &width, &height, &numColorChannels, 0);
        if (!textureImageData) {
            std::cerr << "Failed to load texture image: " << textureFilename << std::endl;
            return 1;
        }

        // Create an OpenGL texture object, and get the unique ID assigned to the object
        unsigned int textureId;
        glGenTextures(1, &textureId);

        // Bind/activate the texture object so subsequent 2D texure configurations will apply
        // to the texture object we just created
        glBindTexture(GL_TEXTURE_2D, textureId);

        // Configure how OpenGL will apply the texture with out-of-bounds coordinates
        // Texture coordinate labels: (s,t,r)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingParam[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingParam[i]);
        // When the texture is minimized, linearly interpolate between the two closest minmaps
        //  and sample the interpolated minmap level with linear interpolation
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // When magnifying, stay on the same minmap level, and linearly interpolate the color value
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Set the loaded texture image to the OpenGL texture object
        glTexImage2D(
            // Texture target
            GL_TEXTURE_2D,
            // Mipmap level - level of detail number
            // n=0 is the original image, n is the nth downsized texture image
            0,
            // Image format to store the texture in
            GL_RGB,
            // Width and height of the *resulting* texture
            width,
            height,
            // This value always has to be zero, due to legacy...
            // What if we don't pass in zero...?
            0,
            // Format of the loaded texture image data
            // We loaded a `.jpg` texture image as an unsigned char
            imageFormats[i],
            GL_UNSIGNED_BYTE,
            // Actual image data
            textureImageData
        );
    
        // Generate mipmaps for this texture
        // Can do the equivalent for each image size by calling `glTexImage2D()`
        //  for each mipmap level to generate
        glGenerateMipmap(GL_TEXTURE_2D);

        // Since the texture object has its own copy of the texture image data
        //  we can free the loaded image
        stbi_image_free(textureImageData);

        // Save the texture ID
        textureIds[i] = textureId;
    }
    
    /* Textures end */

    Shader shaderProgram = Shader(
        "shaders/vertex.glsl",
        "shaders/fragment.glsl"
    );

    // Activate the shader program
    shaderProgram.use(); 

    // Add a horizontal offset
    shaderProgram.setFloat("horizontalOffset", 0.0f);

    shaderProgram.setFloat("mixPercentage", 0.5f);

    // Map a texture unit to a sampler in the fragment shader
    shaderProgram.setInt("texture1", 0); // assign sampler texture1 to texture unit zero
    shaderProgram.setInt("texture2", 1);

    // Define the Model matrix, which converts local coordinates to global coordinates
    glm::mat4 model(1.0f); // Identiy matrix
    // Rotate around the x-axis
    glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
    glm::vec3 axis_of_rotation(0.5f, 1.0f, 0.0f);
    model = glm::rotate(model, glm::radians(-55.0f), x_axis);

    // Define the View matrix, which captures a scene in the view of the camera
    // We aren't really moving the camera, we are moving the scene relative to a camera at the origin (?)
    // So to give the view of the camera that moved away from the scene, we move the scene away from the camera
    glm::mat4 view(1.0f);
    // Translate the vertices into the screen (-z axis)
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // Define the perspective projection matrix
    float field_of_view = glm::radians(45.0f);
    float aspect_ratio = window_width_pixels / window_height_pixels;
    //glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(
        field_of_view,
        aspect_ratio,
        0.1f, // zmin, where the near plane is
        100.0f // zmax, where the far plane is
    );

    // Start the render loop
    // This keeps the application running and handles new input
    //  until the application is closed
    while (!glfwWindowShouldClose(window)) {
        // Process user input
        processInput(window, shaderProgram);
        
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
        
        // Set the transformation matrices
        int modelLoc = glGetUniformLocation(shaderProgram.getProgramId(), "model");
        // Update the angle of rotation over time
        float angle_of_rotation = (float)glfwGetTime() * glm::radians(55.0f);
        model = glm::rotate(model, angle_of_rotation, axis_of_rotation);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(shaderProgram.getProgramId(), "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(shaderProgram.getProgramId(), "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Make the texture object that we created the active texture object
        // Bind each texture to its own texture unit in the fragment shader
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first
        glBindTexture(GL_TEXTURE_2D, textureIds[0]);    

        glActiveTexture(GL_TEXTURE1); // activate the texture unit first
        glBindTexture(GL_TEXTURE_2D, textureIds[1]);    

        glBindVertexArray(VAO);
        
        // Draw a rectangle from the Element Buffer Object that is currently bound
        // The last argument is the starting index of the indices array (...?)
        // Zeichnen!
        //glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

        // Draw from the VBO
        glDrawArrays(GL_TRIANGLES,
            0, // index of the first element
            36 // number of vertices
        );

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
