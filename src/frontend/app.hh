#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "GLFW/glfw3.h"

#define WIDTH 854
#define HEIGHT 480

class App {
    public:
    unsigned char* char_data;
    GLuint render_image{};

    App();

    void Windows();
    void UpdateTexture();
    void Inputs(const ImGuiIO& io, ImVec2 pos);
};