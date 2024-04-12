#include "scene.hh"
#include "../utils/utils.hh"

Scene::Scene(int width, int height) {
    std::vector<Point_Light> l;

    Sphere test({0, 0, 0}, {basic::color::dark_blue, basic::texture::metal});

    spheres.push_back(new Sphere({0, 0, 0}, Uniform_Texture(basic::texture::simple, basic::color::blue)));
    spheres.push_back(new Sphere({2, 0, 0}, Uniform_Texture(basic::texture::plastic, basic::color::magenta)));
    spheres.push_back(new Sphere({-2, 0, 0}, Uniform_Texture(basic::texture::metal, basic::color::purple)));
    l.push_back(Point_Light({0, 5, 1}, 10, basic::color::orange));

    Point3 p(0, 5, -5);
    Camera camera_ = Camera(p, {0, 0, 0}, width, height);

    lights = l;
    camera = camera_;
}

Scene::Scene(std::vector<Shape*> sphere_, std::vector<Point_Light> lights_, Camera camera_) {
    spheres = std::move(sphere_);
    lights = std::move(lights_);
    camera = camera_;
}