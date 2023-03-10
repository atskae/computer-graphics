// Fragment shader is responsible for calculating the colors
// The light source fragment shader keeps the color constant
#version 460 core

// Color of the light source
uniform vec3 lightColor;

// Fragment shader's only required output, a vector of size 4
out vec4 FragColor;

void main() {
    FragColor = vec4(lightColor, 1.0f);
}
