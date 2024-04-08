#pragma once

#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "../backend/src/image.hh"
#include "../backend/backend_main.hh"
#include "GLFW/glfw3.h"

class App {
    public:
    GLuint render_image;
    Image image;
    Scene scene;

    App();
    explicit App(const char* filename);

    void viewport_update();

    void RenderUI();
    void Windows();
    [[nodiscard]] static unsigned char* convertDataToGLRGB(const vector<vector<Color>>& data, int width, int height);
};