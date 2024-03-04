#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "utils/assimpHelper.h"
#include "utils/animData.h"
#include "resourceManager.h"
#include "bone.h"

class Model
{
public:
	Model(std::string const& path, bool gamma = false) : gammaCorrection(gamma)
    {
		int indentation = path.find_last_of('/') + 1;
		this->name = path.substr(indentation, path.find_last_of('.') - indentation);
		loadModel(path);
	}

    unsigned int getID() const;

    void setID(unsigned int ID);
    
	int& getBoneCount() { return m_BoneCounter; }
	Bone* getRootBone() { return rootBone; }
	Bone* findBone(const std::string& name, Bone* bone = nullptr);
	void setRootBone(Bone* bone) { rootBone = bone; }
	std::vector<Vertex> getVertices();
	std::vector<Mesh*> getMeshes() { return meshes; }

	void Draw(Shader* shader, bool useOwnTextures = true, bool drawTessalated = false);

private:
    unsigned int ID;
	std::string name;
    std::vector<Texture*> textures;
	std::vector<Mesh*>    meshes;
	Bone* rootBone = nullptr;
	bool hasDiffuseMap = false;
	bool hasSpecularMap = false;
	bool hasNormalMap = false;
	bool hasHeightMap = false;

	std::string directory;

	bool gammaCorrection;

	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_BoneCounter = 0;

	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes std::vector.
	void loadModel(std::string const& path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);

	void SetVertexBoneDataToDefault(Vertex& vertex);

	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

	Bone* processSkeleton(const aiNode* node, const std::map<std::string, BoneInfo>& boneInfoMap);

	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);

	bool loadTextureMaps(aiMaterial* material, aiTextureType type, TextureType typeName, std::vector<Texture*>& textures);

	std::vector<glm::mat4> getBoneMatrices(Bone* rootBone);

	void getBoneTransfrom(Bone* bone, std::vector<glm::mat4>& transforms);

	aiString removePathFromName(aiString name);

};



#endif