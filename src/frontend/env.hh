#pragma once

#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "GLFW/glfw3.h"

#include "../backend/render/intersection.hh"
#include "../backend/image/image.hh"
#include "action.hh"

class Env {
    public:
    GLuint render_image{};
    Scene scene;
    Image image;
    std::string focus_type = "sphere";
    int focus_index = 1;

    Env();
    explicit Env(const char* filename);

    void update_texture();
    [[nodiscard]] static unsigned char* convertDataToGLRGB(const vector<vector<Color>>& data, int width, int height);

    void render();
    void fast_render();
    void move_camera();
    void move_right();
    void move_left();
    void grow();
    void shrink();
    void change_focus(int index, const string& type);
};
