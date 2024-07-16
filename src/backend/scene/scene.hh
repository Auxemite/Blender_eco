#pragma once

#include "object.hh"
#include "mesh.hh"
#include "camera.hh"
#include "light.hh"
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Scene
{
    public:
        std::vector<Shape*> objects;
        std::vector<Mesh *> meshes;
        std::vector<Light*> lights;
        Camera camera;
        Mesh *focus_mesh = nullptr;
        int focus_index = -1;
        std::vector<Triangle *> focus_faces;
        Point3 *focus_summit = nullptr;
        float dec_x = 0.0f;
        float dec_y = 0.0f;
        float dec_z = 0.0f;
        float kd = 0.9f;
        float ks = 0.1f;
        float ns = 10.0f;

        // Flags
        int selected_mode = 0;
        bool activate_grid = true;
        bool editmode = false;

        Scene()=default;
        Scene(int width, int height);
        Scene(std::vector<Shape*> sphere_, std::vector<Light*> lights_, Camera camera_);

        void move_camera_x(float angle);
        void move_camera_y(float angle);
        void zoom_camera(float value);
        void change_material(float kd, float ks, float ns);
        void change_material(Color color, Texture texture);

        void move_camera_z(float angle);
        void add_mesh(const std::string& name);
        void delete_mesh();
        int select_mesh(float x, float y);
        void select_summit(float x, float y);
        void apply_mesh_changes();
        void change_focus(Mesh *mesh, int mesh_index);
        void change_focus(Mesh *mesh, Triangle *face);
        void change_focus(Mesh *mesh, Point3 *summit);
        void update_selection_mode();

        void move_x(float value);
        void move_y(float value);
        void move_z(float value);
        void rotate_xyz(float anglex, float angley, float anglez);
        void scale(float value);
        void extrude(float x_, float y_, float z_);
        void extrude_along_normal(float thickness);
        void extrude_along_points_normalized(float thickness);

        void add_mesh(Mesh *mesh);
        void select(int x, int y) const;
};
