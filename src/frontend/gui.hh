#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "graphicsUtils.hh"

namespace Gui {

    inline GLuint fbo = 0;
    inline GLuint colorTex = 0;
    inline GLuint depthRbo = 0;
    inline int lastW = 0;
    inline int lastH = 0;
    inline ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_MenuBar;
    inline bool dock_built = false;

    void initialize(ImGuiIO &io, GLFWwindow *window);
    void newFrame();
    void render(ImGuiIO &io);
    void shutDown();

    void setUpTextureViewport();
    void renderTextureViewport();
    ImVec2 getResizeAvail();
};