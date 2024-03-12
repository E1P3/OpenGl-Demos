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

    manipulator(GameObject* controlPoint){
        this->controlPoint = controlPoint;
        this->initialPosition = controlPoint->getWorldPosition();
        this->currentPosition = initialPosition;
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
        ImGui::Begin("Face Manipulation");
        for(int i = 0; i < blendShapes.size(); i++){
            ImGui::SliderFloat(blendShapes[i].name.c_str(), &blendShapes[i].weight, 0.0f, 1.0f);
        }
        ImGui::End();
    }

    void addBlendShape(Mesh* mesh, std::string name){
        blendShape newBlendShape(mesh, defaultMesh, name);
        blendShapes.push_back(newBlendShape);
    }

    void addManipulator(GameObject* controlPoint){
        manipulator newManipulator(controlPoint);
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
               B(j, i) = blendShapes[i].verticies[j/3].x; 
               B(j + 1, i) = blendShapes[i].verticies[j/3].y; 
               B(j + 2, i) = blendShapes[i].verticies[j/3].z;
            }
        }

        Bt = B.transpose();
        BtB = Bt * B;
    }

private:
    Mesh* defaultMesh;
    std::vector<glm::vec3> initialPositions;
    float alpha = 0.001f;
    std::vector<manipulator> manipulators;
    std::vector<blendShape> blendShapes;
    Eigen::MatrixXf B;
    Eigen::MatrixXf Bt;
    Eigen::MatrixXf BtB;

    bool checkForManipulatorMovement(){
        for(int i = 0; i < manipulators.size(); i++){
            if(manipulators[i].currentPosition != manipulators[i].controlPoint->getWorldPosition()){
                manipulators[i].delta = manipulators[i].controlPoint->getWorldPosition() - manipulators[i].currentPosition;
                manipulators[i].currentPosition = manipulators[i].controlPoint->getWorldPosition();
                return true;
            }
        }
        return false;
    }

    Eigen::VectorXf updateM(){
        Eigen::VectorXf m = Eigen::VectorXf(manipulators.size()* 3);
        for(int i = 0; i < manipulators.size(); i++){
            m(i) = manipulators[i].delta.x;
            m(i + 1) = manipulators[i].delta.y;
            m(i + 2) = manipulators[i].delta.z;
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

    void updateWeights(){
        Eigen::VectorXf m = updateM();
        Eigen::VectorXf w0 = getWeights();
        Eigen::MatrixXf A = BtB + alpha * Eigen::MatrixXf::Identity(BtB.rows(), BtB.cols());
        Eigen::VectorXf b = Bt * m + alpha * w0;
        Eigen::LDLT<Eigen::MatrixXf> solver(A);
        Eigen::VectorXf w = solver.solve(b);
        for(int i = 0; i < blendShapes.size(); i++){
            blendShapes[i].weight = w(i);
        }

        updateDefaultMesh();
    }

    bool isNotZero(const glm::vec3& vec) {
        return vec.x != 0.0f || vec.y != 0.0f || vec.z != 0.0f;
    }

};

#endif  // FACE_MANIPULATION_H