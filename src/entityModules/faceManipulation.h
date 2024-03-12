#ifndef FACE_MANIPULATION_H
#define FACE_MANIPULATION_H

#include "../entityModule.h"
#include "../resourceManager.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>


struct manipulator{
    GameObject* controlPoint;
    glm::vec3 initialPosition;
    glm::vec3 currentPosition;
    glm::vec3 delta = glm::vec3(0.0f, 0.0f, 0.0f);
    int vertexIndex;

    manipulator(GameObject* controlPoint, int vertexIndex){
        this->controlPoint = controlPoint;
        this->initialPosition = controlPoint->getWorldPosition();
        this->currentPosition = initialPosition;
        this->vertexIndex = vertexIndex;
    }
};

struct blendShape{
    std::string name;
    std::vector<glm::vec3> verticies;
    std::vector<glm::vec3> deltas;
    float weight = 0.0f;

    blendShape(Mesh* mesh, Mesh* defaultMesh, std::string name){
        this->name = name;
        this->verticies = mesh->getInitialPositions();
        std::vector<glm::vec3> defaultVerticies = defaultMesh->getInitialPositions();
        for(int i = 0; i < verticies.size(); i++){
            deltas.push_back(verticies[i] - defaultVerticies[i]);
        }
    }
};

class FaceManipulation : public EntityModule {

public:
    FaceManipulation(Mesh* defaultMesh){
        this->defaultMesh = defaultMesh;
        this->initialPositions = defaultMesh->getInitialPositions();
    };

    void OnStart() override {
        initialiseEigenVariables();
    }

    void OnUpdate() override {
        if(checkForManipulatorMovement()){
            updateWeights();
        }

        if(manipulators.empty()){
            updateDefaultMesh();
        }
    }

    void OnGui() {

        float _alpha = alpha;
        float _u = u;

        ImGui::Begin("Face Manipulation");
        if(ImGui::Button("Reset Manipulators")){
            resetManipulators();
        }
        
        for(int i = 0; i < blendShapes.size(); i++){
            ImGui::SliderFloat(blendShapes[i].name.c_str(), &blendShapes[i].weight, 0.0f, 1.0f);
        }

        ImGui::SliderFloat("Alpha", &_alpha, 0.0f, 1.0f);
        ImGui::SliderFloat("U", &_u, 0.0f, 1.0f);
        ImGui::End();

        if(_alpha != alpha){
            alpha = _alpha;
            updateWeights();
        }

        if(_u != u){
            u = _u;
            updateWeights();
        }
    }

    void addBlendShape(Mesh* mesh, std::string name){
        blendShape newBlendShape(mesh, defaultMesh, name);
        blendShapes.push_back(newBlendShape);
    }

    void addManipulator(GameObject* controlPoint, int vertexIndex){
        manipulator newManipulator(controlPoint, vertexIndex);
        manipulators.push_back(newManipulator);
    }

    void initialiseEigenVariables(){
        if(blendShapes.empty() || blendShapes[0].verticies.empty()) {
            return;
        }

        unsigned int num_rows = manipulators.size();
        unsigned int num_cols = blendShapes.size();

        B = Eigen::MatrixXf(blendShapes[0].verticies.size() * 3, blendShapes.size());
        Bt = Eigen::MatrixXf(blendShapes.size(), blendShapes[0].verticies.size() * 3);
        BtB = Eigen::MatrixXf(blendShapes.size(), blendShapes.size());

        for(int i = 0; i < blendShapes.size(); i++){
            for(int j = 0; j < (blendShapes[i].verticies.size() * 3); j += 3){
               B(j, i) = blendShapes[i].deltas[j/3].x; 
               B(j + 1, i) = blendShapes[i].deltas[j/3].y; 
               B(j + 2, i) = blendShapes[i].deltas[j/3].z;
            }
        }

        Bt = B.transpose();
        BtB = Bt * B;
    }

private:
    Mesh* defaultMesh;
    std::vector<glm::vec3> initialPositions;
    float alpha = 0.1f;
    float u = 0.001f;
    std::vector<manipulator> manipulators;
    std::vector<blendShape> blendShapes;
    Eigen::MatrixXf B;
    Eigen::MatrixXf Bt;
    Eigen::MatrixXf BtB;

