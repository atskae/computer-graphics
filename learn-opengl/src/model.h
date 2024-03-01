#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <filesystem>
#include <unordered_map>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "stb_image.h"

#include "mesh.h"
#include "shader.h"


class Model {
    public:
        // Takes in a path to the model file
        Model(const char* path);
        // Draws each Mesh object
        void draw(Shader& shader);

    private:
        // Model file loaded as Mesh objects
        std::vector<Mesh> meshes;
        // Directory of the path that contains textures
        std::string directory;
        // Textures that were already loaded
        std::unordered_map<std::string, Texture> loadedTextures;

        // Methods
        void loadModel(std::string path);

        // Assimp
        void processNode(aiNode* node, const aiScene* scene);
        
        // Loads all textures from the Assimp Material object
        std::vector<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName);
        
        // Converts an Assimpl mesh to a Mesh object
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};

Model::Model(const char* path) {
    this->loadModel(path);
    std::cout << "Model " << path << " was created." << std::endl;
}

void Model::draw(Shader& shader) {
    for(unsigned int i=0; i<this->meshes.size(); i++) {
        this->meshes[i].draw(shader);
    }
}

// Loads a model file from path
void Model::loadModel(std::string path) {
    Assimp::Importer importer;
    // ReadFile() takes in a path and post-processing options
    // aiProcess_Triangulate: convert all primitives into triangles, if not already
    // aiProcess_FlipUVs: converts the texture coordinates so that the top-left corner is (0,0) and bottom-right corner is (1,1)
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    // AI_SCENE_FLAGS_INCOMPLETE: if the data that was loaded is incomplete
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Assimp failed to load scene from file " << path << " with error: " << importer.GetErrorString() << std::endl;
        return;
    }

    // std::string.find_last_of(): finds the last character that matches and returns its index
    this->directory = path.substr(0, path.find_last_of('/'));

    // Generate and collect Mesh objects
    this->processNode(scene->mRootNode, scene);
}

// Recursive function that traverses the Assimp Scene graph and generates Mesh objects
void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process the meshes of this node
    std::cout << node->mNumMeshes << " meshes found" << std::endl;
    for (unsigned int i=0; i<node->mNumMeshes; i++) {
        // The index into the Scene (root) node's mesh list where this node's mesh is located
        unsigned int index = node->mMeshes[i];
        // Convert the Assimp Mesh object to our Mesh object
        Mesh mesh = this->processMesh(scene->mMeshes[index], scene);
        this->meshes.push_back(mesh);
    }

    // Convert the meshes of this node's child nodes
    for (unsigned int i=0; i<node->mNumChildren; i++) {
        aiNode* child = node->mChildren[i];
        this->processNode(child, scene);
    }
}

// Converts an Assimp 3D vector to a glm 3D vector
glm::vec3 convert(aiVector3D vector) {
    return glm::vec3(vector[0], vector[1], vector[2]);
}

// Converts an Assimp 2D vector (ex. texture coordinates) to a glm  2D vector
glm::vec2 convert(aiVector2D vector) {
    return glm::vec2(vector[0], vector[1]);
}

// Loads a texture file and binds it to an OpenGL texture unit
// Returns the integer ID of the texture
unsigned int TextureFromFile(const char* filePath, std::string directory) {
    unsigned int id;
    // Generates 1 texture and assigns an ID to it
    glGenTextures(1, &id);
    // Activate the texture unit so subsequent texture calls affect this texture
    glBindTexture(GL_TEXTURE_2D, id);

    // Load the texture file
    // Texture image properties
    int width, height, numColorChannels;
    std::filesystem::path texturePath(directory);
    texturePath = texturePath / filePath;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &numColorChannels, 0);
    if (data) {
        // Bind the texture data to the texture unit
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        std::cout << "Loaded texture file: " << texturePath.string() << std::endl;
    } else {
        std::cout << "Failed to load texture file: " << texturePath.string() << std::endl;
    }
    // Free image data
    stbi_image_free(data);

    return id;
}

// Loads the textures from the material
std::vector<Texture> Model::loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for(unsigned int i=0; i<material->GetTextureCount(type); i++) {
        // Get the file path to the texture file
        aiString filePath;
        material->GetTexture(type, i, &filePath);

        std::string filePathString = std::string(filePath.C_Str());
        if (this->loadedTextures.find(filePathString) == this->loadedTextures.end()) {
            Texture texture;
            texture.id = TextureFromFile(filePath.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = filePath.C_Str();
            textures.push_back(texture);
            // Mark the texture as already loaded
            this->loadedTextures.insert({filePathString, texture});
            std::cout << "Created texture object " << texture.path << std::endl;
        } else {
            std::cout << filePathString << " was already loaded" << std::endl;
        }
    }
    return textures;
}

// Convert an Assimp Mesh to our Mesh object definition
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::cout << "Found " << mesh->mNumVertices << " vertices." << std::endl;

    // Create the Vertex objects
    std::vector<Vertex> vertices;
    for (unsigned int i=0; i<mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = convert(mesh->mVertices[i]);
        if (mesh->HasNormals()) {
            vertex.normal = convert(mesh->mNormals[i]);
        } else {
            std::cout << "No normal vector found" << std::endl;
            vertex.normal = glm::vec3(0);
        }
        
        // Assimp allows up to 8 texture coordinates per vertex
        // but for now we only look at the first one
        if (mesh->mTextureCoords[0]) {
            vertex.textureCoordinates = convert(mesh->mTextureCoords[0][i]);
        } else {
            vertex.textureCoordinates = glm::vec2(0);
        }
        
        vertices.push_back(vertex);
    }

    // Get indices into the vertex array
    // Each face represents a primitive
    // Since we loaded the model with `aiProcess_Triangulate`,
    // all the primitives are triangles
    std::vector<unsigned int> indices;
    for (unsigned int i=0; i<mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j=0; j<face.mNumIndices; j++) {
            unsigned int index = face.mIndices[j];
            indices.push_back(index);
        }
    }

    std::vector<Texture> textures;
    // The mesh only contains the index into the materials array
    // which is held by the scene object
    if(mesh->mMaterialIndex >=0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        std::vector<Texture> diffuseMaps = this->loadMaterialTexture(material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = this->loadMaterialTexture(material, aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

#endif // header guard
