#pragma once

#include <cmath>
#include "../utils/vector3.hh"

class Camera
{
    public:
        Point3 center = Point3();
        Vector3 pixel_v = Vector3();
        Vector3 pixel_u = Vector3();
        Point3 pixel_loc;
        Point3 lookat;
        int width = 0;
        int height = 0;

        Camera()= default;
        Camera(Point3 center_, Point3 lookat_, int width_, int height_);

        Vector3 get_dir(int i, int j);
        void update_cam(Point3 center_);
        void update_lookat(Point3 lookat_);
};
