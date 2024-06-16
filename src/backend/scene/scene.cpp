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

void Scene::add_mesh(const std::string& name) {
    Mesh *mesh = new Mesh("../data/" + name + ".obj", Uniform_Texture(basic::texture::simple, basic::color::cyan));
    add_mesh(mesh);
    std::cout << "Added Mesh\n";
}

void Scene::delete_mesh() {
    if (focus_mesh == nullptr)
        return;
    for (int i = 0; i < meshes.size(); ++i) {
        if (meshes[i] == focus_mesh)
            meshes.erase(meshes.begin()+i);
    }
    focus_mesh = nullptr;
    focus_face = nullptr;
    std::cout << "Deleted Mesh\n";
}

void Scene::move_camera_x(float angle) {
    if (angle == 0)
        return;
    Point3 p = camera.center;
    float angle_d = PI * angle / 180.0;
    float x_ = 0;
    float y_ = sin(angle_d) * p.z + cos(angle_d) * p.y;
    float z_ = cos(angle_d) * p.z - sin(angle_d) * p.y;
    camera.update_cam(Point3(x_, y_, z_) + Point3(p.x, 0, 0));
}

void Scene::move_camera_y(float angle) {
    if (angle == 0)
        return;
    Point3 p = camera.center;
    float angle_d = PI * angle / 180.0;
    float x_ = cos(angle_d) * p.x - sin(angle_d) * p.z;
    float y_ = 0;
    float z_ = sin(angle_d) * p.x + cos(angle_d) * p.z;
    camera.update_cam(Point3(x_, y_, z_) + Point3(0, p.y, 0));
}

void Scene::move_camera_z(float angle) {
    if (angle == 0)
        return;
    Point3 p = camera.center;
    float angle_d = PI * angle / 180.0;
    float x_ = sin(angle_d) * p.y + cos(angle_d) * p.x;
    float y_ = cos(angle_d) * p.y - sin(angle_d) * p.x;
    float z_ = 0;
    camera.update_cam(Point3(x_, y_, z_) + Point3(0, 0, p.z));
}

void Scene::change_material(Color color, Texture texture) {
    Uniform_Texture uni_text = Uniform_Texture(color, texture);
    focus_mesh->texture = uni_text;
    for (auto & face : focus_mesh->faces)
        face->texture = uni_text;
}

void Scene::move_x(float value) {
    if (selected_mode == 0) {
        Point3 new_location = *focus_mesh->points[0] + Point3(value, 0, 0);
        focus_mesh->move_mesh(new_location);
    }
    else if (selected_mode == 1)
        focus_mesh->move_face(focus_face, Point3(value, 0, 0));
}

void Scene::move_y(float value) {
    if (selected_mode == 0) {
        Point3 new_location = *focus_mesh->points[0] + Point3(0, value, 0);
        focus_mesh->move_mesh(new_location);
    }
    else if (selected_mode == 1)
        focus_mesh->move_face(focus_face, Point3(0, value, 0));
}

void Scene::move_z(float value) {
    if (selected_mode == 0) {
        Point3 new_location = *focus_mesh->points[0] + Point3(0, 0, value);
        focus_mesh->move_mesh(new_location);
    }
    else if (selected_mode == 1)
        focus_mesh->move_face(focus_face, Point3(0, 0, value));
}

void Scene::scale(float value) {
    if (selected_mode == 0)
        focus_mesh->scale_mesh(value);
    else if (selected_mode == 1)
        focus_mesh->scale_face(value, focus_face);
}

void Scene::rotate_x(float angle) {
    if (selected_mode == 0)
        focus_mesh->rotate_axis_x(angle);
    else if (selected_mode == 1) {
        std::vector<Point3 *> point_list;
        point_list.push_back(focus_face->a);
        point_list.push_back(focus_face->b);
        point_list.push_back(focus_face->c);
        focus_mesh->rotate_all_axis(angle, 0, 0, point_list);
    }
}

