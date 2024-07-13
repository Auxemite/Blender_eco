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

#define WIDTH 1920
#define HEIGHT 1080

inline float lastFrame = 0.0;
inline glm::vec3 cameraDec = glm::vec3(0.0f, 5.0f, 0.0f); //decalage
inline glm::vec3 cameraFront;
inline glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
inline float deltaTime = 0.0f;
inline float radius = 10.0f;
inline float yaw = -45.0f;
//static float r, g, b;

class Env {
    public:
    GLuint render_image{};
    Scene scene;
    Image image;

    glm::vec3 cameraPos;

    // Flags
    bool photorealist = false;
    int fast_selection = 1;
    int action_mode = 0; // 0 = Camera; 1 = Move; 2 = Rotate; 3 = Scale
    int focus_axe = 0; // 0 = x; 1 = y; 2 = z

    // Opengl
//    std::vector<float> vertices;
//    std::vector<int> indices;
    std::vector<float> gridVertices;
//    unsigned int VBO, VAO, EBO;
    std::vector<unsigned int> VBOs;
    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> EBOs;

    unsigned int gridVBO, gridVAO;

    Env();
    explicit Env(const char* filename);

    void update_camera();
    void update_data(int mesh_index);
    void cleanup(int mesh_index);
    void load_data(int mesh_index, std::vector<float> vertices, std::vector<int> indices);
    void load_grid();
    void draw_data(unsigned int shaderProgram, glm::mat4 model, glm::mat4 view, glm::mat4 projection, int mesh_index);
    void draw_grid(unsigned int shaderProgram, glm::mat4 model, glm::mat4 view, glm::mat4 projection);

    void save_mesh(const std::string& filename) const;
    void create_texture();
    void change_bg(const std::string& name);
    void render(int mesh_index=-1);
};

std::vector<float> generateGrid(int gridSize);