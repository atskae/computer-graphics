// Fragment shader is responsible for calculating the colors
#version 460 core

// Input passed in from the vertex shader
in vec3 fragmentShaderColor;
in vec4 vertexPosition;
in vec2 texCoord;

// Texture object generated on the CPU-side
uniform sampler2D ourTexture;

// Fragment shader's only required output, a vector of size 4
out vec4 FragColor;

void main() {
    // Use color from the vertex shader
    //FragColor = fragmentShaderColor;
    //FragColor = vertexPosition;
    // Apply the texture with GLSL texture() function
    // Samples the color from the texture object
    FragColor = texture(ourTexture, texCoord);
}
