#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "graphics/graphicsUtils.hh"
#include "backend/scene.hh"

namespace Gui {
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar
                                        | ImGuiWindowFlags_NoCollapse
                                        | ImGuiWindowFlags_NoResize
                                        | ImGuiWindowFlags_NoMove
                                        | ImGuiWindowFlags_NoBringToFrontOnFocus
                                        | ImGuiWindowFlags_NoNavFocus
                                        | ImGuiWindowFlags_NoBackground;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
//    inline double mouseX, mouseY;

    void initialize(ImGuiIO &io, GLFWwindow *window);
    void newFrame(ImGuiIO &io);
    void render(ImGuiIO &io);
    void shutDown();
};