#include "scene.hh"
#include "../utils/utils.hh"

Scene::Scene(int width, int height) {
    std::vector<Sphere> s;
    std::vector<Point_Light> l;
    s.push_back(Sphere({0, 0, 0}, basic::texture::simple(basic::color::blue)));
    s.push_back(Sphere({2, 0, 0}, basic::texture::plastic(basic::color::magenta)));
    s.push_back(Sphere({-2, 0, 0}, basic::texture::metal(basic::color::purple)));
    l.push_back(Point_Light({0, 5, 1}, 10, basic::color::orange));

    Point3 p(0, 5, -5);
    Camera camera_ = Camera(p, {0, 0, 0}, width, height);

    spheres = s;
    lights = l;
    camera = camera_;
}

Scene::Scene(std::vector<Sphere> sphere_, std::vector<Point_Light> lights_, Camera camera_) {
    spheres = std::move(sphere_);
    lights = std::move(lights_);
    camera = camera_;
}