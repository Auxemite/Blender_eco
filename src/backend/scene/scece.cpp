#include "scene.hh"
#include "../utils/utils.hh"

Scene::Scene(int width, int height) {
    vector<Sphere> s;
    vector<Point_Light> l;
    s.push_back(Sphere({0, 0, 0}, u::simple(u::white)));
    s.push_back(Sphere({2, 0, 0}, u::plastic(u::magenta)));
    s.push_back(Sphere({-2, 0, 0}, u::metal(u::purple)));
    l.push_back(Point_Light({0, 5, 1}, 10, u::orange));

    Point3 p(0, 5, -5);
    Camera camera_ = Camera(p, {0, 0, 0}, width, height);

    spheres = s;
    lights = l;
    camera = camera_;
}

Scene::Scene(vector<Sphere> sphere_, vector<Point_Light> lights_, Camera camera_) {
    spheres = std::move(sphere_);
    lights = std::move(lights_);
    camera = camera_;
}