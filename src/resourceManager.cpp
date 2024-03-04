#include "resourceManager.h"
#include "utils/programInfo.h"

std::vector<Shader *> ResourceManager::shaders;
std::vector<Texture *> ResourceManager::textures;
std::vector<Mesh *> ResourceManager::meshes;
std::vector<Model *> ResourceManager::models;
std::vector<GameObject *> ResourceManager::gameObjects;
GLFWwindow *ResourceManager::window;
float ResourceManager::deltaTime = 0.0f;
float ResourceManager::previousTime = 0.0f;
double ResourceManager::mouseX = 0.0;
double ResourceManager::mouseY = 0.0;
double ResourceManager::lastMouseX = 0.0;
double ResourceManager::lastMouseY = 0.0;
std::unordered_map<int, keyData> ResourceManager::keyStates;
std::unordered_map<int, keyData> ResourceManager::mouseStates;
Camera *ResourceManager::activeCamera;
std::vector<PointLight *> ResourceManager::pointLights;
std::vector<DirectionalLight *> ResourceManager::directionalLights;
bool ResourceManager::isDebug = false;
bool ResourceManager::isMouseEnabled = false;
int ResourceManager::screenWidth, ResourceManager::screenHeight;
std::map<GameObject *, std::vector<glm::vec3>> ResourceManager::pickableVerticies;
GameObject *ResourceManager::currentlySelected;

Model *ResourceManager::loadModel(const char *modelFile)
{
    Model *model = new Model(modelFile);
    model->setID(models.size());
    models.push_back(model);
    return model;
}

Model *ResourceManager::getModel(unsigned int ID)
{
    return models[ID];
}

Shader *ResourceManager::addShader(Shader *shader)
{
    shaders.push_back(shader);
    return shader;
}
Shader *ResourceManager::getShader(unsigned int ID)
{
    return shaders[ID];
}
Texture *ResourceManager::loadTexture(TextureType type, const char *textureFile, bool useMipmaps, GLenum interpolation)
{
    for (Texture *texture : textures)
    {
        if (strcmp(texture->getFilePath().c_str(), textureFile) == 0)
        {
            return texture;
        }
    }
    Texture *texture = new Texture(type, textureFile, useMipmaps, interpolation);
    textures.push_back(texture);
    return texture;
}
Texture *ResourceManager::getTexture(unsigned int ID)
{
    return textures[ID];
}
Mesh *ResourceManager::loadMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture *> textures)
{
    Mesh *mesh = new Mesh(vertices, indices, textures);
    mesh->setID(meshes.size());
    meshes.push_back(mesh);
    return mesh;
}

Mesh *ResourceManager::getMesh(unsigned int ID)
{
    return meshes[ID];
}

GameObject *ResourceManager::loadGameObject()
{
    GameObject *gameObject = new GameObject();
    gameObject->setID(gameObjects.size());
    gameObjects.push_back(gameObject);
    return gameObject;
}

GameObject *ResourceManager::getGameObject(unsigned int ID)
{
    return gameObjects[ID];
}

float ResourceManager::getDeltaTime()
{
    return deltaTime;
}

void ResourceManager::updateDeltaTime()
{
    float currentTime = glfwGetTime();
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;
}

void ResourceManager::updateKeysPressed()
{
    for (int i = 0; i < 1024; i++)
    {
        if (glfwGetKey(window, i) == GLFW_PRESS)
        {
            keyStates[i].isPressed = true;
            keyStates[i].pressDuration += deltaTime;
            if (i == GLFW_KEY_ESCAPE)
            {
                glfwSetWindowShouldClose(window, true);
            }
        }
        else
        {
            keyStates[i].isPressed = false;
            keyStates[i].pressDuration = 0.0f;
        }
    }
}

bool ResourceManager::isKeyPressed(int key)
{
    return keyStates[key].isPressed;
}

GLFWwindow *ResourceManager::createWindow(int width, int height, const char *title)
{

    screenWidth = width;
    screenHeight = height;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    // Mac m1 will automatically load OpenGL 2.1 without hints enabled
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize Glad" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    std::cout << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;

    return window;
}

GLFWwindow *ResourceManager::getWindow()
{
    return window;
}

double ResourceManager::getMouseX()
{
    return mouseX;
}

double ResourceManager::getMouseY()
{
    return mouseY;
}

double ResourceManager::getMouseDeltaX()
{
    return mouseX - lastMouseX;
}

double ResourceManager::getMouseDeltaY()
{
    return mouseY - lastMouseY;
}

void ResourceManager::updateMousePosition()
{
    lastMouseX = mouseX;
    lastMouseY = mouseY;

    if(OS == "APPLE"){
        mouseX = ImGui::GetIO().MousePos.x * 2;
        mouseY = ImGui::GetIO().MousePos.y * 2;
    }
    else
    {
        glfwGetCursorPos(window, &mouseX, &mouseY);
    }


    // glfwSetCursorPos(window, screenWidth / 2, screenHeight / 2);
}

void ResourceManager::initialize()
{
    for (GameObject *gameObject : gameObjects)
    {
        gameObject->OnStart();
    }

    // All lights in the scene have to be accessible to all shaders
    for (Shader *shader : shaders)
    {
        for (PointLight *light : pointLights)
        {
            shader->bindPointLight(light);
        }
        for (DirectionalLight *light : directionalLights)
        {
            shader->bindDirectionalLight(light);
        }
    }
}

