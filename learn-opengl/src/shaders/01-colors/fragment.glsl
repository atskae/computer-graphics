// Fragment shader is responsible for calculating the colors
#version 460 core

// Uniforms, to be set externally (as opposed to passed in by the vertex shader)
uniform vec3 objectColor;
// Color of the light source
uniform vec3 lightColor;

// Fragment shader's only required output, a vector of size 4
out vec4 FragColor;

void main() {
    // vec4 color: red, green, blue, alpha (transparency)
    FragColor = vec4(objectColor * lightColor, 1.0f);
}
