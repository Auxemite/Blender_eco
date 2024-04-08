#include "camera.hh"

Camera::Camera(Point3 center_, Point3 lookat_, int width_, int height_)
{
    width = width_;
    height = height_;
    auto z_min = 1.0;
    auto screen_height = 1.0;
    auto screen_width = screen_height * (static_cast<double>(width)/height);
    center = center_;
    lookat = lookat_;
    auto lookat_vect = unit_vector(lookat_ - center) * z_min;
    auto up = Vector3(0,1,0);
    auto u = (unit_vector(lookat_vect * up) * -1) * screen_width;
    auto v = (unit_vector(lookat_vect * u)) * -screen_height;

    pixel_u = u / width;
    pixel_v = v / height;

    auto upper_left = center + lookat_vect - u / 2 - v /2;
    pixel_loc = upper_left + (pixel_u + pixel_v) * 0.5;
}

void Camera::update_cam(Point3 center_)
{
    auto z_min = 1.0;
    auto screen_height = 1.0;
    auto screen_width = screen_height * (static_cast<double>(width)/height);
    center = center_;
    auto lookat_vect = unit_vector(lookat - center) * z_min;
    auto up = Vector3(0,1,0);
    auto u = (unit_vector(lookat_vect * up) * -1) * screen_width;
    auto v = (unit_vector(lookat_vect * u)) * -screen_height;

    pixel_u = u / width;
    pixel_v = v / height;

    auto upper_left = center + lookat_vect - u / 2 - v /2;
    pixel_loc = upper_left + (pixel_u + pixel_v) * 0.5;
}

void Camera::update_lookat(Point3 lookat_)
{
    auto z_min = 1.0;
    auto screen_height = 1.0;
    auto screen_width = screen_height * (static_cast<double>(width)/height);
    lookat = lookat_;
    auto lookat_vect = unit_vector(lookat - center) * z_min;
    auto up = Vector3(0,1,0);
    auto u = (unit_vector(lookat_vect * up) * -1) * screen_width;
    auto v = (unit_vector(lookat_vect * u)) * -screen_height;

    pixel_u = u / width;
    pixel_v = v / height;

    auto upper_left = center + lookat_vect - u / 2 - v /2;
    pixel_loc = upper_left + (pixel_u + pixel_v) * 0.5;
}