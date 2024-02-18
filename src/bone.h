#ifndef BONE_H
#define BONE_H

#include "entity.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../src/utils/assimpHelper.h"
#include <assimp/scene.h>

class Bone : public Entity {

public:
    Bone(std::string name, glm::mat4 offset, int ID) {
        this->ID = ID;
        this->name = name;
        this->offset = offset;
    }

    Bone(const aiNode* node, glm::mat4 offset, int ID = -1) {
        this->ID = ID;
        this->name = node->mName.C_Str();
        this->offset = offset;
        glm::mat4 transform = AssimpGLMHelpers::ConvertMatrixToGLMFormat(node->mTransformation);
        this->setPosition(glm::vec3(transform[3]));
        this->setRotation(glm::quat_cast(transform));
        this->setScale(glm::vec3(1.0f));
    }

    ~Bone() {
        for (auto child : children) {
            delete child;
        }
    }

    void addChild(Bone* child) {
        children.push_back(child);
        this->addChildEntity(child);
    }

    std::vector<Bone*> getChildren() {
        return children;
    }

    int getID() {
        return ID;
    }

    std::string getName() {
        return name;
    }

    glm::mat4 getOffset() {
        return offset;
    }

    void setOffset(glm::mat4 offset) {
        this->offset = offset;
    }

    void OnGui() {
        if (ImGui::Button(this->getName().c_str())) {
           this->showTransfromGui = !this->showTransfromGui;
        }

        // If the bone's entity transform UI is visible, display it
        if (this->showTransfromGui) {
            Entity::OnGui();
        }

        // Recursively display children bones
        for (Bone* child : children) {
            if (ImGui::TreeNode(child->getName().c_str())) {
                child->OnGui();
                ImGui::TreePop();
            }
        }
    }

private:
    int ID;
    bool showTransfromGui = false;
    std::string name = "NO_NAME";
    glm::mat4 offset;
    std::vector<Bone*> children;
};

#endif // BONE_H