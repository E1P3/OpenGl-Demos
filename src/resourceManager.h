#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "material.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "model.h"

class ResourceManager {
public:
    static Shader* loadShader(const char* vertexShaderFile, const char* fragmentShaderFile);
    static Shader* getShader(unsigned int ID);
    static Texture* loadTexture( TextureType type, const char* textureFile);
    static Texture* getTexture(unsigned int ID);
    static Mesh* loadMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures);
    static Mesh* getMesh(unsigned int ID);
    static Model* loadModel(const char* modelFile);
    static Model* getModel(unsigned int ID);

private:
    static std::vector<Shader*> shaders;
    static std::vector<Texture*> textures;
    static std::vector<Material*> materials;
    static std::vector<Mesh*> meshes;
    static std::vector<Model*> models;
};

#endif
