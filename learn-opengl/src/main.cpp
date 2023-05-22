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

// Imgui UI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "shader.h"
#include "camera.h"


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 450;
const bool IS_FPS = false;

Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, IS_FPS);

// Location of the light source in the scene
// Position in World Space
//const glm::vec3 lightPos(1.5, 1.0f, 2.0f);

// Light source color
const glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

// Imgui color
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.updateFrontVector(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // Ignore the xoffset for mouse scroll changes
    camera.updateFieldOfView(yoffset);
}

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
    
    // Update camera settings
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.moveForward();
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.moveBackwards();
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveLeft();
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveRight();
    } 

}

// Window-resize callback
// When the user resizes the window, adjust the OpenGL viewport size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Keep the bottom-left corner at (0, 0)
    glViewport(0, 0, width, height);
}

// Renders the Imgui UI onto the screen
void build_imgui_ui(GLFWwindow* window, ImGuiIO& io) {

    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
    
    // Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    
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
    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
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

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    /* Imgui Setup */
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    
    // Register the window resize callback
    // When the user resizes the window, framebuffer_size_callback() gets called
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Register the mouse movement callback
    // This function is called everytime the cursor moves
    glfwSetCursorPosCallback(window, mouse_callback);

    // Register mouse scroll callback
    // This function tracks when the user scrolls with wheel or trackpad
    glfwSetScrollCallback(window, scroll_callback);

    // When the window is active, keep the cursor in the middle (capture) and hide the cursor
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    
    /* Render-loop configuration */
    
    // Background color, gray
    GLfloat background_color[] = {
        0.39f, // red
        0.40f, // green
        0.42f, // blue
        1.0f, // alpha
    };

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // 2D triangle
    int startTriangleIndex = 0;

    // 3D cube
    // 36 vertices in total
    // 6 faces, 2 triangles per face, 3 vertices per triangle
    float vertices[] = {
        // Back face of cube
        // Triangle           // Texture    // Normal vector
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f, 0.0f, -1.0f,
         // Triangle
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f, 0.0f, -1.0f,

        // Front face of cube
        // Triangle
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        // Triangle 
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, 0.0f, 1.0f,

        // Left face of cube (facing the negative x-axis)
        // Triangle
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        // Triangle
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,

        // Right-face of cube (facing the positive x-axis)
        // Triangle
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
        // Triangle 
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,

        // Bottom face of cube
        // Triangle
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        // Triangle
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f, 0.0f,

        // Top face of cube
        // Triangle
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        // Triangle 
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f, 1.0f, 0.0f,

    }; 

    // Positions where we move and place each cube
    glm::vec3 cubePositions[10] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
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
    int verticesStride = 8;
    
    //// A list of triangle's (that make up the rectangle) vertices by index
    //// This create a rectangle split from top-left to bottom-right
    //unsigned int indices[] = {
    //    0, 1, 3, // top-half triangle
    //    1, 2, 3 // bottom-half-triangle
    //};
    //unsigned int numIndices = 6;

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
    unsigned int colorAttributeLocation = 1;

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
        sizeof(float) * verticesStride,
        //0,
        // Where the data starts in the buffer
        // Since the data starts at the beginning of the buffer, we use 0
        (void*)0
    );
    // Enable the vertex attribute in the vertex shader `(location = 0)`
    glEnableVertexAttribArray(vertexAttributeLocation);

    // Set the normal vectors to the vertex shader
    glVertexAttribPointer(
        1,
        3, // size of the input (vec3),
        GL_FLOAT, // type of the input
        GL_FALSE, // normalize data,
        verticesStride * sizeof(float), // stride of the data
        (void*) (5 * sizeof(float)) // offset into the vertex data where this data starts
    );
    // Enable a vertex attribute array
    // This actively connects the vertex data to the attribute in the shader
    // The above call simply describes how to interpret the data, but not activate it
    glEnableVertexAttribArray(1);

    //// Configure the color (vertex attribute) in the vertex shader
    //glVertexAttribPointer(
    //    // The value we specified in our vertex shader, `layout (location = 1)`
    //    // Where to find the memory location of the vec3 input to the vertex shader
    //    colorAttributeLocation,
    //    // The size of the vertex shader's input (vec3 aPos)
    //    3,
    //    // The type of the value in the input vec3
    //    GL_FLOAT,
    //    // Indicate whether the data should be normalized ([-1, 1] for signed values, [0, 1] for positive)
    //    GL_FALSE,
    //    // Stride, the space between vertex attributes, in bytes
    //    // 3 values for vertex, 3 values for color, 2 for texture, total values = 8
    //    sizeof(float) * verticesStride,
    //    // Where the data starts in the buffer
    //    // Since the data starts at the beginning of the buffer, we use 0
    //    (void*)(3 * sizeof(float))
    //);
    //// Enable the (color) vertex attribute in the vertex shader `(location = 1)`
    //glEnableVertexAttribArray(colorAttributeLocation);
    
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
    
    const char* textureFilenames[] = {"textures/container.jpg", "textures/linux-penguin-with-outline.png"};
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
            // Pointer offset to the first texture coordinate in the vertices data
            (void*)(sizeof(float)*3)
            // 6 is where texture coordinates start in general in the rectangle_vertices array
            // + 2*i will choose either texture1 (i=0) or texture2 (i=1)
            //(void*)(sizeof(float)*(6 + 2*i))
        );
        glEnableVertexAttribArray(textureAttributeLocation+i);

        const char* textureFilename = textureFilenames[i];
        std::cout << "Opening texture file: " << textureFilename << std::endl;
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

    /* Light source */
    
    // Unique integer representing the Vertex Array Object (VAO) for the light source
    unsigned int lightVAO;
    // Generates VAO and assigns a unique integer ID
    glGenVertexArrays(1, &lightVAO);
    // Binds the current VAO
    // Subsequent function calls changes the state for this active VAO
    glBindVertexArray(lightVAO);

    // Bind the same VBO
    // The VBO contains the data for both the light source and the container object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Tell OpenGL how to interpret the vertex data
    // This information is saved in the currently active VAO/VBO
    // Here the stride includes the texture coordinates, but we don't use them here
    glVertexAttribPointer(
        vertexAttributeLocation,
        3, // size of the input (vec3),
        GL_FLOAT, // type of the input
        GL_FALSE, // normalize data,
        verticesStride * sizeof(float), // stride of the data
        (void*)0 // offset into the vertex data where this data starts
    );
    // Enable a vertex attribute array
    // This actively connects the vertex data to the attribute in the shader
    // The above call simply describes how to interpret the data, but not activate it
    glEnableVertexAttribArray(vertexAttributeLocation);

    // Enable a vertex attribute array
    // This actively connects the vertex data to the attribute in the shader
    // The above call simply describes how to interpret the data, but not activate it
    glEnableVertexAttribArray(1);


    /* Shader programs */

    // Compiles the individual shader programs into one
    Shader shaderProgram = Shader(
        "Cubes",
        "shaders/vertex.glsl",
        "shaders/fragment.glsl"
    );

    //shaderProgram.use();
    
    //// Add a horizontal offset
    //shaderProgram.setFloat("horizontalOffset", 0.0f);

    //shaderProgram.setFloat("mixPercentage", 0.5f);

    //// Map a texture unit to a sampler in the fragment shader
    //shaderProgram.setInt("texture1", 0); // assign sampler texture1 to texture unit zero
    //shaderProgram.setInt("texture2", 1);

    // Separate shader for the light source only
    Shader lightSourceShader(
        "Light source",
        "shaders/01-colors/vertex.glsl",
        "shaders/01-colors/light_source_fragment.glsl"
    );

    // This shader is responsible for adding the light's effects     
    Shader lightingShader(
        "Lighting effects",
        "shaders/01-colors/vertex.glsl",
        "shaders/01-colors/fragment.glsl"
    );

    lightSourceShader.use();
    lightSourceShader.setVec3("lightColor", lightColor);

    // Activate so setting the values actually applies
    lightingShader.use();

    // Set the object color and light color
    glm::vec3 coralColor(1.0f, 0.5f, 0.3f);
    
    lightingShader.setVec3("objectColor", coralColor);
    lightingShader.setVec3("lightColor", lightColor);
    //lightingShader.setVec3("lightPos", lightPos);

    //// Define the View matrix, which captures a scene in the view of the camera
    //// We aren't really moving the camera, we are moving the scene relative to a camera at the origin (?)
    //// So to give the view of the camera that moved away from the scene, we move the scene away from the camera
    //glm::mat4 view(1.0f);
    //// Translate the vertices into the screen (-z axis)
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    //int viewLoc = glGetUniformLocation(shaderProgram.getProgramId(), "view");
    //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // Rotation around the origin, around the y-axis
    // for rotating objects such as the camera, light source, ...
    const float rotationRadius = 2.5f;
    
    // Start the render loop
    // This keeps the application running and handles new input
    //  until the application is closed
    while (!glfwWindowShouldClose(window)) {
        
        //// Start the Dear ImGui frame
        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
        //ImGui::NewFrame();

        //// Build our custom UI
        //build_imgui_ui(window, io);
                
        // Update the camera speed based on this frame's render time
        camera.updateSpeed();

        // Process user input
        processInput(window, shaderProgram);
        glm::vec3 viewPos = camera.getPosition();

        /* Rendering */
        
        //// Configure the color setting used by glClear()
        //glClearColor(
        //    background_color[0],
        //    background_color[1],
        //    background_color[2],
        //    background_color[3]
        //);

        // Apply the color to the window's color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Clear the previous frames depth buffer information
        glClear(GL_DEPTH_BUFFER_BIT);
        
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

        //// Draw from the VBO
        //glDrawArrays(GL_TRIANGLES,
        //    0, // index of the first element
        //    36 // number of vertices
        //);

        // Activate the objects shader
        //shaderProgram.use();

        glm::mat4 projection = camera.getPerspectiveMatrix();
        //shaderProgram.setMatrix("projection", projection);

        //// Set the view matrix
        //// Rotate the camera along the y-axis over time
        ////float cameraX = cos(glfwGetTime()) * rotationRadius;
        ////float cameraZ = -1 * sin(glfwGetTime()) * rotationRadius; // negative 1 for clockwise rotation
        glm::mat4 viewMatrix = camera.getLookAtMatrix();
        //shaderProgram.setMatrix("view", viewMatrix);

        //// Draw each cube positioned at different locations
        //int modelLoc = glGetUniformLocation(shaderProgram.getProgramId(), "model");
        //for (int i=0; i<10; i++) {
        //    // Create a transformation matrix for this cube and apply it in the vertex shader
        //    glm::mat4 model_matrix(1.0f);
        //    model_matrix = glm::translate(model_matrix, cubePositions[i]);
        //    model_matrix = glm::rotate(model_matrix, glm::radians(20.f * i), glm::vec3(1.0f, 0.3f, 0.5f));
        //     
        //    //if (i%3 == 0) {
        //    //    // Update the angle of rotation over time
        //    //    float angle_of_rotation = (float)glfwGetTime() * glm::radians(50.0f);
        //    //    model_matrix = glm::rotate(model_matrix, angle_of_rotation, axis_of_rotation);
        //    //}
        //    // Set the model matrix
        //    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
        //    
        //    // Draw the cube!
        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        // Need to activate so changes apply 
        glBindVertexArray(lightVAO);
        lightSourceShader.use();
        
        // Set the model, view, and projection matrices
        // Model matrix
        glm::mat4 model(1.0f);
        
        //// Rotate the camera around the y-axis over time
        //double timeStamp = glfwGetTime();
        double timeStamp = 1.0;
        float lightPosX = cos(timeStamp) * rotationRadius;
        float lightPosZ = -1 * sin(timeStamp) * rotationRadius; // negative 1 for clockwise rotation
        glm::vec3 lightPos = glm::vec3(lightPosX, 1.0f, lightPosZ);

        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // scale down
        lightSourceShader.setMatrix("model", model);

        // View Matrix
        lightSourceShader.setMatrix("view", viewMatrix);

        // Projection matrix
        lightSourceShader.setMatrix("projection", projection);

        // Draw
        // type, starting index, number of *vertices*
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Now draw the cube object that is getting hit by the light source
        glBindVertexArray(VAO);
        lightingShader.use();

        // Use the position calculated at this timestamp for the light source 
        lightingShader.setVec3("lightPos", lightPos);
        
        // Update the viewer's position
        //lightingShader.setVec3("viewPos", viewPos);
        
        // Model matrix
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        lightingShader.setMatrix("model", model);
        // View Matrix
        lightingShader.setMatrix("view", viewMatrix);
        // Projection matrix
        lightingShader.setMatrix("projection", projection);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* Rendering end */

        //// Display UI
        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Checks for new input (ex. mouse/keyboard input) and updates the state
        // Registered callbacks are executed 
        glfwPollEvents();

        // SwapBuffer is a 2D buffer with color values for each pixel in the GLFW window
        // Displays the pixels onto the screen
        glfwSwapBuffers(window);

        // Compute frame render time for computing the
        // camera movement speed in the next frame
        camera.updateTimestamp(glfwGetTime());
    }

    // Clean up Imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Application was closed, clean up all GLFW resources
    std::cout << "Terminating application" << std::endl;
    glfwTerminate();

    return 0;
}
