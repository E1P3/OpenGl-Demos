#ifndef MESH_MANIPULATOR_MODULE_H
#define MESH_MANIPULATOR_MODULE_H

#include "../entityModule.h"
#include "../resourceManager.h"

class MeshManipulatorModule : public EntityModule {
public:

    MeshManipulatorModule(Mesh* mesh) {
        this->mesh = mesh;
    }

    void OnStart() override {

    }

    void OnUpdate() override {
        if(checkPreviousPositions()) {
            manipulateVerticesWithRBF();
        }
    }

    void addControlPoint(GameObject* controlPoint) {
        controlPoints.push_back(controlPoint);
        glm::vec3 intitialPosition = controlPoint->getWorldPosition();
        previousPositions.push_back(intitialPosition);
        initialPositions.push_back(intitialPosition);
        weights.push_back(getWeights(intitialPosition));
        manipulateVerticesWithRBF();
    }

    void setSigma(float sigma) {
        this->sigma = sigma;
    }

    void setC(float c) {
        this->c = c;
    }    

    std::map<int, float> getWeights(glm::vec3 controlPointPosition){
        std::map<int, float> weights;
        std::vector<glm::vec3> initialPositions = mesh->getInitialPositions();
        for (int i = 0; i < initialPositions.size(); i++){
            float distance = glm::distance(initialPositions[i], controlPointPosition);
            float weight = computeWeight(distance);
            if(weight > 0.0f)
                weights[i] = weight;
        }
        return weights;
    }

    bool checkPreviousPositions() {
        bool hasMoved = false;
        for (int i = 0; i < controlPoints.size(); i++) {
            if (controlPoints[i]->getWorldPosition() != previousPositions[i]) {
                previousPositions[i] = controlPoints[i]->getWorldPosition();
                hasMoved = true;
            }
        }
        return hasMoved;
    }

    void OnGui() {
        ImGui::Begin("Mesh Manipulator Module");
        ImGui::Text("Control Points");

        static float prevSigma = sigma;
        static float prevC = c;
        static float prevRadius = radius;
        static float prevInfluence = influence;

        ImGui::SliderFloat("Sigma", &sigma, 0.1f, 10.0f);
        ImGui::SliderFloat("C", &c, 0.1f, 10.0f);
        ImGui::SliderFloat("Radius", &radius, 0.1f, 10.0f);
        ImGui::SliderFloat("Influence", &influence, 0.0f, 1.0f);

        if (prevSigma != sigma || prevC != c || prevRadius != radius || prevInfluence != influence) {
            if(prevRadius != radius){
                weights.clear();
                for (int i = 0; i < controlPoints.size(); i++) {
                    weights.push_back(getWeights(controlPoints[i]->getWorldPosition()));
                }
            }
            manipulateVerticesWithRBF();
        }

        prevSigma = sigma;
        prevC = c;
        prevRadius = radius;
        prevInfluence = influence;

        ImGui::End();
    }


private:
    Mesh* mesh;
    std::vector<GameObject*> controlPoints;
    std::vector<glm::vec3> previousPositions;
    std::vector<glm::vec3> initialPositions;
    std::vector<std::map<int, float>> weights;
    float sigma = 3.0f;
    float c = 3.0f;
    float radius = 10.0f;
    float influence = 0.5f;

    void manipulateVerticesWithRBF() {
        std::vector<Vertex>& vertices = mesh->getVertices();
        std::vector<glm::vec3> vertexInitialPositions = mesh->getInitialPositions();
        for(int i = 0; i < vertexInitialPositions.size(); i++){
            glm::vec3 vertexPosition = vertexInitialPositions[i];
            glm::vec3 offset = calculateOffset(i, vertexPosition);
            vertices[i].Position = vertexPosition + offset * influence;
        }
        mesh->updateVertexBuffer();
    }

    glm::vec3 calculateOffset(int vertexIndex, glm::vec3 vertexPosition){
        glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);
        float totalWeight = 0.0f;
        for(int i = 0; i < controlPoints.size(); i++){
            if(weights[i].find(vertexIndex) != weights[i].end()){
                glm::vec3 cpOffset = controlPoints[i]->getWorldPosition() - initialPositions[i];
                float weight = weights[i][vertexIndex];
                offset += cpOffset * weight;
                totalWeight += weight;
            }
        }

        // if(totalWeight > 0.0f){
        //     offset /= totalWeight;
        // }

        return offset;
    }

    float computeWeight(float distance){
        return std::max(0.0f, 1.0f - distance / radius);
    }

    float computeGaussianWeight(float distance, float sigma) {
        return std::exp(-0.5f * (distance * distance) / (sigma * sigma));
    }

    float computeMultiquadricWeight(float distance, float c) {
        return std::sqrt(distance * distance + c * c);
    }

};

#endif // MESH_MANIPULATOR_MODULE_H