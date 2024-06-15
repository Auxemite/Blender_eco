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
    bool photorealist = false;
    int fast_selection = 1;
    int selected_mode = 0;
    bool editmode = false;
    Scene scene;
    Image image;
    Mesh *focus_mesh;
    Triangle *focus_face = nullptr;

    Env();
    explicit Env(const char* filename);

    void create_texture();
    void save_mesh(std::string filename);

    void render();
    void move_camera_x(double angle);
    void move_camera_y(double angle);
    void change_material(Color color, Texture texture);

    void move_camera_z(double angle);
    void add_mesh(std::string name);
    void delete_mesh(Mesh *mesh);
    void select_mesh(int x, int y);
    void change_focus(Mesh *mesh);
    void change_focus(Mesh *mesh, Triangle *face);
    void update_selection_mode();

    void move_x(double value);
    void move_y(double value);
    void move_z(double value);
    void rotate_x(double angle);
    void rotate_y(double angle);
    void rotate_z(double angle);
    void scale(double value);
    void extrude(double x_, double y_, double z_);
};
