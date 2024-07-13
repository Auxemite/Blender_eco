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
    focus_faces.clear();
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

void Scene::zoom_camera(float value) {
    camera.update_cam(camera.center * value);
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
        focus_mesh->move_face(focus_faces, Point3(value, 0, 0));
    else if (selected_mode == 3)
        focus_mesh->translate_point(focus_summit, Point3(value, 0, 0));
}

void Scene::move_y(float value) {
    if (selected_mode == 0) {
        Point3 new_location = *focus_mesh->points[0] + Point3(0, value, 0);
        focus_mesh->move_mesh(new_location);
    }
    else if (selected_mode == 1)
        focus_mesh->move_face(focus_faces, Point3(0, value, 0));
    else if (selected_mode == 3)
        focus_mesh->translate_point(focus_summit, Point3(0, value, 0));
}

void Scene::move_z(float value) {
    if (selected_mode == 0) {
        Point3 new_location = *focus_mesh->points[0] + Point3(0, 0, value);
        focus_mesh->move_mesh(new_location);
    }
    else if (selected_mode == 1)
        focus_mesh->move_face(focus_faces, Point3(0, 0, value));
    else if (selected_mode == 3)
        focus_mesh->translate_point(focus_summit, Point3(0, 0, value));
}

void Scene::scale(float value) {
    if (selected_mode == 0)
        focus_mesh->scale_mesh(value);
    else if (selected_mode == 1)
        focus_mesh->scale_face(value, focus_faces);
}

void Scene::rotate_xyz(float anglex, float angley, float anglez) {
    if (selected_mode == 0)
        focus_mesh->rotate_all_axis(anglex, angley, anglez);
    else if (selected_mode == 1) {
        for (auto face : focus_faces) {
            std::vector<Point3 *> point_list;
            point_list.push_back(face->a);
            point_list.push_back(face->b);
            point_list.push_back(face->c);
            focus_mesh->rotate_all_axis(anglex, angley, anglez, point_list);
        }
    }
}

void Scene::extrude(float x_, float y_, float z_) {
    if (selected_mode == 0 || focus_faces.empty())
        return;
    for (auto face : focus_faces) {
        auto *a_ = new Point3(*face->a + Point3(x_, y_, z_));
        auto *b_ = new Point3(*face->b + Point3(x_, y_, z_));
        auto *c_ = new Point3(*face->c + Point3(x_, y_, z_));
        focus_mesh->extrude_face(face, a_, b_, c_);
    }
}

void Scene::extrude_along_normal(float thickness) {
    if (!focus_faces.empty())
        focus_mesh->extrude_along_normal(thickness, focus_faces);
}

void Scene::extrude_along_points_normalized(float thickness) {
    if (!focus_faces.empty())
        focus_mesh->extrude_along_points(thickness, focus_faces);
}

int Scene::select_mesh(float x, float y) {
    auto c = camera;
    auto pixel_center = c.pixel_loc + (static_cast<float>(x) * c.pixel_u) + (static_cast<float>(y) * c.pixel_v);
    auto dir = (pixel_center - c.center).norm();
    auto inter = Intersection(c.center, dir);
    Mesh *selected_mesh = nullptr;
    int mesh_index = 0;
    Triangle *selected_face = nullptr;
    for (int i = 0; i < meshes.size(); ++i)
    {
        Mesh *mesh = meshes[i];
        if (!mesh->watch)
            continue;

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
                        mesh_index = i;
                        selected_face = face;
                    }
                }
            }
        }
    }
    if (inter.inter_loc == null_point || selected_mesh == nullptr)
        return -1;
    if (selected_mode == 0)
        change_focus(selected_mesh);
    else if (selected_mode == 1)
        change_focus(selected_mesh, selected_face);

    return mesh_index;
}

void Scene::select_summit(float x, float y) {
    auto c = camera;
    auto pixel_center = c.pixel_loc + (static_cast<float>(x) * c.pixel_u) + (static_cast<float>(y) * c.pixel_v);
    auto dir = (pixel_center - c.center).norm();
    Mesh *selected_mesh = nullptr;
    Point3 *selected_summit = nullptr;
    for (auto summit : focus_mesh->points) {
        auto point_dir = (*summit - c.center).norm();
        float diff = 0.999;
        std::cout << "dot " << dot(point_dir, dir) << "\n";
        if (dot(point_dir, dir) > diff && (selected_summit == nullptr
            || (*summit - camera.center).length() < (*selected_summit - c.center).length())) {
            selected_mesh = focus_mesh;
            selected_summit = summit;
            std::cout << "focus summit is " << *summit << " at x = " << x << " and y = " << y << "\n";
        }
    }
    if (selected_mode == 3)
        change_focus(selected_mesh, selected_summit);
}

void Scene::change_focus(Mesh *mesh) {
    if (mesh == nullptr || selected_mode != 0)
        return;
    if (editmode) {
        for (auto &face: focus_mesh->faces)
            face->selected = !face->selected;
        return;
    }

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
    if (mesh != focus_mesh) {
        std::cerr << "Scene Change Focus ERROR : mesh and focus_mesh are different\n";
        return;
    }

    if (!face->selected)
        focus_faces.push_back(face);
    else {
        auto it = find(focus_faces.begin(), focus_faces.end(), face);
        focus_faces.erase(it);
    }
    face->selected = !face->selected;
}

void Scene::change_focus(Mesh *mesh, Point3 *summit) {
    if (mesh == nullptr || selected_mode != 3)
        return;

    focus_summit = summit;
}

void Scene::update_selection_mode() {
    if (focus_mesh != nullptr) {
        for (auto &face: focus_mesh->faces)
            face->selected = false;
    }
    focus_faces.clear();
    std::cout << "Changed Selection Mode " << selected_mode << "\n";
};
