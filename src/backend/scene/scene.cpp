#include "scene.hh"
#include "../utils/utils.hh"

Scene::Scene(int width, int height)
{
   
//    s.push_back(Sphere({0, 0, 0}, 1, utils::simple(utils::blue), "LA SPHEREEEU"));
//     s.push_back(Sphere({0, 0, 0}, utils::simple(utils::blue)));
//     s.push_back(Sphere({2, 0, 0}, utils::plastic(utils::magenta)));
//     s.push_back(Sphere({-2, 0, 0}, utils::metal(utils::purple)));
//     l.push_back(Point_Light({0, 5, 1}, 10, utils::orange));


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

    // Triangle *tr1 = new Triangle(a, b, c, Uniform_Texture(basic::texture::simple, basic::color::blue));
    // Triangle *tr2 = new Triangle(d, e, f, Uniform_Texture(basic::texture::simple, basic::color::red));
// 
	// objects.push_back(tr1);
	// objects.push_back(tr2);
// 
	// tr1->scale(2, {0, 0, 0});
	// tr2->scale(1);

    // std::vector<Point3 *> point_vec({&a, &b, &c, &d, &e, &f});
    // std::vector<std::vector<int>> face_vec({{0, 1, 2}, {0, 2, 3}, {0, 4, 5}, {0, 5, 1}});

    // Mesh mesh = Mesh(point_vec,
    //                 face_vec,
    //         Uniform_Texture(basic::texture::basic, basic::color::blue));
    // add_mesh(mesh);

    //char *file = "truc_chelou.obj";
    char *file = "test_cube.obj";
    Mesh *test_triangle = new Mesh(file, Uniform_Texture(basic::texture::simple, basic::color::cyan));
    add_mesh(test_triangle);

    test_triangle->scale(0.5, {1, 1, 1}); 

    // test_triangle->to_dot_obj("be+.obj");
 
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
    // for (auto triangle : mesh.faces)
    //     objects.push_back(triangle);

    meshes.push_back(mesh);
}