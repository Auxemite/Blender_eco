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
    Mesh *focus_mesh;
    Triangle *focus_face = nullptr;

    // Flags
    bool photorealist = false;
    int fast_selection = 1;
    int selected_mode = 0;
    bool editmode = false;

    Env();
    explicit Env(const char* filename);

    void create_texture();
    void save_mesh(const std::string& filename) const;
    void change_bg(const std::string& name);

    void render();
    void move_camera_x(float angle);
    void move_camera_y(float angle);
    void change_material(Color color, Texture texture);

    void move_camera_z(float angle);
    void add_mesh(const std::string& name);
    void delete_mesh(Mesh *mesh);
    void select_mesh(float x, float y);
    void change_focus(Mesh *mesh);
    void change_focus(Mesh *mesh, Triangle *face);
    void update_selection_mode();

    void move_x(float value);
    void move_y(float value);
    void move_z(float value);
    void rotate_x(float angle);
    void rotate_y(float angle);
    void rotate_z(float angle);
    void scale(float value);
    void extrude(float x_, float y_, float z_);
};
