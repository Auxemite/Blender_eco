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
    Scene scene;
    Image image;
//    Shape_data *focus_obj;
    Mesh *focus_mesh;
    int focus_index = 1;

    Env();
    explicit Env(const char* filename);

    void create_texture();

    void render();
    void move_camera_x(double angle);
    void move_camera_y(double angle);
    void move_camera_z(double angle);
    void move_x(double value);
    void move_y(double value);
    void move_z(double value);
    void rotate_x(double angle);
    void rotate_y(double angle);
    void rotate_z(double angle);
    void scale(double value);
//    void change_focus(int index, Shape *shape);
    void select_mesh(int x, int y);
    void change_focus(Mesh *mesh);
};
