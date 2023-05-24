// Fragment shader is responsible for calculating the colors
#version 460 core

// Values received from the vertex shader
// Normal vector of the vertex
in vec3 Normal;
// Fragment's position in world-coordinate
in vec3 FragPos;
// Light source's position in view-space
in vec3 LightPos;

// Uniforms, to be set externally (as opposed to passed in by the vertex shader)
uniform vec3 objectColor;
// Color of the light source
uniform vec3 lightColor;

// Fragment shader's only required output, a vector of size 4
out vec4 FragColor;

void main() {
    // Create a vector of the light ray
    vec3 lightDirection = vec3(LightPos - FragPos);
    vec3 normalVec = normalize(Normal);

    // Get the angle between the light ray and the normal vector
    // The angle determines the brightness of the fragment
    // If the angle is closer to 0, the light ray and the normal is more closely aligned
    // causing a brighter affect. When the angle is closer to 90 degrees, the effect is no light
    // The product of two *unit* vectors will give us cos(theta) 
    // We take the max to avoid negative dot product (occurs when the angle > 90)
    float diffuseStrength = max(dot(normalize(lightDirection), normalVec), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    // vec4 color: red, green, blue, alpha (transparency)
    float ambienceStrength = 0.1f;
    vec3 ambience = ambienceStrength * lightColor;

    float specularStrength = 0.1;
    // In view space, we compute relative to the viewer (set at 0,0,0)
    vec3 viewPos = vec3(0,0,0);
    vec3 viewDirection = normalize(viewPos - FragPos);
    
    // -1 since the lightDirection is currently fragPos to light, and we want the opposite direction
    // The reflect() function expects direction light-> fragPos
    vec3 reflectionDirection = reflect(-lightDirection, normalVec); 
    // How much the light is properly reflected versus scattered around
    // Higher values, a smaller area will get intense light (highlights)
    // Lower values, light is spread out across the fragment
    float shininess = 4;
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;
    
    // The final light effect is the addition of diffuse and ambience effect
    // The final color is obtained by multiplying the object's color and the final light effect
    vec3 lightEffect = diffuse + ambience + specular;
    FragColor = vec4(objectColor * lightEffect, 1.0f);
}
