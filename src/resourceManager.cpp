#include "resourceManager.h"
#include "utils/programInfo.h"

std::vector<Shader*> ResourceManager::shaders;
std::vector<Texture*> ResourceManager::textures;
std::vector<Material*> ResourceManager::materials;
std::vector<Mesh*> ResourceManager::meshes;
std::vector<Model*> ResourceManager::models;
std::vector<GameObject*> ResourceManager::gameObjects;
GLFWwindow* ResourceManager::window;
float ResourceManager::deltaTime = 0.0f;
float ResourceManager::previousTime = 0.0f;
double ResourceManager::mouseX = 0.0;
double ResourceManager::mouseY = 0.0;
double ResourceManager::lastMouseX = 0.0;
double ResourceManager::lastMouseY = 0.0;
std::unordered_map<int, bool> ResourceManager::keyStates;
std::unordered_map<int, bool> ResourceManager::mouseStates;
Camera* ResourceManager::activeCamera;
std::vector<PointLight*> ResourceManager::pointLights;
std::vector<DirectionalLight*> ResourceManager::directionalLights;

Model* ResourceManager::loadModel(const char* modelFile){
        Model* model = new Model(modelFile);
        model->setID(models.size());
        models.push_back(model);
        return model;
    }

Model* ResourceManager::getModel(unsigned int ID){
    return models[ID];
}

Shader* ResourceManager::addShader(Shader* shader){
        shaders.push_back(shader);
        return shader;
}
Shader* ResourceManager::getShader(unsigned int ID){
    return shaders[ID];
}
Texture* ResourceManager::loadTexture( TextureType type, const char* textureFile){
    for (Texture* texture : textures) {
        if (strcmp(texture->getFilePath().c_str(), textureFile) == 0) {
            return texture;
        }
    }
    Texture* texture = new Texture(type, textureFile);
    textures.push_back(texture);
    return texture;
}
Texture* ResourceManager::getTexture(unsigned int ID){
    return textures[ID];
}
Mesh* ResourceManager::loadMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures){
    Mesh* mesh = new Mesh(vertices, indices, textures);
    mesh->setID(meshes.size());
    meshes.push_back(mesh);
    return mesh;
}
Mesh* ResourceManager::getMesh(unsigned int ID){
    return meshes[ID];
}

Material* ResourceManager::loadMaterial(float ambient, float diffuse, float specular){
    Material* material = new Material(ambient, diffuse, specular);
    material->setID(materials.size());
    materials.push_back(material);
    return material;
}

Material* ResourceManager::getMaterial(unsigned int ID){
    return materials[ID];
}

GameObject* ResourceManager::loadGameObject(){
    GameObject* gameObject = new GameObject();
    gameObject->setID(gameObjects.size());
    gameObjects.push_back(gameObject);
    return gameObject;
}

GameObject* ResourceManager::getGameObject(unsigned int ID){
    return gameObjects[ID];
}

float ResourceManager::getDeltaTime(){
    return deltaTime;
}

void ResourceManager::updateDeltaTime(){
    float currentTime = glfwGetTime();
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;
}

void ResourceManager::updateKeysPressed(){
    for (int i = 0; i < 1024; i++) {
        if (glfwGetKey(window, i) == GLFW_PRESS) {
            keyStates[i] = true;
        }
        else {
            keyStates[i] = false;
        }
    }
}

bool ResourceManager::isKeyPressed(int key){
    return keyStates[key];
}

GLFWwindow* ResourceManager::createWindow(int width, int height, const char* title){
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    // Mac m1 will automatically load OpenGL 2.1 without hints enabled
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize Glad" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    std::cout << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;

    return window;
}

GLFWwindow* ResourceManager::getWindow(){
    return window;
}

double ResourceManager::getMouseX(){
    return mouseX;
}

double ResourceManager::getMouseY(){
    return mouseY;
}

double ResourceManager::getMouseDeltaX(){
    return mouseX - lastMouseX;
}

double ResourceManager::getMouseDeltaY(){
    return mouseY - lastMouseY;
}

void ResourceManager::updateMousePosition(){
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
}

void ResourceManager::initialize(){
    for(GameObject* gameObject : gameObjects){
        gameObject->OnStart();
    }

    // All lights in the scene have to be accessible to all shaders
    for(Shader* shader : shaders){
        for(PointLight* light : pointLights){
            shader->bindPointLight(light);
        }
        for(DirectionalLight* light : directionalLights){
            shader->bindDirectionalLight(light);
        }
    }
}

void ResourceManager::runGameLoop(){
    updateDeltaTime();
    updateKeysPressed();
    updateMousePosition();
    ProgramInfo::printAllInfo();

    for(GameObject* gameObject : gameObjects){
        gameObject->OnUpdate();
    }

    for(Shader* shader : shaders){
        shader->Render();
    }
}

void ResourceManager::setActiveCamera(Camera* camera){
    activeCamera = camera;
}

Camera* ResourceManager::getActiveCamera(){
    return activeCamera;
}

void ResourceManager::updateMousePressed(){
    for (int i = 0; i < 8; i++) {
        if (glfwGetMouseButton(window, i) == GLFW_PRESS) {
            mouseStates[i] = true;
        }
        else {
            mouseStates[i] = false;
        }
    }
}

bool ResourceManager::isMousePressed(int button){
    return mouseStates[button];
}

DirectionalLight* ResourceManager::loadDirectionalLight(float strength, glm::vec3 position, glm::quat rotation){
    DirectionalLight* light = new DirectionalLight(strength, position, rotation);
    directionalLights.push_back(light);
    return light;
}

PointLight* ResourceManager::loadPointLight(float strength ,glm::vec3 position, float constant, float linear, float quadratic){
    PointLight* light = new PointLight(strength, position, constant, linear, quadratic);
    pointLights.push_back(light);
    return light;
}

