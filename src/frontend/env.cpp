#include "env.hh"

inline double aspect_ratio = 16.0 / 9.0;
inline int default_width = 1280;
inline int default_height = static_cast<int>(default_width / aspect_ratio);

Env::Env() {
    image = Image(default_width, default_height);
    scene = Scene(image.width, image.height);
    focus_mesh = nullptr;
    if (!scene.meshes.empty())
        focus_mesh = scene.meshes[0];
    render();
    create_texture();
}

Env::Env(const char* filename) {
    image = load_image(filename);
    scene = Scene(image.width, image.height);
    focus_mesh = nullptr;
    if (!scene.meshes.empty())
        focus_mesh = scene.meshes[0];
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
    image.render(scene, photorealist);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width, image.height, GL_RGB, GL_UNSIGNED_BYTE, image.char_data);
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

void Env::move_x(double value) {
    Point3 new_location = *focus_mesh->points[0] + Point3(value, 0, 0);
    focus_mesh->move_mesh(new_location);
    render();
}

void Env::move_y(double value) {
    Point3 new_location = *focus_mesh->points[0] + Point3(0, value, 0);
    focus_mesh->move_mesh(new_location);
    render();
}

void Env::move_z(double value) {
    Point3 new_location = *focus_mesh->points[0] + Point3(0, 0, value);
    focus_mesh->move_mesh(new_location);
    render();
}

void Env::scale(double value) {
    focus_mesh->scale_mesh(value);
    render();
}

void Env::rotate_x(double angle) {
    focus_mesh->rotate_axis_x(angle);
    render();
}

void Env::rotate_y(double angle) {
//    focus_mesh->rotate_axis_y(angle);
    render();
}

void Env::rotate_z(double angle) {
//    focus_mesh->rotate_axis_z(angle);
    render();
}

//void Env::change_focus(int index, Shape *shape) {
//    // TOFIX UPDATE AT EVERY CHANGE
//    if (index >= 0 && index < scene.objects.size()) {
//        focus_index = index;
//        focus_obj = shape->get_obj_data();
//    }
//}

void Env::change_focus(int index, Mesh *mesh) {
    for (auto & face : focus_mesh->faces) {
        face->selected = false;
    }
    if (index >= 0 && index < scene.meshes.size()) {
        focus_index = index;
        focus_mesh = mesh;
    }
    for (auto & face : mesh->faces) {
        face->selected = true;
    }
    render();
}