#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "shader.h"

// Locations (indicies) in the vertex shader for each type of vertex data
// `layout (location = index)` in GLSL
const unsigned int POSITION_VERTEX_ATTRIBUTE_LOCATION = 0;
const unsigned int NORMAL_VERTEX_ATTRIBUTE_LOCATION = 1;
const unsigned int TEXTURE_COORDINATE_VERTEX_ATTRIBUTE_LOCATION = 2;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates;
};

struct Texture {
    unsigned int id;
    std::string type; // diffuse, specular; used to find the uniform variable
    std::string path; // path of the texture file
};

class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // Constructor
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        // Methods
        // Renders the mesh
        // Uses the shader to set GLSL uniform variables, linking samplers to texture units, etc.
        void draw(Shader& shader);

    private:
        // Render data for OpenGL
        // Holds the unique id for:
        //  Vertex Array Buffer (VAO): holds references to all the VBOs
        //  Vertex Buffer Object (VBO): stores the actual vertex memory
        //  Element Buffer Object (EBO): holds indices
        unsigned int VAO, VBO, EBO;
        
        // Methods
        // Initialize the buffers
        void setupMesh();

}; // class Mesh

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    this->setupMesh();
}

void Mesh::setupMesh() {
    // Assign a unique id for each buffer

    // Vertex Attribute Object
    // Stores the state of glVertexAttribPointer() and related calls
    // Assigns a unique ID to the VAO object (created behind the scenes)
    glGenVertexArrays(1, &this->VAO);

    // Creates a Vertex Buffer Object behind the scenes, and assigns an ID to it
    glGenBuffers(1, &this->VBO);

    // Element buffer object
    // Create one buffer and assign unique ID
    glGenBuffers(1, &this->EBO);

    // Bind the VAO
    // After this bind call, any functions related to vertex buffer objects (VBO) will store
    //  its state inside this VAO
    glBindVertexArray(this->VAO);
                  
    // Specify that the newly created buffer object is specifically a vertex buffer object
    // This binds the GL_ARRAY_BUFFER to the one we created, VBO
    // Any operations on the GL_ARRAY_BUFFER will configure our VBO object
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    // Copy over the triangle vertices data to the VBO
    // The fourth argument specifies how the GPU should manage the data
    // GL_STATIC_DRAW is best for data that doesn't change much and is read many times
    // If the data changes a lot, we'd use GL_DYNAMIC_DRAW
    // We need to pass in the size *in bytes*
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertices.size(), &this->vertices, GL_STATIC_DRAW);

    // Bind the EBO 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    // Copy over the indicies data to the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indices.size(), &this->indices, GL_STATIC_DRAW);

    // Define how OpenGL should interpret the data and enable the buffers
    
    // Specify to OpenGL how to interpret the vertex data in the vertex shader
    // This is applied to the currently active VBO specified in BindBuffer()
    glVertexAttribPointer(
        // The value we specified in our vertex shader, `layout (location = 0)`
        // Where to find the memory location of the vec3 input to the vertex shader
        POSITION_VERTEX_ATTRIBUTE_LOCATION,
        // The size of the vertex shader's input (vec3 aPos)
        this->vertices[0].position.length(),
        // The type of the value in the input vec3
        GL_FLOAT,
        // Indicate whether the data should be normalized ([-1, 1] for signed values, [0, 1] for positive)
        GL_FALSE,
        // Stride, the space between vertex attributes, in bytes
        // If we specify 0, OpenGL tries to figure this out itself
        //  This only works if the data is tightly-packed (no padding between attributes)
        sizeof(Vertex),
        // Where the data starts in the buffer
        // Since the data starts at the beginning of the buffer, we use 0
        (void*) offsetof(Vertex, position) // gets the byte offset of the struct field
    );
    // Enable the vertex attribute in the vertex shader
    glEnableVertexAttribArray(POSITION_VERTEX_ATTRIBUTE_LOCATION);

    // Set the normal vectors to the vertex shader
    glVertexAttribPointer(
        NORMAL_VERTEX_ATTRIBUTE_LOCATION,
        this->vertices[0].normal.length(), // size of each data
        GL_FLOAT, // type of the input
        GL_FALSE, // normalize data,
        sizeof(Vertex), // stride of the data
        (void*) offsetof(Vertex, normal) // offset into the vertex data where this data starts
    );
    // Enable a vertex attribute array
    // This actively connects the vertex data to the attribute in the shader
    // The above call simply describes how to interpret the data, but not activate it
    glEnableVertexAttribArray(NORMAL_VERTEX_ATTRIBUTE_LOCATION);

    // Set the texture coordinates
    glVertexAttribPointer(
        TEXTURE_COORDINATE_VERTEX_ATTRIBUTE_LOCATION,
        this->vertices[0].textureCoordinates.length(), // size of each data
        GL_FLOAT, // type of the input
        GL_FALSE, // normalize data,
        sizeof(Vertex), // stride of the data
        (void*) offsetof(Vertex, textureCoordinates) // offset into the vertex data where this data starts
    );
    glEnableVertexAttribArray(TEXTURE_COORDINATE_VERTEX_ATTRIBUTE_LOCATION);

    // Unbind the bounded Vertex Array Object by passing in zero
    glBindVertexArray(0);
}

void Mesh::draw(Shader& shader) {
    // Bind all the texture units
    // Assumes the following naming convention in the vertex shader:
    //  uniform sampler2D texture_diffuse1;
    //  uniform sampler2D texture_diffuse2;
    //  ...etc
    //  uniform sampler2D texture_specular1;
    //  uniform sampler2D texture_specular2
    //  ...etc
    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;
    for (unsigned int i=0; i<this->textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        
        Texture& texture = this->textures[i];

        // Set the uniform variable that holds the texture
        int texture_unit = 0;
        if (texture.type == "diffuse") {
            texture_unit = numDiffuse++;
        }
        else if (texture.type == "specular") {
            texture_unit = numSpecular++;
        }
        else {
            std::cout << "Invalid texture type: " << texture.type << std::endl;
        }
        
        char buffer[100];
        int result = std::sprintf(buffer, "material.texture_%s%i", texture.type.c_str(), texture_unit);
        std::string texture_name = std::string(buffer, result);
        
        shader.setInt(texture_name, texture.id);
        
        glBindTexture(GL_TEXTURE_2D, texture.id); 
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw the mesh
    
    // Bind the VAO
    glBindVertexArray(this->VAO);

    // Draw the mesh by indices
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    
    // Unbind VAO
    glBindVertexArray(0);
}

#endif // header guard
