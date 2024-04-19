#include "scene.hh"
#include "../utils/utils.hh"

Scene::Scene(int width, int height) {
    vector<Sphere> s;
    vector<Point_Light> l;
//    s.push_back(Sphere({0, 0, 0}, 1, utils::simple(utils::blue), "LA SPHEREEEU"));
    s.push_back(Sphere({0, 0, 0}, utils::simple(utils::blue)));
    s.push_back(Sphere({2, 0, 0}, utils::plastic(utils::magenta)));
    s.push_back(Sphere({-2, 0, 0}, utils::metal(utils::purple)));
    l.push_back(Point_Light({0, 5, 1}, 10, utils::orange));

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