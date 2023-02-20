// Fragment shader is responsible for calculating the colors
#version 460 core

// Values received from the vertex shader
// Normal vector of the vertex
in vec3 Normal;

// Uniforms, to be set externally (as opposed to passed in by the vertex shader)
uniform vec3 objectColor;
// Color of the light source
uniform vec3 lightColor;
// Position of the light source
uniform vec3 lightSourcePos;

// Fragment shader's only required output, a vector of size 4
out vec4 FragColor;

void main() {
    // vec4 color: red, green, blue, alpha (transparency)
    float ambienceStrength = 0.1f;
    vec3 ambience = ambienceStrength * lightColor;
    FragColor = vec4(objectColor * ambience, 1.0f);
}
