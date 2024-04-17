#pragma once

#include "../../src/shader.h"
#include "../../src/resourceManager.h"
#include "../../src/entityModules/renderModule.h"
#include "terrain_patch.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class RoamShader : public Shader {

public:
    RoamShader(const char* PVS, const char* PFS, TerrainPatch* terrainPatch, float LODScaling = 216.0f, float errorMargin = 0.0045f){
        this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS));
        this->terrainPatch = terrainPatch;
        this->terrainPatch->computeVariance(20);
        this->LODScaling = LODScaling;
        this->errorMargin = errorMargin;
    }

    void Render() override {
        this->Use();

        if(isInitialised == false){
            initialiseTerrainPatch();
            isInitialised = true;
        }

        this->SetMatrix4("view", ResourceManager::getActiveCamera()->getViewMatrix());
        this->SetMatrix4("projection", ResourceManager::getActiveCamera()->getProjectionMatrix());

        if(pointLightsToRender[0] != nullptr){
            this->SetVector3f("pointLight.position", pointLightsToRender[0]->getPosition());
            this->SetVector3f("pointLight.ambient", pointLightsToRender[0]->getAmbient());
            this->SetVector3f("pointLight.diffuse", pointLightsToRender[0]->getDiffuse());
            this->SetVector3f("pointLight.specular", pointLightsToRender[0]->getSpecular());
        }

        if(dirLightsToRender[0] != nullptr){
            this->SetVector3f("dirLight.direction", glm::eulerAngles(dirLightsToRender[0]->getRotation()));
            this->SetVector3f("dirLight.ambient", dirLightsToRender[0]->getAmbient());
            this->SetVector3f("dirLight.diffuse", dirLightsToRender[0]->getDiffuse());
            this->SetVector3f("dirLight.specular", dirLightsToRender[0]->getSpecular());
        }

        for(RenderModule* module : objectsToRender){
            this->SetMatrix4("model", module->getParent()->getTransform());
            module->material->Draw(this);
            drawTerrainPatch();
        }
    }

    void initialiseTerrainPatch(){
        const size_t poolSize = this->terrainPatch->poolSize();
        this->triPool = new float[poolSize*9];
        this->colorPool = new float[poolSize*9];
        this->normalTexelPool = new float[poolSize*9];

        glGenBuffers(3, buffers);
        glGenVertexArrays(3, arrays);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBindVertexArray(arrays[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*poolSize*9, NULL, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBindVertexArray(arrays[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*poolSize*9, NULL, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
        glBindVertexArray(arrays[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*poolSize*6, NULL, GL_STREAM_DRAW);

        // generate normal texture
        GLuint normalTexture = 0;
        Heightmap *map = this->terrainPatch->getHeightmap();
        glGenTextures(1, &normalTexture);
        glBindTexture(GL_TEXTURE_2D, normalTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, map->width, map->height, 0, GL_RGB, GL_FLOAT, map->normal_map);

    }

    void setTarget(GameObject* target){
        this->target = target;
    }

    void drawTerrainPatch(GameObject* self = nullptr){

        if(isWireframe){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

		glLineWidth(2.0);
        this->terrainPatch->reset();
        glm::vec3 referencePosition = target != nullptr ? target->getPosition() : ResourceManager::getActiveCamera()->getPosition();
        this->terrainPatch->tessellate(referencePosition * scaler, LODScaling, errorMargin);
        this->terrainPatch->getTessellation(triPool, colorPool, normalTexelPool);

        size_t leaves = this->terrainPatch->amountOfLeaves();

        // update the buffer data
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*9*leaves, triPool);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*9*leaves, colorPool);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*6*leaves, normalTexelPool);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

        // normal texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, normalTexture);
		this->SetInteger("texture_normal", 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, leaves*3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

        if(isWireframe){
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

    }

    void OnGui() {
        ImGui::Begin("Roam Shader");
        ImGui::Text("This project renders terrain based on the position of the camera.");
        ImGui::Text("Use LODScaling and ErrorMargin to adjuts the LOD generation.");
        ImGui::Text("Press F to change from third person to first person view.");
        ImGui::SliderFloat("LOD Scaling", &LODScaling, 0.0f, 1024.0f);
        errorMargin = errorMargin * 1000.0f;
        ImGui::SliderFloat("Error Margin", &errorMargin, 0.0f, 100.0f);
        errorMargin = errorMargin * 0.001f;
        int currentTessLevel = this->tessalationLevel;
        ImGui::SliderInt("Tessalation Level", &this->tessalationLevel, 1, 30);
        if(currentTessLevel != this->tessalationLevel){
            this->terrainPatch->computeVariance(this->tessalationLevel);
        }
        ImGui::Checkbox("Wireframe", &isWireframe);
        ImGui::End();
    }

private:
    TerrainPatch* terrainPatch;
    GameObject* target = nullptr;
    float scaler = 0.001f;
    GLuint buffers[3];
    GLuint arrays[3];
    GLuint normalTexture;
    float* triPool;
    float* colorPool;
    float* normalTexelPool;
    bool isInitialised = false;
    float LODScaling = 216.0f;
    float errorMargin = 0.0045f;
    int tessalationLevel = 20;
    bool isWireframe = true;

    void reverseArray(float* array, int size) {
        int i, j;
        float temp;
        for (i = 0, j = size - 1; i < j; i++, j--) {
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }

};