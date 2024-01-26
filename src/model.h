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
#include "mesh.h"
#include "texture.h"

class Model
{
public:
	Model(std::string const& path, bool gamma = false) : gammaCorrection(gamma)
    {
		loadModel(path);
	}

    unsigned int getID() const;

    void setID(unsigned int ID);
    
	auto& GetBoneInfoMap() { return m_BoneInfoMap; }
	int& GetBoneCount() { return m_BoneCounter; }


private:
    unsigned int ID;
    std::vector<Texture*> textures;
	std::vector<Mesh*>    meshes;

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

	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
};



#endif