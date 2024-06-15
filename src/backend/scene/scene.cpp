#include <utility>

#include "scene.hh"
#include "../render/intersection.hh"

Scene::Scene(int width, int height)
{
    objects.push_back(new Plane({0,0,0},
                                {0,1,0},
                                Uniform_Texture(basic::texture::basic, basic::color::dark_gray),
                                true));

    Mesh *cube = new Mesh("../data/cube.obj", Uniform_Texture(basic::texture::simple, basic::color::red));
    add_mesh(cube);

    lights.push_back(new Point_Light({3,5,3}, 20,
                                     basic::color::orange));

    camera = Camera(
            {0, 5, -5},
            {0, 0, 0},
            width,
            height);
}

Scene::Scene(std::vector<Shape*> sphere_, std::vector<Light*> lights_, Camera camera_)
{
    objects = std::move(sphere_);
    lights = std::move(lights_);
    camera = std::move(camera_);
}

void Scene::add_mesh(Mesh *mesh)
{
    meshes.push_back(mesh);
}

void Scene::select(int x, int y) const
{
    auto pixel_center = camera.pixel_loc + (static_cast<float>(x) * camera.pixel_u) + (static_cast<float>(y) * camera.pixel_v);
    auto dir = (pixel_center - camera.center).norm();
    auto intersection = Intersection(camera.center, dir);
    intersection.throw_ray(*this);

    if (intersection.object != nullptr)
        intersection.object->selected = !intersection.object->selected;
}
