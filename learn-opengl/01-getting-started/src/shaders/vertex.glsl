// OpenGL version 4.6 with core-profile functionality
#version 460 core 

// The `in` keyword specifies all the inputs (input vertex attributes) to this vertex shader
// The input to this shader program is a 3D vector `vec3`
// We name the input variable `aPos`
// `layout` allows us to define the location of the input variable
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord1;
layout (location = 3) in vec2 aTexCoord2;

// Offset to add to the x-coordinate
uniform float horizontalOffset;

// Values to send to the fragment shader
out vec3 fragmentShaderColor;
out vec2 texCoord1;
out vec2 texCoord2;

void main() {
    // We set the return value to `gl_Position`
    // Here we are converting the 3D input coordinate to a 4D output coordinate
    // The fourth coordinate is used for `perspective division`
    vec4 position = vec4(aPos.x + horizontalOffset, aPos.y, aPos.z, 1.0f); 
    gl_Position = position;
    
    // Set the color as the output to the vertex shader
    // This becomes the input to the fragment shader
    // Color that was recieved from the CPU-side
    fragmentShaderColor = aColor;
    
    texCoord1 = aTexCoord1;
    texCoord2 = aTexCoord2;
}
