#ifndef FACE_MANIPULATION_H
#define FACE_MANIPULATION_H

#include "../entityModule.h"
#include "../resourceManager.h"
#include "../animator.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>

struct WeightAnimation{
    bool isPlaying = false;
    bool isLoop = true;
    bool isCubic = false;
    float speed = 1.0f;
    float currentFrame = 0.0f;
    std::vector<FloatAnimator*> animators;

    void addAnimator(FloatAnimator* animator){
        animators.push_back(animator);
    }

    void setSpeed(float speed){
        this->speed = speed;
        for(int i = 0; i < animators.size(); i++){
            animators[i]->setSpeed(speed);
        }
    }

    void setIsLoop(bool isLoop){
        this->isLoop = isLoop;
        for(int i = 0; i < animators.size(); i++){
            animators[i]->setLoop(isLoop);
        }
    }

    void setIsCubic(bool isCubic){
        this->isCubic = isCubic;
        for(int i = 0; i < animators.size(); i++){
            animators[i]->setCubic(isCubic);
        }
    }

    void setIsPlaying(bool isPlaying){
        this->isPlaying = isPlaying;
        for(int i = 0; i < animators.size(); i++){
            animators[i]->setPlaying(isPlaying);
        }
    }

    void setCurrentFrame(float currentFrame){
        this->currentFrame = currentFrame;
        for(int i = 0; i < animators.size(); i++){
            animators[i]->setCurrentFrame(currentFrame);
        }
    }

    void update(){
        for(int i = 0; i < animators.size(); i++){
            animators[i]->update();
        }
        setCurrentFrame(animators[0]->getCurrentFrame());
    }

    void OnGui(){
        bool _isLoop = isLoop;
        bool _isCubic = isCubic;
        float _speed = speed;

        if(ImGui::Button("Reset")){
            setCurrentFrame(0);
            update();
            setIsPlaying(false);
        }

        if(ImGui::Button("Play")){
            setIsPlaying(true);
        }

        if(ImGui::Button("Stop")){
            setIsPlaying(false);
        }

        ImGui::Text("Current Frame %f", currentFrame);

        ImGui::Checkbox("Loop", &_isLoop);
        ImGui::Checkbox("Cubic", &_isCubic);
        ImGui::SliderFloat("Speed", &_speed, 0.0f, 100.0f);

        if(_isLoop != isLoop){
            setIsLoop(_isLoop);
        }

        if(_isCubic != isCubic){
            setIsCubic(_isCubic);
        }

        if(_speed != speed){
            setSpeed(_speed);
        }
    }


};

struct manipulator{
    GameObject* controlPoint;
    glm::vec3 currentPosition;
    int vertexIndex;

