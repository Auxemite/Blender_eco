#pragma once

#include <cmath>
#include "../utils/vector3.hh"

class Camera
{
    public:
        Point3 center;
        Vector3 pixel_v;
        Vector3 pixel_u;
        Point3 pixel_loc;
        Point3 lookat;
        int width;
        int height;

        Camera()= default;
        Camera(Point3 center_, Point3 lookat_, int width_, int height_);

        void update(const Point3& center_, const Point3& lookat_);
        void update_cam(Point3 center_);
        void update_lookat(Point3 lookat_);
};
