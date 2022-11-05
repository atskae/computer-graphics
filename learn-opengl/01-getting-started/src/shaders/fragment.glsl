// Fragement shader is responsible for calculating the colors
#version 460 core

// Input passed in from the vertex shader
in vec4 fragmentShaderColor;

// Fragment shader's only required output, a vector of size 4
out vec4 FragColor;

void main() {
    // Use color from the vertex shader
    FragColor = fragmentShaderColor;
}
