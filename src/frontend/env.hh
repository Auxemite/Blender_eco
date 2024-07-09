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
#include "render_utils.h"

inline unsigned int VBO, VAO, EBO;
inline float lastFrame = 0.0;
inline glm::vec3 cameraPos;
inline glm::vec3 cameraFront;
inline glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
inline float deltaTime = 0.0f;
inline float radius = 3.0f;
inline float yaw = -90.0f;
inline float vertices[] = {
        // positions          // colors
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
//        -3.0f,  3.0f,  3.0f,  0.0f, 0.0f, 0.0f
};
static float r = 0.0f;
static float g = 0.0f;
static float b = 0.0f;
//inline float vertices[] = {
//        // positions          // colors
//        -0.5f, -0.5f, -0.5f,  r, g, b,
//        0.5f, -0.5f, -0.5f,  r, g, b,
//        0.5f,  0.5f, -0.5f,  r, g, b,
//        -0.5f,  0.5f, -0.5f,  r, g, b,
//        -0.5f, -0.5f,  0.5f,  r, g, b,
//        0.5f, -0.5f,  0.5f,  r, g, b,
//        0.5f,  0.5f,  0.5f,  r, g, b,
//        -0.5f,  0.5f,  0.5f,  r, g, b,
////        -3.0f,  3.0f,  3.0f,  0.0f, 0.0f, 0.0f
//};
inline int indices[] = {
        0, 1, 3,
        1, 2, 3,
        1, 5, 2,
        5, 6, 2,
        5, 4, 6,
        4, 7, 6,
        4, 0, 7,
        0, 3, 7,
        3, 2, 7,
        2, 6, 7,
        4, 5, 0,
        5, 1, 0,
//        0, 3, 8,
};

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

    void update_data();
    void cleanup();
    void load_data();
    void draw_data(unsigned int shaderProgram);

    void save_mesh(const std::string& filename) const;
    void create_texture();
    void change_bg(const std::string& name);
    void render();
};
