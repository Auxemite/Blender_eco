#pragma once

#include "object.hh"
#include "camera.hh"
#include "light.hh"
#include <vector>

class Scene
{
    public:
        std::vector<Sphere> spheres;
        std::vector<Point_Light> lights;
        Camera camera;

        Scene()=default;
        Scene(int width, int height);
        Scene(std::vector<Sphere> sphere_, std::vector<Point_Light> lights_, Camera camera_);
};
