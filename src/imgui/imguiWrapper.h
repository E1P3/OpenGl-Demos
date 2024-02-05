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
    using GuiFunction = std::function<void()>;

    static void init();
    static void update();
    static void render();
    static void shutdown();
    static void attachGuiFunction(std::string name, GuiFunction guiFunction);

private:
    static ImGuiIO io; // Add this line to initialize the reference member
    static std::map<std::string, GuiFunction> guiFunctions;
};

#endif // !IMGUI_WRAPPER_H