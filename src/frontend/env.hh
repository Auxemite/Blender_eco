#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "../backend/render/intersection.hh"
#include "../backend/image/image.hh"

class Env {
    public:
    GLuint render_image{};
    Scene scene;
    Image image;

    // Flags
    bool photorealist = false;
    int fast_selection = 1;
    int action_mode = 0; // 0 = Camera; 1 = Move; 2 = Rotate; 3 = Scale
    int focus_axe = 0; // 0 = x; 1 = y; 2 = z

    Env();
    explicit Env(const char* filename);

    void save_mesh(const std::string& filename) const;
    void create_texture();
    void change_bg(const std::string& name);
    void render();
};
