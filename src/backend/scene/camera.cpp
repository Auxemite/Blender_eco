#include "camera.hh"

Camera::Camera(const Point3& center_, const Point3& lookat_, int width_, int height_)
{
    width = width_;
    height = height_;

    update(center_, lookat_);
}

void Camera::update(const Point3& center_, const Point3& lookat_)
{
    if (center_ != 0)
        center = center_;

    if (lookat_ != 0)
        lookat = lookat_;

    float z_min = 1.0f;
    float screen_height = 1.0f;
    float screen_width = screen_height * (static_cast<float>(width)/static_cast<float>(height));
    Vector3 lookat_vect = (lookat - center).norm() * z_min;
    Vector3 up = Vector3(0,1,0);
    Vector3 u = (lookat_vect * up).norm() * -screen_width;
    Vector3 v = (lookat_vect * u).norm() * -screen_height;

    pixel_u = u / static_cast<float>(width);
    pixel_v = v / static_cast<float>(height);

    auto upper_left = center + lookat_vect - u / 2 - v /2;
    pixel_loc = upper_left + (pixel_u + pixel_v) * 0.5;
}

void Camera::update_cam(const Point3& center_)
{
    update(center_, 0);
}        

void Camera::update_lookat(const Point3& lookat_)
{
    // TODO to correct up update problem
    /// Vector3 old_dir = (this->lookat - center).norm();
    /// Vector3 new_dir = (lookat_ - center).norm();
    /// Vector3 diff = new_dir - old_dir;
    /// up += diff;
    /// v += diff;

    update(0, lookat_);
}