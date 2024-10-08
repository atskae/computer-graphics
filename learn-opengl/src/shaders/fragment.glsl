// Fragment shader is responsible for calculating the colors
#version 460 core

// Input passed in from the vertex shader
in vec3 fragmentShaderColor;
in vec2 texCoord1;
in vec2 texCoord2;

// Texture objects generated on the CPU-side
uniform sampler2D texture1; // box-crate thing
uniform sampler2D texture2; // awesomeface thing
uniform float mixPercentage; // lerp setting for the mix() texture function

// Fragment shader's only required output, a vector of size 4
out vec4 FragColor;

void main() {
    // Use color from the vertex shader
    //FragColor = fragmentShaderColor;
    //FragColor = vertexPosition;
    // Apply the texture with GLSL texture() function
    // Samples the color from the texture object
    // Mix the texture color with the color sent by the vertex shader
    //FragColor = texture(texture1, texCoord) * vec4(fragmentShaderColor, 1.0);

    // Linearly interpolates between two textures
    // 0.3 means 70% of the first texture, and 30% of the second texture
    FragColor = mix(texture(texture1, texCoord1), texture(texture2, texCoord2), mixPercentage);
}
