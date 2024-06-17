#pragma once

#include "object.hh"
#include "mesh.hh"
#include "camera.hh"
#include "light.hh"
#include <vector>

class Scene
{
    public:
        std::vector<Shape*> objects;
        std::vector<Mesh *> meshes;
        std::vector<Light*> lights;
        Camera camera;
        Mesh *focus_mesh = nullptr;
        Triangle *focus_face = nullptr;
//        Point3 *focus_edge[2];
        Point3 *focus_summit = nullptr;

        // Flags
        int selected_mode = 0;
        int activate_grid = 1;
        bool editmode = false;

        Scene()=default;
        Scene(int width, int height);
        Scene(std::vector<Shape*> sphere_, std::vector<Light*> lights_, Camera camera_);

        void move_camera_x(float angle);
        void move_camera_y(float angle);
        void change_material(Color color, Texture texture);

        void move_camera_z(float angle);
        void add_mesh(const std::string& name);
        void delete_mesh();
        void select_mesh(float x, float y);
        void select_summit(float x, float y);
        void change_focus(Mesh *mesh);
        void change_focus(Mesh *mesh, Triangle *face);
        void change_focus(Mesh *mesh, Point3 *summit);
        void update_selection_mode();

        void move_x(float value);
        void move_y(float value);
        void move_z(float value);
        void rotate_x(float angle);
        void rotate_y(float angle);
        void rotate_z(float angle);
        void scale(float value);
        void extrude(float x_, float y_, float z_);

        void add_mesh(Mesh *mesh);
        void select(int x, int y) const;
};
