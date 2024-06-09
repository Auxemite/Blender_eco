#include "scene.hh"
#include "../utils/utils.hh"
#include "../render/intersection.hh"

Scene::Scene(int width, int height)
{
    objects.push_back(new Plane({0,0,0},
                                {0,1,0},
                                Uniform_Texture(basic::texture::basic, basic::color::dark_gray),
                                true));

//    objects.push_back(new Sphere(
//            {0,0,0}, 1,
//            Uniform_Texture(basic::texture::simple, basic::color::blue)));

    // Tests
    Point3 a(1, 0, 0), b(0, 1, 0), c(0, 0, 1), d(2, 0, 0),
           e(0, 2, 0), f(0, 0, 2);

    // char *file = "truc_chelou.obj";
    char *file = "test_extrude_nom.obj";

    Mesh *cube1 = new Mesh(file, Uniform_Texture(basic::texture::simple, basic::color::cyan));
    Mesh *cube2 = new Mesh(file, Uniform_Texture(basic::texture::simple, basic::color::yellow));

    add_mesh(cube1);

    add_mesh(cube2);
    double pi = 3.14159;
    std::vector<Triangle *> faces(cube2->faces.begin(), cube2->faces.end() - 4.0);
    cube2->extrude_along_points(0.25, faces);
    // cube2->rotate_all_axis(8, 1, 4);

    // faces = std::vector<Triangle *>(cube2->faces.begin() + 4, cube2->faces.end() - 4);
    // for (auto face : faces)
    //     face->selected = true;

    cube2->move_mesh({1, 1, 1});
//     faces = std::vector<Triangle *>(cube2->faces.begin() + 4.0, cube2->faces.end() - 4.0);
//     for (auto face : faces)
//         face->selected = true;

    // cube2->to_dot_obj("test.obj");

    lights.push_back(new Point_Light({0,5,1}, 10,
                                     basic::color::orange));

    camera = Camera(
            {0, 5, -5},
            {0, 0, 0},
            width,
            height);

    for (int i = 0; i < 1280; i++)
            select(i, 720/2);
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

void Scene::select(int x, int y)
{
    auto pixel_center = camera.pixel_loc + (x * camera.pixel_u) + (y * camera.pixel_v);
    auto dir = (pixel_center - camera.center).norm();
    auto intersection = Intersection(camera.center, dir);
    intersection.throw_ray(*this);

    if (intersection.object != nullptr)
        intersection.object->selected = !intersection.object->selected;
}
