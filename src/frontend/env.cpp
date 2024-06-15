#include <utility>

#include "env.hh"

inline double aspect_ratio = 16.0 / 9.0;
inline int default_width = 1280;
inline int default_height = static_cast<int>(default_width / aspect_ratio);

Env::Env() {
    image = Image(default_width, default_height);
    scene = Scene(image.width, image.height);
    focus_mesh = nullptr;
    render();
    create_texture();
}

Env::Env(const char* filename) {
    image = load_image(filename);
    scene = Scene(image.width, image.height);
    focus_mesh = nullptr;
    render();
    create_texture();
}

void Env::create_texture() {
    unsigned char* image_data = image.char_data;
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);

    glDeleteTextures(1, &render_image);
    render_image = image_texture;
}

void Env::render() {
    image.render(scene, photorealist, fast_selection);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width, image.height, GL_RGB, GL_UNSIGNED_BYTE, image.char_data);
}

void Env::add_mesh(std::string name) {
    Mesh *mesh = new Mesh("../data/" + name + ".obj", Uniform_Texture(basic::texture::simple, basic::color::cyan));
    scene.add_mesh(mesh);
    std::cout << "Added Mesh\n";
    render();
}

void Env::delete_mesh(Mesh *mesh) {
    if (mesh == nullptr)
        return;
    for (int i = 0; i < scene.meshes.size(); ++i) {
        if (scene.meshes[i] == mesh)
            scene.meshes.erase(scene.meshes.begin()+i);
    }
    focus_mesh = nullptr;
    focus_face = nullptr;
    std::cout << "Deleted Mesh\n";
    render();
}

void Env::move_camera_x(double angle) {
    if (angle == 0)
        return;
    Point3 p = scene.camera.center;
    double angle_d = PI * angle / 180.0;
    double x_ = 0;
    double y_ = sin(angle_d) * p.z + cos(angle_d) * p.y;
    double z_ = cos(angle_d) * p.z - sin(angle_d) * p.y;
    scene.camera.update_cam(Point3(x_, y_, z_) + Point3(p.x, 0, 0));
}

void Env::move_camera_y(double angle) {
    if (angle == 0)
        return;
    Point3 p = scene.camera.center;
    double angle_d = PI * angle / 180.0;
    double x_ = cos(angle_d) * p.x - sin(angle_d) * p.z;
    double y_ = 0;
    double z_ = sin(angle_d) * p.x + cos(angle_d) * p.z;
    scene.camera.update_cam(Point3(x_, y_, z_) + Point3(0, p.y, 0));
}

void Env::move_camera_z(double angle) {
    if (angle == 0)
        return;
    Point3 p = scene.camera.center;
    double angle_d = PI * angle / 180.0;
    double x_ = sin(angle_d) * p.y + cos(angle_d) * p.x;
    double y_ = cos(angle_d) * p.y - sin(angle_d) * p.x;
    double z_ = 0;
    scene.camera.update_cam(Point3(x_, y_, z_) + Point3(0, 0, p.z));
}

void Env::change_material(Color color, Texture texture) {
    Uniform_Texture uni_text = Uniform_Texture(color, texture);
    focus_mesh->texture = uni_text;
    for (auto & face : focus_mesh->faces)
        face->texture = uni_text;
    render();
}

void Env::move_x(double value) {
    if (selected_mode == 0) {
        Point3 new_location = *focus_mesh->points[0] + Point3(value, 0, 0);
        focus_mesh->move_mesh(new_location);
    }
    else if (selected_mode == 1)
        focus_mesh->move_face(focus_face, Point3(value, 0, 0));
    render();
}

void Env::move_y(double value) {
    if (selected_mode == 0) {
        Point3 new_location = *focus_mesh->points[0] + Point3(0, value, 0);
        focus_mesh->move_mesh(new_location);
    }
    else if (selected_mode == 1)
        focus_mesh->move_face(focus_face, Point3(0, value, 0));
    render();
}

void Env::move_z(double value) {
    if (selected_mode == 0) {
        Point3 new_location = *focus_mesh->points[0] + Point3(0, 0, value);
        focus_mesh->move_mesh(new_location);
    }
    else if (selected_mode == 1)
        focus_mesh->move_face(focus_face, Point3(0, 0, value));
    render();
}

