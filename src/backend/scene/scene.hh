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

        // Flags
        int activate_grid = 1;

        Scene()=default;
        Scene(int width, int height);
        Scene(std::vector<Shape*> sphere_, std::vector<Light*> lights_, Camera camera_);
        void add_mesh(Mesh *mesh);
        void select(int x, int y) const;
};
