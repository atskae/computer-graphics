// Fragment shader is responsible for calculating the colors
// The light source fragment shader keeps the color constant
#version 460 core

// Fragment shader's only required output, a vector of size 4
out vec4 FragColor;

void main() {
    // Constant white light source
    FragColor = vec4(1.0f);
}
