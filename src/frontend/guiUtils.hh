#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "graphicsUtils.hh"

namespace Gui {

    inline GLuint fbo = 0;
    inline GLuint colorTex = 0;
    inline GLuint depthRbo = 0;
//    inline double mouseX, mouseY;

    void initialize(ImGuiIO &io, GLFWwindow *window);
    void newFrame();
    void render(ImGuiIO &io);
    void shutDown();

    void setUpTextureViewport();
    void renderTextureViewport();
    ImVec2 getResizeAvail();
};