    bool checkForManipulatorMovement(){
        for(int i = 0; i < manipulators.size(); i++){
            glm::vec3 currentManipulatorPosition = manipulators[i].controlPoint->getWorldPosition();
            if(manipulators[i].currentPosition != currentManipulatorPosition){
                manipulators[i].delta = currentManipulatorPosition - initialPositions[manipulators[i].vertexIndex];
                manipulators[i].currentPosition = currentManipulatorPosition;
                return true;
            }
        }
        return false;
    }

    Eigen::VectorXf updateM(){
        Eigen::VectorXf m = Eigen::VectorXf(manipulators.size()* 3);
        glm::vec3 delta = glm::vec3(0.0f,0.0f,0.0f);
        for(int i = 0; i < manipulators.size(); i++){
            delta = manipulators[i].currentPosition - manipulators[i].initialPosition;
            m(i) = delta.x;
            m(i + 1) = delta.y;
            m(i + 2) = delta.z;
        }
        return m;
    }

    Eigen::VectorXf getWeights(){
        Eigen::VectorXf w = Eigen::VectorXf(blendShapes.size());
        for(int i = 0; i < blendShapes.size(); i++){
            w(i) = blendShapes[i].weight;
        }
        return w;
    }

    void resetManipulators(){
        for(int i = 0; i < manipulators.size(); i++){
            manipulators[i].controlPoint->setPosition(initialPositions[manipulators[i].vertexIndex]);
            manipulators[i].currentPosition = initialPositions[manipulators[i].vertexIndex];
            manipulators[i].initialPosition = initialPositions[manipulators[i].vertexIndex];
            manipulators[i].delta = glm::vec3(0.0f, 0.0f, 0.0f);
        }
        updateWeights();
    }

    void updateDefaultMesh(){
        std::vector<Vertex>& vertices = defaultMesh->getVertices();
        for(int i = 0; i < vertices.size(); i++){
            glm::vec3 delta = glm::vec3(0.0f, 0.0f, 0.0f);
            for(int j = 0; j < blendShapes.size(); j++){
                if(blendShapes[j].weight > 0.0f && isNotZero(blendShapes[j].deltas[i])){
                    delta += blendShapes[j].deltas[i] * blendShapes[j].weight;
                }
            }
            vertices[i].Position = initialPositions[i] + delta;
        }
        defaultMesh->updateVertexBuffer();
    }

    void updateWeights() {
        Eigen::VectorXf m = updateM();
        Eigen::MatrixXf _B = Eigen::MatrixXf::Zero(manipulators.size() * 3, blendShapes.size());
        Eigen::MatrixXf _Bt = Eigen::MatrixXf::Zero(blendShapes.size(), manipulators.size() * 3);
        Eigen::MatrixXf _BtB;
        Eigen::VectorXf w0 = getWeights();

        // Populate _B and _Bt matrices
        for (int i = 0; i < blendShapes.size(); i++) {
            for (int j = 0; j < manipulators.size(); j++) {
                int vertexIndex = manipulators[j].vertexIndex;
                _B(j * 3, i) = blendShapes[i].verticies[vertexIndex].x;
                _B(j * 3 + 1, i) = blendShapes[i].verticies[vertexIndex].y;
                _B(j * 3 + 2, i) = blendShapes[i].verticies[vertexIndex].z;
            }
        }

        _Bt = _B.transpose();
        _BtB = _Bt * _B;
        Eigen::MatrixXf A = _BtB + ((alpha + u) * Eigen::MatrixXf::Identity(_BtB.rows(), _BtB.cols()));
        Eigen::MatrixXf b = (_Bt * m) + (alpha * w0);
        Eigen::LDLT<Eigen::MatrixXf> solver(A);
        Eigen::VectorXf w = solver.solve(b);
        w.normalize();
        
        // Clamp the weights and update blendShapes
        for (int i = 0; i < blendShapes.size(); i++) {
            blendShapes[i].weight = clamp(w(i), 1.0f, 0.0f);
        }

        updateDefaultMesh();
    }


    bool isNotZero(const glm::vec3& vec) {
        return vec.x != 0.0f || vec.y != 0.0f || vec.z != 0.0f;
    }

    float clamp(float value, float max, float min){
        return std::max(min, std::min(max, value));
    }

};

#endif  // FACE_MANIPULATION_H