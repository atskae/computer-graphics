#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream> // ifstream
#include <iostream> // std::cout
#include <sstream> // stringstream

// Include all OpenGL headers
#include <glad/glad.h>

#include <glm/glm.hpp>


class Shader {
    private:
        // The unique ID of the main shader program, with the vertex and fragment
        //  shader linked together and compiled
        unsigned int programId;

        // For debugging
        std::string programName;
    
    public:
        // Constructor
        Shader(std::string programName, const char* vertexShaderPath, const char* fragmentShaderPath);

        // Use/activate the shader
        void use();

        // Getters
        unsigned int getProgramId() const { return this->programId; }
        bool getBool(const std::string& name) const;
        float getFloat(const std::string& name) const;
        int getInt(const std::string& name) const;
        // Set the returned matrix
        glm::mat4 getMatrix(const std::string& name) const;
        
        // Setters for uniform variables (global variables in the shader program)
        // Templating these wouldn't help much since the OpenGL functions
        // are not templated
        void setBool(const std::string& name, bool value);
        void setFloat(const std::string& name, float value);
        void setInt(const std::string& name, int value);
        void setVec3(const std::string& name, glm::vec3 vec);
        void setMatrix(const std::string& name, glm::mat4& matrix);
};


Shader::Shader(std::string programName, const char* vertexShaderPath, const char* fragmentShaderPath): programId(0) {

    // Save the name for debugging purposes
    this->programName = programName;

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

    std::cout << "Created shader program " << this->programName << " with ID: " << this->programId << std::endl;
}

// Activate the shader program
void Shader::use() { 
    glUseProgram(this->programId);
    //std::cout << "Using " << this->programName << " with ID: " << this->programId << std::endl;
}

/* Getters 
    Setting uniforms can also fail if the uniform is not used in the shader, even if
    the uniform is declared at the top.
*/

bool Shader::getBool(const std::string& name) const {
    // Booleans are integers in OpenGL (because it is written in C)
    return (bool) this->getInt(name);
}

float Shader::getFloat(const std::string& name) const {
    GLint location = glGetUniformLocation(this->programId, name.c_str());
    float value = -1.0f;
    if (location < 0) {
        std::cerr << "Invalid or unused uniform variable: " << name << std::endl;
    } else {
        glGetUniformfv(this->programId, location, &value);
    }
    return value;
}

int Shader::getInt(const std::string& name) const {
    GLint location = glGetUniformLocation(this->programId, name.c_str());
    int value = -1;
    if (location < 0) {
        std::cerr << "Invalid or unused uniform variable: " << name << std::endl;
    } else {
        glGetUniformiv(this->programId, location, &value);
    }
    return value;
}

glm::mat4 Shader::getMatrix(const std::string& name) const {
    GLint location = glGetUniformLocation(this->programId, name.c_str());
    GLfloat value[16] = {-1.0f};
    if (location < 0) {
        std::cerr << "Invalid uniform variable for program " << this->programId << ": " << name << std::endl;
    } else {
        glGetUniformfv(this->programId, location, value);
    }
    return glm::make_mat4(value);
}

/* Setters */

void Shader::setBool(const std::string& name, bool value) {
    // Booleans are integers in OpenGL (because it is written in C)
    this->setInt(name, (int)value);
}

void Shader::setFloat(const std::string& name, float value) {
    GLint location = glGetUniformLocation(this->programId, name.c_str());
    if (location < 0) {
        std::cerr << "Invalid or unused uniform variable: " << name << std::endl;
    } else {
        glUniform1f(location, value);
    }
}

void Shader::setInt(const std::string& name, int value) {
    GLint location = glGetUniformLocation(this->programId, name.c_str());
    if (location < 0) {
        std::cerr << "Invalid or unused uniform variable: " << name << std::endl;
    } else {
        glUniform1i(location, value);
    }
}

void Shader::setVec3(const std::string& name, glm::vec3 vec) {
    GLint location = glGetUniformLocation(this->programId, name.c_str());
    if (location < 0) {
        std::cerr << "Invalid or unused uniform variable vec3 for program " << this->programId << ": " << name << std::endl;
    } else {
        glUniform3fv(location, 1, glm::value_ptr(vec));
    }
}

void Shader::setMatrix(const std::string& name, glm::mat4& matrix) {
    GLint location = glGetUniformLocation(this->programId, name.c_str());
    if (location < 0) {
        std::cerr << "Invalid or unused uniform variable for program " << this->programId << ": " << name << std::endl;
    } else {
        // glUniformMatrix4fv(location, count, transpose, matrix)
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
    }
}

#endif // header guard
// You know, I can't believe it's 2022 and the C++ compiler still is not
// not smart enough to *not* include a header file twice without
// a header guard. I wonder how hard this problem is. Or since the header
// guard is everywhere in legacy code, it's hard to remove? /end rant
