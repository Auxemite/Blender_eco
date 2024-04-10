#pragma once

#include "../object/object.hh"
#include "camera.hh"
#include "light.hh"
#include <vector>

class Scene
{
    public:
        vector<Sphere> spheres;
        vector<Point_Light> lights;
        Camera camera;

        Scene()=default;
        Scene(int width, int height);
        Scene(vector<Sphere> sphere_, vector<Point_Light> lights_, Camera camera_);
};
