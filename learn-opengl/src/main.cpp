#include <iostream>
#include <cmath>
#include <algorithm> // clamp
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL mathematics library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // value_ptr()
#include <glm/gtx/string_cast.hpp> // to_string()

// Read in image files
//#include "stb_image.h"

// Imgui UI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Assimp
#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "shader.h"
#include "camera.h"
#include "model.h"

// Test import
Assimp::Importer importer;

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 750;
const bool IS_FPS = false;

bool mouse_pressed = false;

Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, IS_FPS);

// Location of the light source in the scene
// Position in World Space
const glm::vec3 lightPos(1.5, 1.0f, 2.0f);

// Light source color
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

// Imgui color
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

const int NUM_POINT_LIGHTS = 4;

const glm::vec3 DEFAULT_AMBIENCE = glm::vec3(0.25);
const glm::vec3 DEFAULT_DIFFUSE = glm::vec3(0.75);
const glm::vec3 DEFAULT_SPECULAR = glm::vec3(0.25);


// Equivalent struct in the fragment shader
struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct DirectionalLight {
    bool enabled;
    glm::vec3 direction; // from light source to fragment
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight {
    bool enabled[NUM_POINT_LIGHTS] = {true};
    glm::vec3 position[NUM_POINT_LIGHTS] = {glm::vec3(0.0)};
    
    glm::vec3 ambient = DEFAULT_AMBIENCE;
    glm::vec3 diffuse[NUM_POINT_LIGHTS] = {DEFAULT_DIFFUSE};
    glm::vec3 specular = DEFAULT_SPECULAR;

    // Values for different distances: https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    // Here we chose settings for distance of up to 50
    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;
};

// A flashlight that follows the camera/user
struct SpotLight {
    bool enabled;
    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    // The vector from the camera to what it is looking at (it's front vector)
    glm::vec3 spot_dir;
    // The angle of the inner cone, in degrees
    float inner_cutoff_degrees;
    // The angle of the outer cone, in degrees
    float outer_cutoff_degrees;
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse && mouse_pressed) {
        camera.updateFrontVector(xpos, ypos);
    }
}

