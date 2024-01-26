#include "resourceManager.h"

std::vector<Shader*> ResourceManager::shaders;
std::vector<Texture*> ResourceManager::textures;
std::vector<Material*> ResourceManager::materials;
std::vector<Mesh*> ResourceManager::meshes;
std::vector<Model*> ResourceManager::models;

Model* ResourceManager::loadModel(const char* modelFile){
        Model* model = new Model(modelFile);
        model->setID(models.size());
        models.push_back(model);
        return model;
    }

Model* ResourceManager::getModel(unsigned int ID){
    return models[ID];
}

Shader* ResourceManager::loadShader(const char* vertexShaderFile, const char* fragmentShaderFile){
        Shader* shader = new Shader(vertexShaderFile, fragmentShaderFile);
        shaders.push_back(shader);
        return shader;
}
Shader* ResourceManager::getShader(unsigned int ID){
    return shaders[ID];
}
Texture* ResourceManager::loadTexture( TextureType type, const char* textureFile){
    for (Texture* texture : textures) {
        if (strcmp(texture->getFilePath().c_str(), textureFile) == 0) {
            return texture;
        }
    }
    Texture* texture = new Texture(type, textureFile);
    textures.push_back(texture);
    return texture;
}
Texture* ResourceManager::getTexture(unsigned int ID){
    return textures[ID];
}
Mesh* ResourceManager::loadMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures){
    Mesh* mesh = new Mesh(vertices, indices, textures);
    mesh->setID(meshes.size());
    meshes.push_back(mesh);
    return mesh;
}
Mesh* ResourceManager::getMesh(unsigned int ID){
    return meshes[ID];
}

