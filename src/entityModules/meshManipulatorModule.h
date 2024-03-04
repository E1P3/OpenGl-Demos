#ifndef MESH_MANIPULATOR_MODULE_H
#define MESH_MANIPULATOR_MODULE_H

#include "../entityModule.h"
#include "../resourceManager.h"

class MeshManipulatorModule : public EntityModule {
public:

    MeshManipulatorModule(Mesh* mesh){
        this->setAssignedVertices(mesh);
        this->assignedMesh = mesh;
    }

    ~MeshManipulatorModule() = default;

    void OnUpdate() override{

        if(this->getParent()->getPosition() != this->previousPosition){
            std::vector<glm::vec3> vertices = assignedMesh->getVertices();
            for(auto pair : vertexInfluenceIndex){
                int vertexIndex = pair.first;
                float influence = pair.second;
                glm::vec3 vertex = assignedVertices[vertexIndex];
                glm::vec3 newPosition = vertex + (this->getParent()->getWorldPosition() - vertex) * influence;
                vertices[vertexIndex] = newPosition;
            }
            assignedMesh->updateMeshVerticies(vertices);
            this->previousPosition = this->getParent()->getPosition();
        }
    }

    void OnStart() override{
        this->vertexInfluenceIndex = calculateInfluence();
        this->previousPosition = this->getParent()->getPosition();
    }

    void setInfluenceRadius(float influenceRadius){
        this->influenceRadius = influenceRadius;
    }

    void OnGui() {
        ImGui::Begin("Mesh Manipulator Module");
        ImGui::SliderFloat("Influence Radius", &influenceRadius, 0.0f, 1.0f);
        ImGui::End();
    }

private:
    std::vector<glm::vec3> assignedVertices;
    std::map<int, float> vertexInfluenceIndex; //int corresponds to vertex indes, float to influence
    Mesh* assignedMesh;
    float influenceRadius = 3.0f;
    glm::vec3 previousPosition = glm::vec3(0, 0, 0);



    void setAssignedVertices(Mesh* mesh){
        this->assignedVertices = mesh->getVertices();
    }

    std::map<int, float> calculateInfluence(){
        std::map<int, float> influenceMap;
        for(int i = 0; i < assignedVertices.size(); i++){
            float influence = calculateGaussianInfluence(assignedVertices[i]);
            if(influence > 0.0f){
                influenceMap[i] = influence;
            }
        }
        return influenceMap;
    }

    float calculateGaussianInfluence(glm::vec3 vertexPosition){
        float distance = glm::distance(this->getParent()->getPosition(), vertexPosition);
        if(distance < influenceRadius){
            float normalizedDistance = distance / influenceRadius;
            float sigma = 0.5f; // Standard deviation
            float exponent = -0.5f * pow((normalizedDistance / sigma), 2);
            return exp(exponent);
        }
        
        return 0.0f;
    }

    float calculateInfluence(glm::vec3 vertexPosition){
        float distance = glm::distance(this->getParent()->getPosition(), vertexPosition);
        if(distance < influenceRadius){
            float normalizedDistance = distance / influenceRadius;
            return 1.0f - pow(normalizedDistance, 2);
        }
        
        return 0.0f;
    }

};

#endif // MESH_MANIPULATOR_MODULE_H