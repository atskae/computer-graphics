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

// Light source with no position and are defined to be infinitely far away (ex. sun)
// All light rays have the same direction
// No position is defined (light source is infintely far away)
struct DirectionalLight {
    bool enabled;
    // Vector from the light source to the fragment
    // Direction vector's w component should be zero so they do not get
    // impacted by transformations (since we only care about direction, not position)
    // ex. vec4 direction(x, y, z, 0.0);
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight directionalLight;

struct PointLight {
    //bool enabled;
    
    // Obtain position from vertex shader's calculation
    // vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform PointLight pointLight;
// Point light positions in view space
#define NUM_POINT_LIGHTS 4
in vec3 PointLightPos[NUM_POINT_LIGHTS];
uniform bool pointLightEnabled[NUM_POINT_LIGHTS];

// Values received from the vertex shader
// Normal vector of the vertex
in vec3 Normal;
// Fragment's position in view-space
in vec3 FragPos;
// Light source's position in view-space
in vec3 LightPos;
// Texture coordinates
in vec2 TextureCoordinates;

//// Color of the light source
//uniform vec3 lightColor;
// Material properties of the object
uniform Material material;

// Fragment shader's only required output, a vector of size 4
out vec4 FragColor;

/*
    Functions to calculate each type of light source
*/
vec3 calculateAmbience(vec3 lightAmbience);
vec3 calculateDiffuse(vec3 lightDiffuse, vec3 lightDirection, vec3 normal);
vec3 calculateSpecular(vec3 lightSpecular, vec3 lightDirection, vec3 normal, vec3 viewDirection);
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDirection, vec3 lightPos, vec3 fragPos, bool enabled);

void main() {
    // Create a vector of the light ray
    // A vector from FragPos to LightPos
    vec3 lightDirection = normalize(vec3(LightPos - FragPos));
    vec3 normalVec = normalize(Normal);
    // In view space, we compute relative to the viewer (set at 0,0,0)
    vec3 viewPos = vec3(0,0,0);
    vec3 viewDirection = normalize(viewPos - FragPos);

    // Calculate the flashlight's area and intensity

    //// Get the cos(theta) between the lightDir and the light's direction
    //// We negate the light's direction since it is pointing *from* the light source
    //// We need the vector from the target *to* the light source
    //float cos_theta = dot(lightDirection, normalize(-light.direction));
    //float cos_epsilon = light.cos_inner_cutoff - light.cos_outer_cutoff;
    //float intensity = clamp((cos_theta - light.cos_outer_cutoff) / cos_epsilon, 0.0, 1.0);
    
    //// The final light effect is the addition of diffuse and ambience effect
    //// The final color is obtained by multiplying the object's color and the final light effect
    //vec3 lightEffect = diffuse + ambience + specular;
    vec3 lightEffect = vec3(0.0);
    //// Apply the flashlight's intensity effect
    //lightEffect *= intensity;
    
    // Add directional light
    lightEffect = calculateDirectionalLight(directionalLight, normalVec, viewDirection);
    // Add point lights
    for (int i=0; i<NUM_POINT_LIGHTS; i++) {
        lightEffect += calculatePointLight(pointLight, normalVec, viewDirection, PointLightPos[i], FragPos, pointLightEnabled[i]);
    }

    FragColor = vec4(lightEffect, 1.0f);
}

vec3 calculateAmbience(vec3 lightAmbience) {
    vec3 ambience = lightAmbience * vec3(texture(material.diffuse, TextureCoordinates));
    return ambience;
}

vec3 calculateDiffuse(vec3 lightDiffuse, vec3 lightDirection, vec3 normal) {
    // Get the angle between the light ray and the normal vector
    // The angle determines the brightness of the fragment
    // If the angle is closer to 0, the light ray and the normal is more closely aligned
    // causing a brighter affect. When the angle is closer to 90 degrees, the effect is no light
    // The product of two *unit* vectors will give us cos(theta) 
    // We take the max to avoid negative dot product (occurs when the angle > 90)
    float diffuseStrength = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = lightDiffuse * diffuseStrength * vec3(texture(material.diffuse, TextureCoordinates));
    return diffuse;
}

vec3 calculateSpecular(vec3 lightSpecular, vec3 lightDirection, vec3 normal, vec3 viewDirection) {
    // -1 since the lightDirection is currently fragPos to light, and we want the opposite direction
    // The reflect() function expects direction light-> fragPos
    vec3 reflectionDirection = reflect(-lightDirection, normal); 
    // How much the light is properly reflected versus scattered around
    // Higher values, a smaller area will get intense light (highlights)
    // Lower values, light is spread out across the fragment
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    vec3 texture_color = vec3(texture(material.specular, TextureCoordinates));
    vec3 specular = lightSpecular * texture_color * spec;
    return specular;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection) {
    if (!light.enabled) {
        return vec3(0.0);
    }
    // Directional light, light position is irrelevant
    // We negate the light direction since it was set to the vector
    //  pointing away from the light source
    // In our calculation we want the vector pointing toward our light source
    vec3 lightDirection = normalize(-light.direction);

    vec3 ambience = calculateAmbience(light.ambient);
    vec3 diffuse = calculateDiffuse(light.diffuse, lightDirection, normal);
    vec3 specular = calculateSpecular(light.specular, lightDirection, normal, viewDirection);
    return ambience + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDirection, vec3 lightPos, vec3 fragPos, bool enabled) {
    if (!enabled) {
        return vec3(0.0);
    }
    
    vec3 lightDirection = normalize(vec3(lightPos - fragPos));
    
    vec3 ambience = calculateAmbience(light.ambient);
    vec3 diffuse = calculateDiffuse(light.diffuse, lightDirection, normal);
    vec3 specular = calculateSpecular(light.specular, lightDirection, normal, viewDirection);

    // Attenuation
    // Apply attenuation to each light component
    // Distance from the point light and the fragment
    float dist = length(LightPos - FragPos);
    float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    
    ambience *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambience + diffuse + specular;
}
