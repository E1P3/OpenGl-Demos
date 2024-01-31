#include "imguiWrapper.h"


#include "../resourceManager.h"



void ImGuiWrapper::init() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO();

    #ifdef _WIN32
        char* glsl_version = "#version 130";
    #else
        char* glsl_version = "#version 330";
    #endif

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(ResourceManager::getWindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

void ImGuiWrapper::update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    for(auto& guiFunction : guiFunctions){
        ImGui::Begin(guiFunction.first.c_str());
        guiFunction.second();
        ImGui::End();
    }


}

void ImGuiWrapper::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWrapper::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiWrapper::attachGuiFunction(std::string name, GuiFunction guiFunction) {
    guiFunctions[name] = guiFunction;
}

ImGuiWrapper::ImGuiWrapper() {
    init();
}

ImGuiWrapper::~ImGuiWrapper() {
    shutdown();
}