void Env::scale(double value) {
    if (selected_mode == 0)
        focus_mesh->scale_mesh(value);
    else if (selected_mode == 1)
        focus_mesh->scale_face(value, focus_face);
    render();
}

void Env::rotate_x(double angle) {
    if (selected_mode == 0)
        focus_mesh->rotate_axis_x(angle);
    else if (selected_mode == 1) {
        std::vector<Point3 *> point_list;
        point_list.push_back(focus_face->a);
        point_list.push_back(focus_face->b);
        point_list.push_back(focus_face->c);
        focus_mesh->rotate_all_axis(angle, 0, 0, point_list);
    }
    render();
}

void Env::rotate_y(double angle) {
    if (selected_mode == 0)
        focus_mesh->rotate_all_axis(0, angle, 0);
    else if (selected_mode == 1) {
        std::vector<Point3 *> point_list;
        point_list.push_back(focus_face->a);
        point_list.push_back(focus_face->b);
        point_list.push_back(focus_face->c);
        focus_mesh->rotate_all_axis(0, angle, 0, point_list);
    }
    render();
}

void Env::rotate_z(double angle) {
    if (selected_mode == 0)
        focus_mesh->rotate_all_axis(0, 0, angle);
    else if (selected_mode == 1) {
        std::vector<Point3 *> point_list;
        point_list.push_back(focus_face->a);
        point_list.push_back(focus_face->b);
        point_list.push_back(focus_face->c);
        focus_mesh->rotate_all_axis(0, 0, angle, point_list);
    }
    render();
}


void Env::extrude(double x_, double y_, double z_) {
    if (selected_mode == 0 || focus_face == nullptr)
        return;
    auto *a_ = new Point3(*focus_face->a + Point3(x_, y_, z_));
    auto *b_ = new Point3(*focus_face->b + Point3(x_, y_, z_));
    auto *c_ = new Point3(*focus_face->c + Point3(x_, y_, z_));
    focus_mesh->extrude_face(focus_face, a_, b_, c_);
    render();
}

void Env::select_mesh(int x, int y) {
    auto c = scene.camera;
    auto pixel_center = c.pixel_loc + (x * c.pixel_u) + (y * c.pixel_v);
    auto dir = (pixel_center - c.center).norm();
    auto inter = Intersection(c.center, dir);
    Mesh *selected_mesh = nullptr;
    Triangle *selected_face = nullptr;
    for (auto mesh : scene.meshes)
    {
        for (auto face : mesh->faces)
        {
            // Check backface culling
            if (dot(dir, face->normal_) < 0) {
                auto inter_scal = face->ray_intersection(c.center, dir);
                if (inter_scal > 0)
                {
                    Point3 new_inter_loc = c.center + dir * inter_scal;
                    if (inter.inter_loc == Point3(INT_MAX, INT_MAX, INT_MAX)
                        || (new_inter_loc - scene.camera.center).length() < (inter.inter_loc - c.center).length()) {
                        inter.inter_loc = new_inter_loc;
                        selected_mesh = mesh;
                        selected_face = face;
                    }
                }
            }
        }
    }
    if (inter.inter_loc == Point3(INT_MAX, INT_MAX, INT_MAX) || selected_mesh == nullptr)
        return;
    if (selected_mode == 0)
        change_focus(selected_mesh);
    else if (selected_mode == 1)
        change_focus(selected_mesh, selected_face);
}

void Env::change_focus(Mesh *mesh) {
    if (selected_mode != 0)
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
    render();
}

void Env::change_focus(Mesh *mesh, Triangle *face) {
    if (selected_mode != 1)
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
    render();
}

void Env::update_selection_mode() {
    if (focus_mesh != nullptr) {
        for (auto &face: focus_mesh->faces)
            face->selected = false;
    }
    if (focus_face != nullptr)
        focus_face->selected = false;
    std::cout << "Changed Selection Mode " << selected_mode << "\n";
    render();
};

void Env::save_mesh(std::string filename) {
    if (focus_mesh == nullptr)
        std::cerr << "Save Mesh Error: NO MESH SELECTED\n";
    else
        focus_mesh->to_dot_obj(std::move(filename));
}
