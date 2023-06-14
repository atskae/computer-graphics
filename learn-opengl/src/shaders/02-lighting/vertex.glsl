// OpenGL version 4.6 with core-profile functionality
#version 460 core 

// The `in` keyword specifies all the inputs (input vertex attributes) to this vertex shader
// The input to this shader program is a 3D vector `vec3`
// We name the input variable `aPos`
// `layout` allows us to define the location of the input variable
layout (location = 0) in vec3 aPos;
// Normal vector of the triangle surface
layout (location = 1) in vec3 aNormal;
// Texture coordinates
layout (location = 2) in vec2 aTextureCoordinates;

// Transformation matrices
// Convert local to global
uniform mat4 model;
// Converts global to camera/view space
uniform mat4 view;
// Applies perspective projection
uniform mat4 projection;
// Position of the light source, in world-space
uniform vec3 lightPos;

// Variables to pass to the fragment shader
// Normal vector
out vec3 Normal;
// World-space coordinate of the fragment
// All the lighting is done in world-space
out vec3 FragPos;
// The position of the light source, in view space
out vec3 LightPos;
// Texture coordinates
out vec2 TextureCoordinates;

void main() {
    // We set the return value to `gl_Position`
    // Here we are converting the 3D input coordinate to a 4D output coordinate
    // The fourth coordinate is used for `perspective division`
    
    // Apply the transformations
    // Read tranformations from right to left
    // So here, model is applied first, then view, then lastly projection
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    // Pass the normal vector to the fragment shader
    // To ensure that the normal vector stays at 90 degrees
    // to the surface, we the inverse + transpose to the model matrix
    // model = 4x4 matrix, we take the top-left inner 3x3 matrix of model
    // aNormal = 1x3 vector
    Normal = mat3(transpose(inverse(view * model))) * aNormal;

    // Compute the fragment's world coordinates only
    //FragPos = vec3(model * vec4(aPos, 1.0f));
    // Compute the fragment's view coordinates
    FragPos = vec3(view * model * vec4(aPos, 1.0f));

    // Convert the light source coordinates from world-space to view space
    LightPos = vec3(view * vec4(lightPos, 1.0f));

    // Pass the texture coordinates to the fragment shader
    TextureCoordinates = aTextureCoordinates;
}
