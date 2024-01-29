#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

class RenderModule;
class DirectionalLight;
class PointLight;

class Shader {
public:
    Shader();
    Shader(const char* PVS, const char* PFS);
    Shader& Use();
    unsigned int getID() const;

    void setDebug(bool debug);

    void SetFloat(const std::string& name, float value, bool useShader = false);
    void SetInteger(const std::string& name, int value, bool useShader = false);
    void SetVector2f(const std::string& name, float x, float y, bool useShader = false);
    void SetVector2f(const std::string& name, const glm::vec2& value, bool useShader = false);
    void SetVector3f(const std::string& name, float x, float y, float z, bool useShader = false);
    void SetVector3f(const std::string& name, const glm::vec3& value, bool useShader = false);
    void SetVector4f(const std::string& name, float x, float y, float z, float w, bool useShader = false);
    void SetVector4f(const std::string& name, const glm::vec4& value, bool useShader = false);
    void SetMatrix4(const std::string& name, const glm::mat4& matrix, bool useShader = false);

    void Compile(const char* PVS, const char* PFS);
    void Delete();
    virtual void Render();

    void bindRenderModule(RenderModule* object);
    void bindDirectionalLight(DirectionalLight* light);
    void bindPointLight(PointLight* light);

protected:
    char* readShaderSource(const char* shaderFile);
    std::vector<RenderModule*> objectsToRender;
    std::vector<DirectionalLight*> dirLightsToRender;
    std::vector<PointLight*> pointLightsToRender;

private:
    bool isDebug = false;
    unsigned int ID;
    unsigned int AddShader(const char* shaderText, GLenum shaderType);


};

#endif // SHADER_CLASS_H
