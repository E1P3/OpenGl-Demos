#ifndef IMGUI_WRAPPER_H
#define IMGUI_WRAPPER_H

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class ImGuiWrapper
{
public:
    ImGuiWrapper();
    ~ImGuiWrapper();

    void init();
    void update();
    void render();
    void shutdown();
    bool show_demo_window = true;
    bool show_another_window = false;
    float clear_color[3] = { 0.45f, 0.55f, 0.60f };

private:
    ImGuiIO io; // Add this line to initialize the reference member
};

#endif // !IMGUI_WRAPPER_H