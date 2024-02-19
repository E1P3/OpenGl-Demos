#include "model.h"

unsigned int Model::getID() const {
    return ID;
}

void Model::setID(unsigned int ID) {
    this->ID = ID;
}
    
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes std::vector.
	void Model::loadModel(std::string const& path)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		// retrieve the directory path of the filepath
		directory = path.substr(0, path.find_last_of('/'));

		printf("  %i materials\n", scene->mNumMaterials);
		printf("  %i meshes\n", scene->mNumMeshes);
		printf("  %i animations\n", scene->mNumAnimations);

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			printf("  %i vertices in mesh\n", mesh->mNumVertices);
			meshes.push_back(processMesh(mesh, scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	void Model::Draw(Shader* shader, bool useOwnTextures, bool drawTessalated) 
	{
		if(rootBone){
			std::vector<glm::mat4> boneTransforms = getBoneMatrices(rootBone);
			shader->SetInteger("hasBones", 1);
			shader->SetInteger("numBones", boneTransforms.size());
			for (unsigned int i = 0; i < boneTransforms.size(); i++){
				std::string uniformName = "boneTransforms[" + std::to_string(i) + "]";
				shader->SetMatrix4(uniformName.c_str(), boneTransforms[i]);
			}
		}

		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i]->Draw(shader, useOwnTextures, drawTessalated);
	}

	void Model::SetVertexBoneDataToDefault(Vertex& vertex)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			vertex.m_BoneIDs[i] = -1;
			vertex.m_Weights[i] = 0.0f;
		}
	}

	Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture*> textures;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			SetVertexBoneDataToDefault(vertex);
			glm::vec3 vector; // we declare a placeholder std::vector since assimp uses its own std::vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}

			if (mesh->HasTangentsAndBitangents()) {
				// tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;
				// bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			//std::cout<< " Position: " << vertex.Position.x << " , " << vertex.Position.y << " , " << vertex.Position.z << "\n";

			vertices.push_back(vertex);
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		this->hasDiffuseMap = loadTextureMaps(material, aiTextureType_DIFFUSE, DIFFUSE, textures);
		this->hasNormalMap = loadTextureMaps(material, aiTextureType_HEIGHT, HEIGHT, textures);
		this->hasSpecularMap = loadTextureMaps(material, aiTextureType_SPECULAR, SPECULAR, textures);
		this->hasHeightMap = loadTextureMaps(material, aiTextureType_AMBIENT, NORMAL, textures);

		ExtractBoneWeightForVertices(vertices, mesh, scene);

		return ResourceManager::loadMesh(vertices, indices, textures);
	}
	
	bool Model::loadTextureMaps(aiMaterial* material, aiTextureType type, TextureType typeName, std::vector<Texture*>& textures){
		std::vector<Texture*> textureMaps = loadMaterialTextures(material, type, typeName);
		if(textureMaps.size() > 0){
			textures.insert(textures.end(), textureMaps.begin(), textureMaps.end());
			return true;
		} else {
			return false;
		}
	}

	void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			if (vertex.m_BoneIDs[i] < 0)
			{
				vertex.m_Weights[i] = weight;
				vertex.m_BoneIDs[i] = boneID;
				break;
			}
		}
	}

	void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
	{
		auto& boneInfoMap = m_BoneInfoMap;
		int& boneCount = m_BoneCounter;

		for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			int boneID = -1;
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				BoneInfo newBoneInfo;
				newBoneInfo.id = boneCount;
				newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
				boneInfoMap[boneName] = newBoneInfo;
				boneID = boneCount;
				boneCount++;
			}
			else
			{
				boneID = boneInfoMap[boneName].id;
			}

			assert(boneID != -1);
			auto weights = mesh->mBones[boneIndex]->mWeights;
			int numWeights = mesh->mBones[boneIndex]->mNumWeights;

			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
			{
				int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;
				assert(vertexId <= vertices.size());
				SetVertexBoneData(vertices[vertexId], boneID, weight);
			}
		}

		rootBone = processSkeleton(scene->mRootNode, boneInfoMap);

		printf("  %i bones\n", boneCount);
	}

    std::vector<Texture*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName)
	{
		std::vector<Texture*> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string fullPath = std::string(ASSET_DIR) + "/textures/" + str.C_Str();
            Texture* texture = ResourceManager::loadTexture(typeName, fullPath.c_str());
            textures.push_back(texture);
        
		}
		return textures;
	}

	Bone* Model::processSkeleton(const aiNode* node, const std::map<std::string, BoneInfo>& boneInfoMap){
		Bone* bone = nullptr;
		for (int i = 0; i < node->mNumChildren; ++i){
			Bone* childBone = processSkeleton(node->mChildren[i], boneInfoMap);
			if (childBone){
				if (!bone){
					bone = childBone;
				} else {
					bone->addChild(childBone);
				}
			}
		}
		auto boneInfo = boneInfoMap.find(node->mName.C_Str());
		if (boneInfo != boneInfoMap.end()){
			bone = new Bone(node, boneInfo->second.offset, boneInfo->second.id);
			for (int i = 0; i < node->mNumChildren; ++i){
				Bone* childBone = processSkeleton(node->mChildren[i], boneInfoMap);
				if (childBone){
					bone->addChild(childBone);
				}
			}
		}
		return bone;
	}

	std::vector<glm::mat4> Model::getBoneMatrices(Bone* rootBone){
		std::vector<glm::mat4> boneMatrices = std::vector<glm::mat4>(m_BoneCounter, glm::mat4(1.0f));
		if (rootBone){
			getBoneTransfrom(rootBone, boneMatrices);
		}
		return boneMatrices;
	}

	void Model::getBoneTransfrom(Bone* bone, std::vector<glm::mat4>& transforms){
		int id = bone->getID();
		transforms[id] = bone->getTransform() * bone->getOffset();
		for (auto child : bone->getChildren()){
			getBoneTransfrom(child, transforms);
		}
	}

	Bone* Model::findBone(const std::string& name, Bone* bone){
		if (bone){
			if (bone->getName() == name){
				return bone;
			}
			for (auto child : bone->getChildren()){
				Bone* childBone = findBone(name, child);
				if (childBone){
					return childBone;
				}
			}
		}
		return nullptr;

	}