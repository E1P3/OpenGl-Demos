#ifndef IMGUI_WRAPPER_H
#define IMGUI_WRAPPER_H

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <functional>
#include <map>
#include <string>

class ImGuiWrapper
{
public:
    ImGuiWrapper();
    ~ImGuiWrapper();

    using GuiFunction = std::function<void()>;

    void init();
    void update();
    void render();
    void shutdown();
    void attachGuiFunction(std::string name, GuiFunction guiFunction);
    bool show_demo_window = true;
    bool show_another_window = false;
    float clear_color[3] = { 0.45f, 0.55f, 0.60f };

private:
    ImGuiIO io; // Add this line to initialize the reference member
    std::map<std::string, GuiFunction> guiFunctions;
};

#endif // !IMGUI_WRAPPER_H