// Handler for checking when the mouse is pressed and released
void mouse_pressed_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouse_pressed = true;
    } else {
        mouse_pressed = false;
    }
}
 
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse && mouse_pressed) {
        // Ignore the xoffset for mouse scroll changes
        camera.updateFieldOfView(yoffset);
    }
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

    
    // Our state
    
    // Draw in wireframe mode (only draw outlines of primitive shapes, no fill)
    // Draw both front and back of the primitive shape
    //  and only draw the lines of the primitives.
    // GL_FILL would draw primitives with the fill color
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Specify window to OpenGL
    // Create the viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Register the window resize callback
    // When the user resizes the window, framebuffer_size_callback() gets called
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Register the mouse movement callback
    // This function is called everytime the cursor moves
    glfwSetCursorPosCallback(window, mouse_callback);
    
    // Only move the camera if the mouse is pressed down
    glfwSetMouseButtonCallback(window, mouse_pressed_callback);

    // Register mouse scroll callback
    // This function tracks when the user scrolls with wheel or trackpad
    glfwSetScrollCallback(window, scroll_callback);

    // When the window is active, keep the cursor in the middle (capture) and hide the cursor
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

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
    ImGui_ImplOpenGL3_Init("#version 460");

    /* Render-loop configuration */
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

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

    //// Positions where we move and place each cube
    //glm::vec3 cubePositions[10] = {
    //    glm::vec3( 0.0f,  0.0f,  0.0f), 
    //    glm::vec3( 2.0f,  5.0f, -15.0f), 
    //    glm::vec3(-1.5f, -2.2f, -2.5f),  
    //    glm::vec3(-3.8f, -2.0f, -12.3f),  
    //    glm::vec3( 2.4f, -0.4f, -3.5f),  
    //    glm::vec3(-1.7f,  3.0f, -7.5f),  
    //    glm::vec3( 1.3f, -2.0f, -2.5f),  
    //    glm::vec3( 1.5f,  2.0f, -2.5f), 
    //    glm::vec3( 1.5f,  0.2f, -1.5f), 
    //    glm::vec3(-1.3f,  1.0f, -1.5f)  
    //};

    // Rectangle using an Element Buffer Object (EBO)
    // The z-coordinates are zero to keep it 2D in a 3D space
    //float numAwesomeFaces = 1.0f; // per row/column
    //float rectangle_vertices[] = {
    //    // Positions            // Colors               // Texture coordinates
    //    0.5f, 0.5f, 0.0f,       1.0f, 1.0f, 0.0f,       1.0f, 1.0f,     numAwesomeFaces, numAwesomeFaces,   // top-right
    //    0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 1.0f,       1.0f, 0.0f,     numAwesomeFaces, 0.0f,              // bottom-right
    //    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 1.0f,       0.0f, 0.0f,     0.0f, 0.0f,                         // bottom-left
    //    -0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,       0.0f, 1.0f,     0.0f, numAwesomeFaces               // top-right
    //};
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
    
    //unsigned int textureAttributeLocation = 2;
    
    //// Read in each texture image
    //// Images by default have y=0.0 at the top of the image, but OpenGL expects y=0.0 at the bottom
    //// Configure stdbi library to flip the y-axis
    //stbi_set_flip_vertically_on_load(true);
    
    //const char* textureFilenames[] = {
    //    "textures/container2.png",
    //    "textures/container2_specular.png",
    //    "textures/matrix.jpg",
    //    "textures/container2_specular_inverted.png",
    //    "textures/linux-penguin-with-outline.png",
    //};
    //unsigned int textureIds[] = {0};
    //GLenum imageFormats[] = {GL_RGBA, GL_RGBA, GL_RGB, GL_RGBA, GL_RGBA};
    //for (int i=0; i<4; i++) {
    //    // Configure the input argument in our vertex shader
    //    // to accept the texture coordinates 
    //    glVertexAttribPointer(
    //        // (location = 2)
    //        textureAttributeLocation+i,
    //        // Number of components per vertex attribute
    //        // Each texture coordinate is a (s,t) pair
    //        2,
    //        // The data type of the texture coordinate
    //        GL_FLOAT,
    //        // No need to normalize the data to [-1, 1]
    //        GL_FALSE,
    //        // The number of bytes between the first element of each texture coordinate
    //        //  aka the stride
    //        sizeof(float) * verticesStride,
    //        // Pointer offset to the first texture coordinate in the vertices data
    //        (void*)(sizeof(float)*3)
    //        // 6 is where texture coordinates start in general in the rectangle_vertices array
    //        // + 2*i will choose either texture1 (i=0) or texture2 (i=1)
    //        //(void*)(sizeof(float)*(6 + 2*i))
    //    );
    //    glEnableVertexAttribArray(textureAttributeLocation+i);

    //    const char* textureFilename = textureFilenames[i];
    //    std::cout << "Opening texture file: " << textureFilename << std::endl;
    //    int width, height, numColorChannels;
    //    // Reads in the image, and computes the width, height, and number of color channels in the image
    //    unsigned char* textureImageData = stbi_load(textureFilename, &width, &height, &numColorChannels, 0);
    //    if (!textureImageData) {
    //        std::cerr << "Failed to load texture image: " << textureFilename << std::endl;
    //        return 1;
    //    }

    //    // Create an OpenGL texture object, and get the unique ID assigned to the object
    //    unsigned int textureId;
    //    glGenTextures(1, &textureId);

    //    // Bind/activate the texture object so subsequent 2D texure configurations will apply
    //    // to the texture object we just created
    //    glBindTexture(GL_TEXTURE_2D, textureId);

    //    // Configure how OpenGL will apply the texture with out-of-bounds coordinates
    //    // Texture coordinate labels: (s,t,r)
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //    // When the texture is minimized, linearly interpolate between the two closest minmaps
    //    //  and sample the interpolated minmap level with linear interpolation
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //    // When magnifying, stay on the same minmap level, and linearly interpolate the color value
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //    // Set the loaded texture image to the OpenGL texture object
    //    glTexImage2D(
    //        // Texture target
    //        GL_TEXTURE_2D,
    //        // Mipmap level - level of detail number
    //        // n=0 is the original image, n is the nth downsized texture image
    //        0,
    //        // Image format to store the texture in
    //        GL_RGB,
    //        // Width and height of the *resulting* texture
    //        width,
    //        height,
    //        // This value always has to be zero, due to legacy...
    //        // What if we don't pass in zero...?
    //        0,
    //        // Format of the loaded texture image data
    //        // We loaded a `.jpg` texture image as an unsigned char
    //        imageFormats[i],
    //        GL_UNSIGNED_BYTE,
    //        // Actual image data
    //        textureImageData
    //    );
    
    //    // Generate mipmaps for this texture
    //    // Can do the equivalent for each image size by calling `glTexImage2D()`
    //    //  for each mipmap level to generate
    //    glGenerateMipmap(GL_TEXTURE_2D);

    //    // Since the texture object has its own copy of the texture image data
    //    //  we can free the loaded image
    //    stbi_image_free(textureImageData);

    //    // Save the texture ID
    //    textureIds[i] = textureId;
    //}
    
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
    //Shader shaderProgram = Shader(
    //    "Cubes",
    //    "shaders/vertex.glsl",
    //    "shaders/fragment.glsl"
    //);

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
        "shaders/02-lighting/vertex.glsl",
        "shaders/02-lighting/light_source_fragment.glsl"
    );

    // This shader is responsible for adding the light's effects     
    Shader lightingShader(
        "Lighting effects",
        "shaders/02-lighting/vertex.glsl",
        //"shaders/02-lighting/gouraud_vertex.glsl",
        "shaders/02-lighting/fragment.glsl"
        //"shaders/02-lighting/gouraud_fragment.glsl"
    );

    lightSourceShader.use();
    //lightSourceShader.setVec3("lightColor", lightColor);

    
    bool pointLightEnabled[NUM_POINT_LIGHTS] = {
        true, true, true, true
    };

    // Activate so setting the values actually applies
    lightingShader.use();

    // Set the object color and light color
    glm::vec3 coralColor(1.0f, 0.5f, 0.3f);
    
    //lightingShader.setVec3("objectColor", coralColor);
    //lightingShader.setVec3("lightColor", lightColor);
    //lightingShader.setVec3("lightPos", lightPos);

    // Set the materials properties
    //lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    //lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    //lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    //lightingShader.setFloat("material.shininess", 2.0);

    //// Emerald
    //Material material = {
    //    glm::vec3(0.0215, 0.1745, 0.0215), // ambient
    //    glm::vec3(0.07568, 0.6124, 0.07568), // diffuse
    //    glm::vec3(0.633, 0.727811, 0.633), // specular
    //    //0.6 // shininess
    //    256
    //};

    // Cyan plastic
    Material material = {
        glm::vec3(0, 0.1, 0.06),
        glm::vec3(0, 0.50980392, 0.50196078),
        glm::vec3(0.50196078, 0.50196078, 0.50196078),
        256
    };

    //// Gold
    //Material material = {
    //    glm::vec3(0.24725, 0.1995, 0.0745),
    //    glm::vec3(0.75164, 0.60648, 0.22648),
    //    glm::vec3(0.628281, 0.555802, 0.366065),
    //    128
    //};

    //lightingShader.setVec3("material.ambient", material.ambient);
    //lightingShader.setVec3("material.diffuse", material.diffuse);
    lightingShader.setVec3("material.specular", material.specular);
    lightingShader.setFloat("material.shininess", material.shininess);

    // Set the light intensities for each component
    //lightingShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    //lightingShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    //lightingShader.setVec3("light.specular", glm::vec3(0.5, 0.5, 0.5));
        
    DirectionalLight directional_light = {
        true, // enabled
        glm::vec3(-0.2f, -1.0f, -0.3f), // direction
        DEFAULT_AMBIENCE,
        DEFAULT_DIFFUSE,
        DEFAULT_SPECULAR,
    };

    lightingShader.setBool("directionalLight.enabled", directional_light.enabled);
    lightingShader.setVec3("directionalLight.direction", directional_light.direction);
    lightingShader.setVec3("directionalLight.ambient", directional_light.ambient);
    lightingShader.setVec3("directionalLight.diffuse", directional_light.diffuse);
    lightingShader.setVec3("directionalLight.specular", directional_light.specular);
    
    PointLight pointLights;
    pointLights.position[0] = glm::vec3(1.5f, 1.0f, 2.0f);
    pointLights.position[1] = glm::vec3(0.0, 1.0f, -5.0f);
    pointLights.position[2] = glm::vec3(1.5f, -3.0f, 2.0f);
    pointLights.position[3] = glm::vec3(1.5f, 3.0f, -3.0f);

    for (int i=0; i<NUM_POINT_LIGHTS; i++) {
        char buffer[100];
        int result = std::sprintf(buffer, "pointLights.enabled[%i]", i);
        lightingShader.setBool(std::string(buffer, result), pointLights.enabled[i]);

        result = std::sprintf(buffer, "pointLightPos[%i]", i);
        lightingShader.setVec3(std::string(buffer, result), pointLights.position[i]);

        result = std::sprintf(buffer, "pointLights.diffuse[%i]", i);
        lightingShader.setVec3(std::string(buffer, result), pointLights.diffuse[i]);
    }
    
    lightingShader.setVec3("pointLights.ambient", pointLights.ambient);
    lightingShader.setVec3("pointLights.specular", pointLights.specular);

    lightingShader.setFloat("pointLights.constant", pointLights.constant);
    lightingShader.setFloat("pointLights.linear", pointLights.linear);
    lightingShader.setFloat("pointLights.quadratic", pointLights.quadratic);

    // Spotlight
    SpotLight spot_light = {
        true, // enabled,
        DEFAULT_AMBIENCE,
        DEFAULT_DIFFUSE,
        DEFAULT_SPECULAR,
        camera.getFront(), // spot_dir, direction of the spotlight
        20.0, // inner cutoff, in degrees
        40.0, // outer cutoff, in degrees
    };

    lightingShader.setBool("spotLight.enabled", spot_light.enabled);
    lightingShader.setVec3("spotLight.spot_dir", spot_light.spot_dir);
    lightingShader.setFloat("spotLight.cos_inner_cutoff", glm::cos(glm::radians(spot_light.inner_cutoff_degrees)));
    lightingShader.setFloat("spotLight.cos_outer_cutoff", glm::cos(glm::radians(spot_light.outer_cutoff_degrees)));

    lightingShader.setVec3("spotLight.ambient", spot_light.ambient);
    lightingShader.setVec3("spotLight.diffuse", spot_light.diffuse);
    lightingShader.setVec3("spotLight.specular", spot_light.specular);

    //// Define the View matrix, which captures a scene in the view of the camera
    //// We aren't really moving the camera, we are moving the scene relative to a camera at the origin (?)
    //// So to give the view of the camera that moved away from the scene, we move the scene away from the camera
    //glm::mat4 view(1.0f);
    //// Translate the vertices into the screen (-z axis)
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    //int viewLoc = glGetUniformLocation(shaderProgram.getProgramId(), "view");
    //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // Set the diffuse map to the correct texture unit
    //lightingShader.setInt("material.diffuse", 0);
    //lightingShader.setInt("material.specular", 1);
    //// Use the matrix emission map
    //lightingShader.setInt("material.emission", 2);
    //// Indicate where the emission occurs
    //lightingShader.setInt("material.emission_area", 3);
    
    // Load the backpack model
    std::filesystem::path modelPath("textures");
    modelPath /= "backpack";
    modelPath /= "backpack.obj";
    Model backpack(modelPath.c_str());

    // Start the render loop
    // This keeps the application running and handles new input
    //  until the application is closed
    while (!glfwWindowShouldClose(window)) {
        
        // Checks for new input (ex. mouse/keyboard input) and updates the state
        // Registered callbacks are executed 
        glfwPollEvents();
        
        // Apply the color to the window's color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Clear the previous frames depth buffer information
        glClear(GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
                
        // Update the camera speed based on this frame's render time
        camera.updateSpeed();

        // Process user input
        if (!io.WantCaptureMouse) {
            processInput(window, lightingShader);
        }

        /* Rendering */
        
        // Configure the color setting used by glClear()
        if (directional_light.enabled) {
                glClearColor(
                directional_light.diffuse[0],
                directional_light.diffuse[1],
                directional_light.diffuse[2],
                1.0 // alpha
            );
        } else {
            glClearColor(0.0, 0.0, 0.0, 0.0);
        }

        //// Make the texture object that we created the active texture object
        //// Bind each texture to its own texture unit in the fragment shader
        //glActiveTexture(GL_TEXTURE0); // activate the texture unit first
        //glBindTexture(GL_TEXTURE_2D, textureIds[0]);    

        //glActiveTexture(GL_TEXTURE1); // activate the texture unit first
        //glBindTexture(GL_TEXTURE_2D, textureIds[1]);    

        //// Matrix emission map    
        //glActiveTexture(GL_TEXTURE2);
        //glBindTexture(GL_TEXTURE_2D, textureIds[2]);    

        //// Matrix emission map area
        //glActiveTexture(GL_TEXTURE3);
        //glBindTexture(GL_TEXTURE_2D, textureIds[3]);    

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
        lightingShader.use();
        
        glm::mat4 projection = camera.getPerspectiveMatrix();
        lightingShader.setMatrix("projection", projection);
        //shaderProgram.setMatrix("projection", projection);

        //// Set the view matrix
        //// Rotate the camera along the y-axis over time
        ////float cameraX = cos(glfwGetTime()) * rotationRadius;
        ////float cameraZ = -1 * sin(glfwGetTime()) * rotationRadius; // negative 1 for clockwise rotation
        glm::mat4 viewMatrix = camera.getLookAtMatrix();
        lightingShader.setMatrix("view", viewMatrix);
        //shaderProgram.setMatrix("view", viewMatrix);

        // Now draw the cube object that is getting hit by the light source
        glBindVertexArray(VAO);
        
        lightingShader.setBool("directionalLight.enabled", directional_light.enabled);
        lightingShader.setVec3("directionalLight.ambient", directional_light.ambient);
        lightingShader.setVec3("directionalLight.diffuse", directional_light.diffuse);
        lightingShader.setVec3("directionalLight.specular", directional_light.specular);

        for (int i=0; i<NUM_POINT_LIGHTS; i++) {
            char buffer[100];
            int result = std::sprintf(buffer, "pointLights.enabled[%i]", i);
            lightingShader.setBool(std::string(buffer, result), pointLights.enabled[i]);

            result = std::sprintf(buffer, "pointLightPos[%i]", i);
            lightingShader.setVec3(std::string(buffer, result), pointLights.position[i]);

            result = std::sprintf(buffer, "pointLights.diffuse[%i]", i);
            lightingShader.setVec3(std::string(buffer, result), pointLights.diffuse[i]);
        }
    
        lightingShader.setVec3("pointLights.ambient", pointLights.ambient);
        lightingShader.setVec3("pointLights.specular", pointLights.specular);

        lightingShader.setFloat("pointLights.constant", pointLights.constant);
        lightingShader.setFloat("pointLights.linear", pointLights.linear);
        lightingShader.setFloat("pointLights.quadratic", pointLights.quadratic);
        
        lightingShader.setBool("spotLight.enabled", spot_light.enabled);
        lightingShader.setVec3("spotLight.ambient", spot_light.ambient);
        lightingShader.setVec3("spotLight.diffuse", spot_light.diffuse);
        lightingShader.setVec3("spotLight.specular", spot_light.specular);
        lightingShader.setVec3("spotLight.spot_dir", spot_light.spot_dir);
        lightingShader.setFloat("spotLight.cos_inner_cutoff", glm::cos(glm::radians(spot_light.inner_cutoff_degrees)));
        lightingShader.setFloat("spotLight.cos_outer_cutoff", glm::cos(glm::radians(spot_light.outer_cutoff_degrees)));

        //// Draw each cube positioned at different locations
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
        //    lightingShader.setMatrix("model", model_matrix);

        //    //// Change the emission over time
        //    //float emission_strength = abs(cos(glfwGetTime() + i));
        //    //float emission_strength = 1.0f;
        //    //lightingShader.setFloat("material.emission_strength", emission_strength);

        //    // Draw the cube!
        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        // Create a transformation matrix
        glm::mat4 model_matrix(1.0f);
        lightingShader.setMatrix("model", model_matrix);

        // Draw the backpack
        backpack.draw(lightingShader);

        //lightColor.x = sin(glfwGetTime() * 2.0f);
        //lightColor.y = sin(glfwGetTime() * 0.7f);
        //lightColor.z = sin(glfwGetTime() * 1.3f);

        //// Need to activate so changes apply 
        glBindVertexArray(lightVAO);
        lightSourceShader.use();

        // Draw each point light
        for (int i=0; i<NUM_POINT_LIGHTS; i++) {
            // Set the model, view, and projection matrices
            // Model matrix
            glm::mat4 model(1.0f);
        
            ////// Rotate the camera around the y-axis over time
            ////double timeStamp = glfwGetTime();
            ////double timeStamp = 1.0;
            ////float lightPosX = cos(timeStamp) * rotationRadius;
            ////float lightPosZ = -1 * sin(timeStamp) * rotationRadius; // negative 1 for clockwise rotation
            //////glm::vec3 lightPos = glm::vec3(lightPosX, 1.0f, lightPosZ);
            //glm::vec3 lightPos = glm::vec3(1.2, 1.0, 2);

            model = glm::translate(model, pointLights.position[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // scale down
            lightSourceShader.setMatrix("model", model);

            // View Matrix
            lightSourceShader.setMatrix("view", viewMatrix);

            // Projection matrix
            lightSourceShader.setMatrix("projection", projection);

            // Update light color
            if (pointLightEnabled[i]) {
                lightSourceShader.setVec3("lightColor", pointLights.diffuse[i]);
            } else {
                // If disabled, set to black
                lightSourceShader.setVec3("lightColor", glm::vec3(0.0));
            }
        
            // Draw
            // type, starting index, number of *vertices*
            glDrawArrays(GL_TRIANGLES, 0, 36);
        } 
                
        // Change the cube's color over time
        //glm::vec3 ambientColor = lightColor * glm::vec3(0.2);
        //glm::vec3 diffuseColor = lightColor * glm::vec3(0.5);
        //lightingShader.setVec3("lightColor", lightColor);
        //lightingShader.setVec3("light.diffuse", diffuseColor);
        //lightingShader.setVec3("light.ambient", ambientColor);
        
        ////float shininess = sin(glfwGetTime());
        //float shininess = 2;
        ////std::cout << "Shininess: " << shininess << std::endl;
        //lightingShader.setFloat("material.shininess", shininess);
        
        // Update the viewer's position
        //lightingShader.setVec3("viewPos", viewPos);
        
        // Model matrix
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        //lightingShader.setMatrix("model", model);
        //// View Matrix
        //lightingShader.setMatrix("view", viewMatrix);
        //// Projection matrix
        //lightingShader.setMatrix("projection", projection);

        //glDrawArrays(GL_TRIANGLES, 0, 36);

        /* Rendering end */

        // Display UI
        
        ImGui::Begin("ImGui Controls");

        ImGui::Text("Directional Light Settings");
        ImGui::Checkbox("Enable Directional Light", (bool*)&directional_light.enabled);
        ImGui::ColorEdit3("Ambient##Directional Light", (float*)&directional_light.ambient);
        ImGui::ColorEdit3("Diffuse##Directional Light", (float*)&directional_light.diffuse);
        ImGui::ColorEdit3("Specular##Directional Light", (float*)&directional_light.specular);

        ImGui::Text("Point Light Settings");
        ImGui::ColorEdit3("Ambient##Point Light", (float*)&pointLights.ambient);
        ImGui::ColorEdit3("Specular##Point Light", (float*)&pointLights.specular);

        for (int i=0; i<NUM_POINT_LIGHTS; i++) {
            char buffer[100];
            int result = std::sprintf(buffer, "Enable Point Light %i", i);
            ImGui::Checkbox(std::string(buffer, result).c_str(), (bool*)&pointLights.enabled[i]);

            result = std::sprintf(buffer, "Position## Point Light %i", i);
            ImGui::SliderFloat3(std::string(buffer, result).c_str(), (float*)&pointLights.position[i], -25, 25.0);
            
            result = std::sprintf(buffer, "Diffuse##Point Light %i", i);
            ImGui::ColorEdit3(std::string(buffer, result).c_str(), (float*)&pointLights.diffuse[i]);
        }
        
        ImGui::Text("Spotlight Settings");
        ImGui::Checkbox("Enable Spot Light", (bool*)&spot_light.enabled);
        ImGui::ColorEdit3("Ambient##Spot Light", (float*)&spot_light.ambient);
        ImGui::ColorEdit3("Diffuse##Spot Light", (float*)&spot_light.diffuse);
        ImGui::ColorEdit3("Specular##Spot Light", (float*)&spot_light.specular);
        ImGui::SliderFloat("Inner Cutoff Angle", (float*)&spot_light.inner_cutoff_degrees, 0.0, spot_light.outer_cutoff_degrees);
        ImGui::SliderFloat("Outer Cutoff Angle", (float*)&spot_light.outer_cutoff_degrees, 0.0, 90.0);
        
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
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
