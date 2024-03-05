#ifndef BLEND_SHAPE_MODULE_H
#define BLEND_SHAPE_MODULE_H

#include "../entityModule.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "../mesh.h"
#include <imgui.h>

struct deltablendShape {
    float weight;
    std::vector<glm::vec3> deltas;
    std::string name;

};

class BlendShapeModule : public EntityModule {

public:

    BlendShapeModule(Mesh* targetMesh){
        this->targetMesh = targetMesh;
        initialPositions = targetMesh->getInitialPositions();
    }

    void OnUpdate() override {
        if(checkWeights()){
            updateMesh();
        }
    }

    void OnStart() override {

    }

    void OnGui(){
        ImGui::Begin("Blend Shapes");
        for(int i = 0; i < blendShapes.size(); i++){
            ImGui::PushID(i);
            ImGui::SliderFloat(blendShapes[i].name.c_str(), &blendShapes[i].weight, 0.0f, 1.0f);
            ImGui::PopID();
        }
        ImGui::End();
    }

    void addDeltaBlendShape(Mesh* target, std::string name){
        deltablendShape* bs = getBlendShapeFromMesh(target, name);
        if(bs != nullptr){
            blendShapes.push_back(*bs);
            previousWeights.push_back(0.0f);
        }
    }

private:
    Mesh* targetMesh;
    std::vector<deltablendShape> blendShapes;
    std::vector<float> previousWeights;
    std::vector<glm::vec3> initialPositions;

    bool checkWeights(){
        for(int i = 0; i < blendShapes.size(); i++){
            if(blendShapes[i].weight != previousWeights[i]){
                previousWeights[i] = blendShapes[i].weight;
                return true;
            }
        }
        return false;
    }

    void updateMesh(){
        std::vector<Vertex>& vertices = targetMesh->getVertices();
        for(int i = 0; i < vertices.size(); i++){
            glm::vec3 delta = glm::vec3(0.0f, 0.0f, 0.0f);
            for(int j = 0; j < blendShapes.size(); j++){
                if(blendShapes[j].weight > 0.0f && isNotZero(blendShapes[j].deltas[i])){
                    delta += blendShapes[j].deltas[i] * blendShapes[j].weight;
                }
            }
            vertices[i].Position = initialPositions[i] + delta;
        }
        targetMesh->updateVertexBuffer();
    }

    deltablendShape* getBlendShapeFromMesh(Mesh* blendShapeMesh, std::string name){
        deltablendShape* bs = new deltablendShape();
        bs->name = name;
        bs->weight = 0.0f;
        bs->deltas = std::vector<glm::vec3>();
        std::vector<glm::vec3> blendShapeMeshPositions = blendShapeMesh->getInitialPositions();
        if(initialPositions.size() != blendShapeMeshPositions.size()){
            std::cout << "Error: Meshes have different number of vertices" << std::endl;
            return nullptr;
        }
        bs->deltas.reserve(initialPositions.size());
        for(int i = 0; i < initialPositions.size(); i++){
            glm::vec3 delta = blendShapeMeshPositions[i] - initialPositions[i];
            bs->deltas.push_back(delta);
        }
        return bs;
    }

    bool isNotZero(const glm::vec3& vec) {
        return vec.x != 0.0f || vec.y != 0.0f || vec.z != 0.0f;
    }

};

#endif // BLEND_SHAPE_MODULE_H