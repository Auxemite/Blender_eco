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
#define TEMP_WIDTH 1920
#define TEMP_HEIGHT 1080

// Main Flags
inline bool alpha_feature = 0;
inline bool isFullscreen = true;
inline GLFWwindow* window = nullptr;
inline GLFWmonitor* monitor = nullptr;
inline const GLFWvidmode* mode = nullptr;

// Camera Flags
inline float lastFrame = 0.0;
inline glm::vec3 cameraDec = glm::vec3(0.0f, 5.0f, 0.0f); //decalage
inline glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
inline glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
inline glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
inline float deltaTime = 0.0f;
inline float radius = 10.0f;
inline float yaw = -45.0f;
inline float pitch =  0.0f;
inline float lastX =  400.0f;
inline float lastY =  300.0f;
inline int speed_rotation = 30;
inline int speed_zoom = 10;

// Mouse Flags
inline bool firstMouse = true;
inline bool mousePressed = false;
inline float sensitivity = 0.35f;
inline float zoom_sensitivity = 0.5f;

// Render Flags
inline int render_mode = 0;

class Env {
    public:
    GLuint render_image{};
    Scene scene;
    Image image;

//    glm::vec3 cameraPos;
    glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 3.0f);

    // Flags
    int fast_selection = 1;
    int action_mode = 0; // 0 = Camera; 1 = Move; 2 = Rotate; 3 = Scale
    int focus_axe = 0; // 0 = x; 1 = y; 2 = z

    // Opengl
//    std::vector<float> vertices;
//    std::vector<int> indices;
    std::vector<float> gridVertices;
    std::vector<unsigned int> VBOs;
    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> EBOs;

    unsigned int gridVBO, gridVAO;

    Env();

    void save_mesh(const std::string& filename) const;
    void create_texture();
    void change_bg(const std::string& name);
    void render(int mesh_index=-1);

    void edit_mode();
    void normal_mode();
    void add_mesh(const std::string& name);
    void delete_mesh();

    void update_camera();
    void update_data(int mesh_index);

    void cleanup(int mesh_index);
    void load_data(int mesh_index, std::vector<float> vertices, std::vector<int> indices);
    void load_grid();
    void draw_data(unsigned int shaderProgram, glm::mat4 model, glm::mat4 view, glm::mat4 projection, int mesh_index);
    void draw_grid(unsigned int shaderProgram, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
};

std::vector<float> generateGrid(int gridSize);