    manipulator(GameObject* controlPoint, int vertexIndex){
        this->controlPoint = controlPoint;
        this->currentPosition = controlPoint->getWorldPosition();
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

    void OnStart() override {}

    void OnUpdate() override {
        if(useManipulators){
            float index = checkForManipulatorMovement();
            if(index != -1){
                updateWeights(index);
            }
        } else {
            updateDefaultMesh();
        }
    }

    void OnGui() {

        float _alpha = alpha;
        float _u = u;

        ImGui::Checkbox("Use Manipulators", &useManipulators);
        ImGui::Checkbox("Use Animation", &useAnimation);

        if(useAnimation){
            if(useManipulators){
                useManipulators = false;
            }
            weightAnimation.OnGui();
            weightAnimation.update();
        }

        if(useManipulators){
            ImGui::SliderFloat("Alpha", &_alpha, 0.0f, 1.0f);
            ImGui::SliderFloat("U", &_u, 0.0f, 1.0f);
            if(ImGui::Button("Reset Manipulators")){
                resetManipulators();
            }
        }

        ImGui::Text("Blend Shapes");

        for(int i = 0; i < blendShapes.size(); i++){
            ImGui::SliderFloat(blendShapes[i].name.c_str(), &blendShapes[i].weight, 0.0f, 1.0f);
        }

        if(_alpha != alpha){
            alpha = _alpha;
            updateWeights();
        }

        if(_u != u){
            u = _u;
            updateWeights();
        }
    }

    void addBlendShape(Mesh* mesh, std::string name, float weight = 0.0f){
        blendShape newBlendShape(mesh, defaultMesh, name);
        newBlendShape.weight = weight;
        blendShapes.push_back(newBlendShape);
    }

    void addManipulator(GameObject* controlPoint, int vertexIndex){
        manipulator newManipulator(controlPoint, vertexIndex);
        manipulators.push_back(newManipulator);
    }

    void loadWeightAnimation(const std::string& filename){
        std::vector<std::vector<float>> animationFrames = AnimatorParser::parseAnimationData(filename);
        for (int i = 0; i < blendShapes.size(); i++) {
            FloatAnimator* animator = new FloatAnimator(&blendShapes[i].weight, 0.5f);
            animator->setKeyFrames(animationFrames[i]);
            weightAnimation.addAnimator(animator);
        }
    }

private:
    Mesh* defaultMesh;
    std::vector<glm::vec3> initialPositions;
    float alpha = 0.1f;
    float u = 0.001f;
    std::vector<manipulator> manipulators;
    std::vector<blendShape> blendShapes;
    bool useManipulators = true;
    WeightAnimation weightAnimation;
    bool useAnimation = false;

    float checkForManipulatorMovement(){
        for(int i = 0; i < manipulators.size(); i++){
            glm::vec3 currentManipulatorPosition = manipulators[i].controlPoint->getWorldPosition();
            if(manipulators[i].currentPosition != currentManipulatorPosition){
                manipulators[i].currentPosition = currentManipulatorPosition;
            return i;
            }
        }
        return -1;
    }

    Eigen::VectorXf updateM(){
        Eigen::VectorXf m = Eigen::VectorXf(manipulators.size()* 3);
        glm::vec3 delta = glm::vec3(0.0f,0.0f,0.0f);
        for(int i = 0; i < manipulators.size() * 3; i+=3){
            delta = manipulators[i/3].currentPosition - initialPositions[manipulators[i/3].vertexIndex];
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
        }
        updateWeights();
    }

    void updateDefaultMesh(int mainpulatorIndex = -1){
        std::vector<Vertex>& vertices = defaultMesh->getVertices();
        for(int i = 0; i < vertices.size(); i++){
            glm::vec3 delta = glm::vec3(0.0f, 0.0f, 0.0f);
            for(int j = 0; j < blendShapes.size(); j++){
                if(blendShapes[j].weight > 0.0f){
                    delta += blendShapes[j].deltas[i] * blendShapes[j].weight;
                }
            }
            vertices[i].Position = initialPositions[i] + delta;
        }

        if(mainpulatorIndex != -1){
            manipulators[mainpulatorIndex].controlPoint->setPosition(vertices[manipulators[mainpulatorIndex].vertexIndex].Position);
            manipulators[mainpulatorIndex].currentPosition = vertices[manipulators[mainpulatorIndex].vertexIndex].Position;
        }

        defaultMesh->updateVertexBuffer();
    }

    void updateWeights(int manipulatorIndex = -1) {
        Eigen::VectorXf m = updateM();
        Eigen::MatrixXf _B = Eigen::MatrixXf::Zero(manipulators.size() * 3, blendShapes.size());
        Eigen::MatrixXf _Bt = Eigen::MatrixXf::Zero(blendShapes.size(), manipulators.size() * 3);
        Eigen::MatrixXf _BtB;
        Eigen::VectorXf w0 = getWeights();

        // Populate _B and _Bt matrices
        for (int i = 0; i < blendShapes.size(); i++) {
            for (int j = 0; j < manipulators.size(); j++) {
                int vertexIndex = manipulators[j].vertexIndex;
                _B(j * 3, i) = blendShapes[i].deltas[vertexIndex].x;
                _B(j * 3 + 1, i) = blendShapes[i].deltas[vertexIndex].y;
                _B(j * 3 + 2, i) = blendShapes[i].deltas[vertexIndex].z;
            }
        }

        _Bt = _B.transpose();
        _BtB = _Bt * _B;
        Eigen::MatrixXf A = _BtB + ((alpha + u) * Eigen::MatrixXf::Identity(_BtB.rows(), _BtB.cols()));
        Eigen::MatrixXf b = (_Bt * m) + (alpha * w0);
        Eigen::LDLT<Eigen::MatrixXf> solver(A);
        Eigen::VectorXf w = solver.solve(b);
        
        // Clamp the weights and update blendShapes
        for (int i = 0; i < blendShapes.size(); i++) {
            blendShapes[i].weight = clamp(w(i), 1.0f, 0.0f);
        }

        updateDefaultMesh(manipulatorIndex);
    }


    bool isNotZero(const glm::vec3& vec) {
        return vec.x != 0.0f || vec.y != 0.0f || vec.z != 0.0f;
    }

    float clamp(float value, float max, float min){
        return std::max(min, std::min(max, value));
    }

};

#endif  // FACE_MANIPULATION_H