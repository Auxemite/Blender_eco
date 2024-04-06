//
// Created by ernes on 15/03/2024.
//

#ifndef RAYTRACER_SCENE_HH
#define RAYTRACER_SCENE_HH

#include "object/object.hh"
#include "camera.hh"
#include "light.hh"
#include <iostream>
#include <utility>
#include <vector>

class Scene
{
    public:
        vector<Sphere> spheres;
        vector<Point_Light> lights;
        Camera camera;

        Scene(vector<Sphere> sphere_, vector<Point_Light> lights_, Camera camera_) {
            spheres = std::move(sphere_);
            lights = std::move(lights_);
            camera = camera_;
        }
};

#endif //RAYTRACER_SCENE_HH
