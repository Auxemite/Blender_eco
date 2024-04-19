#include "scene.hh"
#include "../utils/utils.hh"

Scene::Scene(int width, int height)
{
    spheres.push_back(new Plane({0, -2, 0}, {0, -1, 0}, Uniform_Texture(basic::texture::metal, basic::color::white)));
    spheres.push_back(new Sphere({0, 0, 0}, Uniform_Texture(basic::texture::simple, basic::color::blue)));
    spheres.push_back(new Sphere({2, 0, 0}, Uniform_Texture(basic::texture::plastic, basic::color::magenta)));
    spheres.push_back(new Sphere({-2, 0, 0}, Uniform_Texture(basic::texture::metal, basic::color::purple)));
    lights.push_back(new Point_Light({0, 5, 1}, 10, basic::color::orange));

    camera = Camera({0, 1, -5}, {0, 0, 0}, width, height);
}

Scene::Scene(std::vector<Shape*> sphere_, std::vector<Light*> lights_, Camera camera_)
{
    spheres = std::move(sphere_);
    lights = std::move(lights_);
    camera = camera_;
}
