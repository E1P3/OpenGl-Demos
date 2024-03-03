#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "../../src/resourceManager.h"
#include "../../src/model.h"

class Manipulator : public GameObject {

public:

    Manipulator(glm::vec3 assignedVertex){
        this->setPosition(assignedVertex);
    }

    void setInfluenceRadius(float influenceRadius){
        this->influenceRadius = influenceRadius;
    }

    void setAssignedVertices(Model* model){
        this->assignedVertices = assignedVertices;
    }

private:

    std::vector<Vertex*> assignedVertices;
    float influenceRadius = 0.1f;

};

#endif // MANIPULATOR_H