void Scene::rotate_y(float angle) {
    if (selected_mode == 0)
        focus_mesh->rotate_all_axis(0, angle, 0);
    else if (selected_mode == 1) {
        std::vector<Point3 *> point_list;
        point_list.push_back(focus_face->a);
        point_list.push_back(focus_face->b);
        point_list.push_back(focus_face->c);
        focus_mesh->rotate_all_axis(0, angle, 0, point_list);
    }
}

void Scene::rotate_z(float angle) {
    if (selected_mode == 0)
        focus_mesh->rotate_all_axis(0, 0, angle);
    else if (selected_mode == 1) {
        std::vector<Point3 *> point_list;
        point_list.push_back(focus_face->a);
        point_list.push_back(focus_face->b);
        point_list.push_back(focus_face->c);
        focus_mesh->rotate_all_axis(0, 0, angle, point_list);
    }
}


void Scene::extrude(float x_, float y_, float z_) {
    if (selected_mode == 0 || focus_face == nullptr)
        return;
    auto *a_ = new Point3(*focus_face->a + Point3(x_, y_, z_));
    auto *b_ = new Point3(*focus_face->b + Point3(x_, y_, z_));
    auto *c_ = new Point3(*focus_face->c + Point3(x_, y_, z_));
    focus_mesh->extrude_face(focus_face, a_, b_, c_);
}

void Scene::select_mesh(float x, float y) {
    auto c = camera;
    auto pixel_center = c.pixel_loc + (static_cast<float>(x) * c.pixel_u) + (static_cast<float>(y) * c.pixel_v);
    auto dir = (pixel_center - c.center).norm();
    auto inter = Intersection(c.center, dir);
    Mesh *selected_mesh = nullptr;
    Triangle *selected_face = nullptr;
    Point3 *selected_edge = nullptr;
    for (auto mesh : meshes)
    {
        for (auto face : mesh->faces)
        {
            // Check backface culling
            if (dot(dir, face->normal_) < 0) {
                auto inter_scal = face->ray_intersection(c.center, dir);
                if (inter_scal > 0)
                {
                    Point3 new_inter_loc = c.center + dir * inter_scal;
                    if (inter.inter_loc == null_point
                        || (new_inter_loc - camera.center).length() < (inter.inter_loc - c.center).length()) {
                        inter.inter_loc = new_inter_loc;
                        selected_mesh = mesh;
                        selected_face = face;
                    }
                }
            }
        }
    }
    if (inter.inter_loc == null_point || selected_mesh == nullptr)
        return;
    if (selected_mode == 0)
        change_focus(selected_mesh);
    else if (selected_mode == 1)
        change_focus(selected_mesh, selected_face);
}

void Scene::change_focus(Mesh *mesh) {
    if (mesh == nullptr || selected_mode != 0)
        return;

    if (focus_mesh != nullptr) {
        for (auto &face: focus_mesh->faces)
            face->selected = false;
    }
    if (focus_mesh == nullptr || focus_mesh != mesh) {
        focus_mesh = mesh;
        for (auto &face: mesh->faces)
            face->selected = true;
    }
    else
        focus_mesh = nullptr;
}

void Scene::change_focus(Mesh *mesh, Triangle *face) {
    if (mesh == nullptr || selected_mode != 1)
        return;

    if (focus_face != nullptr)
        focus_face->selected = false;
    if (focus_face == nullptr || focus_face != face) {
        focus_face = face;
        focus_face->selected = true;
        focus_mesh = mesh;
    }
    else {
        focus_face = nullptr;
        focus_mesh = nullptr;
    }
}

void Scene::change_focus(Mesh *mesh, Point3 *edge) {
    if (mesh == nullptr || selected_mode != 2)
        return;
}

void Scene::update_selection_mode() {
    if (focus_mesh != nullptr) {
        for (auto &face: focus_mesh->faces)
            face->selected = false;
    }
    if (focus_face != nullptr)
        focus_face->selected = false;
    focus_mesh = nullptr;
    focus_face = nullptr;
    std::cout << "Changed Selection Mode " << selected_mode << "\n";
};
