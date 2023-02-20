// OpenGL version 4.6 with core-profile functionality
#version 460 core 

// The `in` keyword specifies all the inputs (input vertex attributes) to this vertex shader
// The input to this shader program is a 3D vector `vec3`
// We name the input variable `aPos`
// `layout` allows us to define the location of the input variable
layout (location = 0) in vec3 aPos;
// Normal vector of the triangle surface
layout (location = 1) in vec3 aNormal;

// Transformation matrices
// Convert local to global
uniform mat4 model;
// Converts global to camera/view space
uniform mat4 view;
// Applies perspective projection
uniform mat4 projection;

// Variables to pass to the fragment shader
// Normal vector
out vec3 Normal;
// World-space coordinate of the fragment
// All the lighting is done in world-space
out vec3 FragPos;

void main() {
    // We set the return value to `gl_Position`
    // Here we are converting the 3D input coordinate to a 4D output coordinate
    // The fourth coordinate is used for `perspective division`
    
    // Apply the transformations
    // Read tranformations from right to left
    // So here, model is applied first, then view, then lastly projection
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    // Pass the normal vector to the fragment shader
    Normal = aNormal;

    // Compute the fragment's world coordinates only
    FragPos = vec3(model * vec4(aPos, 1.0f));
}
