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
#include "shader_utils.hh"

#define WIDTH 1920
#define HEIGHT 1080
#define TEMP_WIDTH 1280
#define TEMP_HEIGHT 720
inline Image *bg = load_image("../data/sunset.ppm");

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
inline int render_mode = 1;
inline bool display_image = false;
inline bool smooth = false;

// Fur Flags
inline int fur_length = 3;
inline float fur_size = 0.1;
inline Color fur_color = Color(0.0f, 0.7f, 0.7f);
inline int tesselation_surface = 1;
inline bool fur = false;
inline glm::vec3 waveAmplitude = glm::vec3(0.0f, 0.0f, 0.0f);
inline glm::vec3 waveFrequency = glm::vec3(0.0f, 0.0f, 0.0f);
inline bool waveDependance[3][2] = {{false, false}, {false, false}, {false, false}};

// Time Flags
inline float last_time = 0.0f;
inline float timer_interval = 0.033f;
inline float anim_time = 0.0f;
inline bool stop_anim_time = false;

// BRDF Flags
inline float metaless = 0.0f;
inline float roughness = 0.5f;

//enum RenderMode {
//    BASIC_MODE = 0,
//    NORMAL = 1,
//    PHONG = 2,
//    FUR = 3,
//    WAVE = 4,
//    WAVEHAIR = 5,
//    BRDF = 6,
//};

class Env {
    public:
    GLuint render_image{};
    Scene scene;
    Image image;

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
    unsigned int furVBO, furVAO;
    std::vector<float> furVertices;

    Env();

    void save_mesh(const std::string& filename) const;
    void create_texture();
    void change_bg(const std::string& name);
    void render(int mesh_index=-1);
    void render_all();

    void edit_mode();
    void normal_mode();
    void add_mesh(const std::string& name);
    void duplicate_mesh();
    void delete_mesh();

    void update_camera();
    void update_data(int mesh_index);

    void cleanup(int mesh_index);
    void load_data(int mesh_index, std::vector<float> vertices, std::vector<int> indices);
    void load_grid();
    void load_fur();
    void draw_data(unsigned int shaderProgram, glm::mat4 model, glm::mat4 view, glm::mat4 projection, int mesh_index, int render_id);
    void draw_grid(unsigned int shaderProgram, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    void drawFur(unsigned int shaderProgram, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
};

std::vector<float> generateGrid(int gridSize);
std::vector<float> generateFur();