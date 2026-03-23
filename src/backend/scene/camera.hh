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
        int width=0;
        int height=0;

        Camera()= default;
        Camera(const Point3& center_, const Point3& lookat_, int width_, int height_);

        void update(const Point3& center_, const Point3& lookat_);
        Vector3 get_dir(int i, int j);
        void update_cam(const Point3& center_);
        void update_lookat(const Point3& lookat_);
};
