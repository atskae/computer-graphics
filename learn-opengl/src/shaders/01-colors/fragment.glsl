// Fragment shader is responsible for calculating the colors
#version 460 core

// Values received from the vertex shader
// Normal vector of the vertex
in vec3 Normal;
// Fragment's position in world-coordinate
in vec3 FragPos;

// Uniforms, to be set externally (as opposed to passed in by the vertex shader)
uniform vec3 objectColor;
// Color of the light source
uniform vec3 lightColor;
// Position of the light source
uniform vec3 lightPos;

// Fragment shader's only required output, a vector of size 4
out vec4 FragColor;

void main() {
    // Create a vector of the light ray
    vec3 lightDirection = vec3(lightPos - FragPos);
    // Get the angle betweenthe angle between them the light ray and the normal vector
    // The angle determines the brightness of the fragment
    // If the angle is closer to 0, the light ray and the normal is more closely aligned
    // causing a brighter affect. When the angle is closer to 90 degrees, the effect is no light
    // The product of two *unit* vectors will give us cos(theta) 
    // We take the max to avoid negative dot product (occurs when the angle > 90)
    float diffuseStrength = max(dot(normalize(lightDirection), normalize(Normal)));
    vec3 diffuse = diffuseStrength * lightColor;

    // vec4 color: red, green, blue, alpha (transparency)
    float ambienceStrength = 0.1f;
    vec3 ambience = ambienceStrength * lightColor;
    
    // The final light effect is the addition of diffuse and ambience effect
    // The final color is obtained by multiplying the object's color and the final light effect
    float lightEffect = diffuse + ambience;
    FragColor = vec4(objectColor * lightEffect, 1.0f);
}
