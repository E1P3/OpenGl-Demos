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
            updateVerticies();
        }
    }

    void addControlPoint(GameObject* controlPoint) {
        controlPoints.push_back(controlPoint);
        glm::vec3 intitialPosition = controlPoint->getWorldPosition();
        previousPositions.push_back(intitialPosition);
        initialPositions.push_back(intitialPosition);
        weights.push_back(getWeights(intitialPosition));
        updateVerticies();
    }

    void setSigma(float sigma) {
        this->sigma = sigma;
    }

    void setC(float c) {
        this->c = c;
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
        ImGui::Begin("Control Points");

        static float prevSigma = sigma;
        static float prevC = c;
        static float prevRadius = radius;
        static float prevInfluence = influence;
        static bool prevIsGaussian = isGaussian;

        ImGui::SliderFloat("Sigma", &sigma, 0.0f, 10.0f);
        ImGui::SliderFloat("C", &c, 0.1f, 10.0f);
        ImGui::SliderFloat("Radius", &radius, 0.1f, 10.0f);
        ImGui::SliderFloat("Influence", &influence, 0.0f, 1.0f);
        ImGui::Checkbox("Gaussian", &isGaussian);

        if (prevSigma != sigma || prevC != c || prevRadius != radius || prevInfluence != influence || prevIsGaussian != isGaussian) {
            if(prevRadius != radius || prevIsGaussian != isGaussian || prevSigma != sigma){
                factor = -0.5 / (sigma * sigma);
                updateControlPointWeights();
            }
            updateVerticies();
        }

        prevSigma = sigma;
        prevC = c;
        prevRadius = radius;
        prevInfluence = influence;
        prevIsGaussian = isGaussian;

        ImGui::End();
    }


private:
    Mesh* mesh;
    std::vector<GameObject*> controlPoints;
    std::vector<glm::vec3> previousPositions;
    std::vector<glm::vec3> initialPositions;
    std::vector<std::map<int, float>> weights;
    float sigma = 3.0f;
    float factor = -0.5 / 9.0f;
    float c = 3.0f;
    float radius = 10.0f;
    float influence = 0.5f;
    bool isGaussian = false;

    void updateVerticies() {
        std::vector<Vertex>& vertices = mesh->getVertices();
        std::vector<glm::vec3> vertexInitialPositions = mesh->getInitialPositions();
        for(int i = 0; i < vertexInitialPositions.size(); i++){
            glm::vec3 vertexPosition = vertexInitialPositions[i];
            glm::vec3 offset = calculateOffset(i, vertexPosition);
            if(offset != glm::vec3(0.0f, 0.0f, 0.0f)){
                vertices[i].Position = vertexPosition + offset * influence;
            }
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

        return offset;
    }

    void updateControlPointWeights() {
        weights.clear();
        for (int i = 0; i < controlPoints.size(); i++) {
            weights.push_back(getWeights(initialPositions[i]));
        }
    }

    std::map<int, float> getWeights(glm::vec3 controlPointPosition){
        std::map<int, float> weights;
        std::vector<glm::vec3> vertexPositions = mesh->getInitialPositions();
        for (int i = 0; i < vertexPositions.size(); i++){
            float distance = glm::distance(vertexPositions[i], controlPointPosition);
            float weight = computeWeight(distance);
            if(weight > 0.0f)
                weights[i] = weight;
        }
        return weights;
    }

    float computeWeight(float distance) {
        if(!isGaussian){
            return std::max(0.0f, 1.0f - distance / radius);
        }
        else{
            return computeGaussianWeight(distance, factor);
        }

    }

    float computeGaussianWeight(float distance, float factor) {
        return std::exp(distance * distance * factor);
    }

};

#endif // MESH_MANIPULATOR_MODULE_H