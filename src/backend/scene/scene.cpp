#include "scene.hh"
#include "../utils/utils.hh"

Scene::Scene(int width, int height)
{
    objects.push_back(new Plane({0,0,0},
                                {0,1,0},
                                Uniform_Texture(basic::texture::basic, basic::color::dark_gray),
                                true));

    objects.push_back(new Sphere(
            {0,0,0}, 0.1,
            Uniform_Texture(basic::texture::simple, basic::color::blue)));

    // Tests
    Point3 a(1, 0, 0), b(0, 1, 0), c(0, 0, 1), d(2, 0, 0),
           e(0, 2, 0), f(0, 0, 2);

    // char *file = "truc_chelou.obj";
    char *file = "test_cube.obj";

    Mesh *cube1 = new Mesh(file, Uniform_Texture(basic::texture::simple, basic::color::cyan));
    Mesh *cube2 = new Mesh(file, Uniform_Texture(basic::texture::simple, basic::color::yellow));

    // add_mesh(cube1);
    add_mesh(cube2);

    cube1->scale_mesh(1);
	// cube2->move_mesh({1, 0, 0});
    cube2->rotate_axis(3.14159 / 4);

    lights.push_back(new Point_Light({0,5,1}, 10,
                                     basic::color::orange));

    camera = Camera(
            {0, 1, -5},
            {0, 0, 0},
            width,
            height);
}

Scene::Scene(std::vector<Shape*> sphere_, std::vector<Light*> lights_, Camera camera_)
{
    objects = std::move(sphere_);
    lights = std::move(lights_);
    camera = camera_;
}

void Scene::add_mesh(Mesh *mesh)
{
    meshes.push_back(mesh);
}