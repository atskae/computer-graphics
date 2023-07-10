// Fragment shader is responsible for calculating the colors
#version 460 core

// Properties of a material surface
struct Material {
    //vec3 ambient;
    //vec3 diffuse;
    // The texture serves as the ambient and diffuse effects
    // texture(material.diffuse, textureCoordinates) becomes the color
    sampler2D diffuse;
    //vec3 specular;
    sampler2D specular;
    sampler2D emission;
    sampler2D emission_area;
    float emission_strength;
    float shininess;
};

// Intensities of the light source
struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    // For directional light, position is irrelevant
    // Direction from the light source
    vec3 direction;
};

// Values received from the vertex shader
// Normal vector of the vertex
in vec3 Normal;
// Fragment's position in world-coordinate
in vec3 FragPos;
// Light source's position in view-space
//in vec3 LightPos;
// Texture coordinates
in vec2 TextureCoordinates;

//// Color of the light source
//uniform vec3 lightColor;
// Material properties of the object
uniform Material material;
// Intensities and position of the light source
uniform Light light;

// Fragment shader's only required output, a vector of size 4
out vec4 FragColor;

void main() {
    // Create a vector of the light ray
    //vec3 lightDirection = vec3(LightPos - FragPos);
    // Directional light, light position is irrelevant
    // We negate the light direction since it was set to the vector
    //  pointing away from the light source
    // In our calculation we want the vector pointing toward our light source
    vec3 lightDirection = normalize(-light.direction);
    vec3 normalVec = normalize(Normal);

    // Get the angle between the light ray and the normal vector
    // The angle determines the brightness of the fragment
    // If the angle is closer to 0, the light ray and the normal is more closely aligned
    // causing a brighter affect. When the angle is closer to 90 degrees, the effect is no light
    // The product of two *unit* vectors will give us cos(theta) 
    // We take the max to avoid negative dot product (occurs when the angle > 90)
    float diffuseStrength = max(dot(normalize(lightDirection), normalVec), 0.0);
    vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(material.diffuse, TextureCoordinates)) +
        (material.emission_strength * vec3(texture(material.emission, TextureCoordinates)) * vec3(texture(material.emission_area, TextureCoordinates)));

    // vec4 color: red, green, blue, alpha (transparency)
    //vec3 ambience = light.ambient * vec3(texture(material.emission, TextureCoordinates));
    vec3 ambience = light.ambient * vec3(texture(material.diffuse, TextureCoordinates));
    
    // In view space, we compute relative to the viewer (set at 0,0,0)
    vec3 viewPos = vec3(0,0,0);
    vec3 viewDirection = normalize(viewPos - FragPos);
    
    // -1 since the lightDirection is currently fragPos to light, and we want the opposite direction
    // The reflect() function expects direction light-> fragPos
    vec3 reflectionDirection = reflect(-normalize(lightDirection), normalVec); 
    // How much the light is properly reflected versus scattered around
    // Higher values, a smaller area will get intense light (highlights)
    // Lower values, light is spread out across the fragment
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    vec3 texture_color = vec3(texture(material.specular, TextureCoordinates));
    vec3 specular = light.specular * texture_color * spec;
    
    // The final light effect is the addition of diffuse and ambience effect
    // The final color is obtained by multiplying the object's color and the final light effect
    vec3 lightEffect = diffuse + ambience + specular;
    FragColor = vec4(lightEffect, 1.0f);
}
