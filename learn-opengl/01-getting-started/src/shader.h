#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream> // ifstream
#include <iostream> // std::cout
#include <sstream> // stringstream

// Include all OpenGL headers
#include <glad/glad.h>


class Shader {
    private:
        // The unique ID of the main shader program, with the vertex and fragment
        //  shader linked together and compiled
        unsigned int programId;
    
    public:
        // Constructor
        Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

        // Use/activate the shader
        void use();

        // Getters
        unsigned int getProgramId() const { return this->programId; }

        // Setters for uniform variables (global variables in the shader program)
        // Templating these wouldn't help much since the OpenGL functions
        // are not templated
        void setBool(const std::string& name, bool value) const;
        void setFloat(const std::string& name, float value) const;
        void setInt(const std::string& name, int value) const;
};


Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath): programId(0) {

    // Place paths in a list so we can apply the same compilation steps
    // for each shader program
    const char* shaderPaths[] = {vertexShaderPath, fragmentShaderPath};

    GLenum shaderTypes[2] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

    // Unique IDs of the compiled shader programs
    unsigned int shaderIds[2] = {0};

    // Catch compilation and linking return code and error message
    int success;
    char infoLog[512];

    for (int i=0; i<2; i++) {
        // Shader source code as a C++ string
        std::string sourceCodeString;
        // Allows us to open the shader source file
        std::ifstream shaderFile;
        // Enable exceptions on the ifstream... (why isn't this default behavior?)
        shaderFile.exceptions(std::fstream::failbit | std::fstream::badbit);

        // Try to read in the file
        const char* shaderPath = shaderPaths[i];
        try {
            // Open the file
            std::cout << "Opening shader file: " << shaderPath << std::endl;
            shaderFile.open(shaderPath);

            // Create a buffer to hold in the file contents
            std::stringstream shaderStream;
            // Read in the file contents
            shaderStream << shaderFile.rdbuf();
            // Close the file handler
            shaderFile.close();

            // Convert the stream buffer contents into a string
            sourceCodeString = shaderStream.str();
        } catch (std::ifstream::failure err) {
            std::cerr << "Failed to read in shader file " << shaderPath << ": " << err.what() << std::endl;
            continue;
        }

        // Convert the C++ string to a const char* for compilation
        const char* sourceCode = sourceCodeString.c_str();
        
        // Create an empty shader object and the unique ID assigned to the shader
        unsigned int shaderId = glCreateShader(shaderTypes[i]);
        if (shaderId == 0) {
            std::cerr << "Failed to create a shader object" << std::endl;
            continue;
        }

        // Set the shader object's source code to the file contents
        glShaderSource(shaderId, 1, &sourceCode, NULL);

        // Compile the shader program!
        glCompileShader(shaderId);

        // Print compile errors, if any
        // Read in the error code and message
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            std::cerr << "Failed to compile shader program " << shaderPath << ": " << infoLog << std::endl;
            continue;
        }

        // Save the shader's Id for linking later
        shaderIds[i] = shaderId;
    }

    // Link the vertex shader and fragment shader together to create on main shader program

    // Check if any compilation has failed
    if (shaderIds[0] == 0 or shaderIds[1] == 0) {
        std::cerr << "A shader program failed compilation. Cannot link main shader program." << std::endl;
        return;
    }

    // Create a shader program object and a unique ID assigned to it
    this->programId = glCreateProgram();
    if (this->programId == 0) {
        std::cerr << "Failed to create a main shader program object." << std::endl;
        return;
    }

    // Attach the vertex and fragment shader to the main shader program
    glAttachShader(this->programId, shaderIds[0]);
    glAttachShader(this->programId, shaderIds[1]);
    glLinkProgram(this->programId);

    // Print linking errors, if any
    glGetProgramiv(this->programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->programId, 512, NULL, infoLog);
        std::cerr << "Failed to link shader program: " << infoLog << std::endl;
        return;
    }

    // We can delete the vertex and fragment shader now since they are linked
    // into the main shader program
    glDeleteShader(shaderIds[0]);
    glDeleteShader(shaderIds[1]);
}

// Activate the shader program
void Shader::use() { 
    glUseProgram(this->programId);
}

void Shader::setBool(const std::string& name, bool value) const {
    // Booleans are integers in OpenGL (because it is written in C)
    this->setInt(name, (int)value);
}

void Shader::setFloat(const std::string& name, float value) const {
    GLint location = glGetUniformLocation(this->programId, name.c_str());
    glUniform1f(location, value);
}

void Shader::setInt(const std::string& name, int value) const {
    GLint location = glGetUniformLocation(this->programId, name.c_str());
    glUniform1i(location, value);
}

#endif // header guard
// You know, I can't believe it's 2022 and the C++ compiler still is not
// not smart enough to *not* include a header file twice without
// a header guard. I wonder how hard this problem is. Or since the header
// guard is everywhere in legacy code, it's hard to remove? /end rant