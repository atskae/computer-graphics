// OpenGL version 4.6 with core-profile functionality
#version 460 core 

// The `in` keyword specifies all the inputs (input vertex attributes) to this vertex shader
// The input to this shader program is a 3D vector `vec3`
// We name the input variable `aPos`
// `layout` allows us to define the location of the input variable
layout (location = 0) in vec3 aPos;
// Normal vector of the triangle surface
layout (location = 1) in vec3 aNormal;

// Transformation matrices
// Convert local to global
uniform mat4 model;
// Converts global to camera/view space
uniform mat4 view;
// Applies perspective projection
uniform mat4 projection;
// Position of the light source, in world-space
uniform vec3 lightPos;

// Gouraud lighting

uniform vec3 objectColor;
// Color of the light source
uniform vec3 lightColor;

// Variables to pass to the fragment shader
// Normal vector
out vec3 Normal;
// World-space coordinate of the fragment
// All the lighting is done in world-space
out vec3 FragPos;
// The position of the light source, in view space
out vec3 LightPos;

// The final color at this vertex
out vec4 fragmentColor;

void main() {
    // We set the return value to `gl_Position`
    // Here we are converting the 3D input coordinate to a 4D output coordinate
    // The fourth coordinate is used for `perspective division`
    
    // Apply the transformations
    // Read tranformations from right to left
    // So here, model is applied first, then view, then lastly projection
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    // Pass the normal vector to the fragment shader
    // To ensure that the normal vector stays at 90 degrees
    // to the surface, we the inverse + transpose to the model matrix
    // model = 4x4 matrix, we take the top-left inner 3x3 matrix of model
    // aNormal = 1x3 vector
    Normal = mat3(transpose(inverse(view * model))) * aNormal;

    // Compute the fragment's world coordinates only
    //FragPos = vec3(model * vec4(aPos, 1.0f));
    // Compute the fragment's view coordinates
    FragPos = vec3(view * model * vec4(aPos, 1.0f));

    // Convert the light source coordinates from world-space to view space
    LightPos = vec3(view * vec4(lightPos, 1.0f));

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
    fragmentColor = vec4(objectColor * lightEffect, 1.0f);
}