void ResourceManager::runGameLoop()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_FRAMEBUFFER_SRGB); 

    updateDeltaTime();
    updateKeysPressed();
    updateMousePressed();
    updateMousePosition();

    if(OS == "APPLE"){    
        screenHeight = ImGui::GetIO().DisplaySize.y * 2;
        screenWidth = ImGui::GetIO().DisplaySize.x * 2;
    }

    activeCamera->OnUpdate();

    if (isDebug)
        ProgramInfo::printAllInfo();

    for (GameObject *gameObject : gameObjects)
    {
        gameObject->OnUpdate();
    }

    for (Shader *shader : shaders)
    {

        shader->Render();
    }
}

void ResourceManager::setActiveCamera(Camera *camera)
{
    if (activeCamera != nullptr)
    {
        activeCamera->setActive(false);
    }
    activeCamera = camera;
    activeCamera->setActive(true);
}

Camera *ResourceManager::getActiveCamera()
{
    return activeCamera;
}

void ResourceManager::updateMousePressed()
{
    for (int i = 0; i < 8; i++)
    {
        if (glfwGetMouseButton(window, i) == GLFW_PRESS)
        {
            mouseStates[i].isPressed = true;
            mouseStates[i].pressDuration += deltaTime;
        }
        else
        {
            mouseStates[i].isPressed = false;
            mouseStates[i].pressDuration = 0.0f;
        }
    }
}

bool ResourceManager::isMousePressed(int button)
{
    return mouseStates[button].isPressed;
}

DirectionalLight *ResourceManager::loadDirectionalLight(float strength, glm::vec3 rotation)
{
    DirectionalLight *light = new DirectionalLight(strength, rotation, glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    directionalLights.push_back(light);
    return light;
}

PointLight *ResourceManager::loadPointLight(float strength, glm::vec3 position, float constant, float linear, float quadratic)
{
    PointLight *light = new PointLight(strength, position, constant, linear, quadratic, glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    pointLights.push_back(light);
    return light;
}

void ResourceManager::setMouseEnabled(bool isEnabled)
{
    if (isEnabled)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    isMouseEnabled = isEnabled;
}

bool ResourceManager::getIsMouseEnabled()
{
    return isMouseEnabled;
}

keyData ResourceManager::getKeyData(int key)
{
    return keyStates[key];
}

keyData ResourceManager::getMouseData(int button)
{
    return mouseStates[button];
}

void ResourceManager::addGeometryInfo(GameObject *gameObject, std::vector<glm::vec3> vertexPositions)
{
    pickableVerticies[gameObject] = vertexPositions;
}

glm::vec3 ResourceManager::getMouseRayHit()
{
    int mouseX = ResourceManager::getMouseX();
    int mouseY = ResourceManager::getMouseY();
    int screenWidth = ResourceManager::getScreenWidth();
    int screenHeight = ResourceManager::getScreenHeight();
    glm::mat4 projectionMatrix = ResourceManager::getActiveCamera()->getProjectionMatrix();
    glm::mat4 viewMatrix = ResourceManager::getActiveCamera()->getViewMatrix();

    glm::vec3 window = glm::vec3(mouseX, screenHeight - mouseY - 1, 0);
    glReadPixels(mouseX, screenHeight - mouseY - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &window.z);

    return glm::unProject(window, viewMatrix, projectionMatrix, glm::vec4(0, 0, screenWidth, screenHeight));
}

glm::vec3 ResourceManager::getMouseRayOrigin()
{
    int mouseX = ResourceManager::getMouseX();
    int mouseY = ResourceManager::getMouseY();
    int screenWidth = ResourceManager::getScreenWidth();
    int screenHeight = ResourceManager::getScreenHeight();
    glm::mat4 projectionMatrix = ResourceManager::getActiveCamera()->getProjectionMatrix();
    glm::mat4 viewMatrix = ResourceManager::getActiveCamera()->getViewMatrix();

    glm::vec3 window = glm::vec3(mouseX, screenHeight - mouseY - 1, 0);
    return glm::unProject(window, viewMatrix, projectionMatrix, glm::vec4(0, 0, screenWidth, screenHeight));
}

GameObject *ResourceManager::checkMouseVertexPick(glm::vec3 &vertex)
{
    glm::vec3 mouseRayDirection = getMouseRayHit();
    GameObject *closestGameObject = nullptr;
    glm::vec3 closestVertex = glm::vec3(0, 0, 0);
    
    float closestDistance = 1000000;
    for ( const auto& pair : pickableVerticies)
    {
        GameObject* gameObject = pair.first;
        const std::vector<glm::vec3> vertices = pair.second;

        glm::mat4 modelMatrix = gameObject->getTransform();
        for (int i = 0; i < vertices.size(); i++)
        {
            glm::vec4 vertex = modelMatrix * glm::vec4(vertices[i], 1);
            float distance = glm::distance(glm::vec3(vertex), mouseRayDirection);
            if (distance < closestDistance)
            {
                closestDistance = distance;
                closestVertex = glm::vec3(vertex);
                closestGameObject = gameObject;
            }
        }
    }

    if(closestDistance > 1.0f){
        return nullptr;
    }
    vertex = closestVertex;
    return closestGameObject;
}

GameObject *ResourceManager::getCurrentlySelected()
{
    return currentlySelected;
}

void ResourceManager::setCurrentlySelected(GameObject *selected)
{
    currentlySelected = selected;
}
