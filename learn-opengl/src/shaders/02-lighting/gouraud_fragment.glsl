// Fragment shader is responsible for calculating the colors
#version 460 core

// Input from the vertex shader
in vec4 fragmentColor;

// Fragment shader's only required output, a vector of size 4
out vec4 FragColor;

void main() {
    // Already computed the color in the vertex shader
    FragColor = fragmentColor;
}
