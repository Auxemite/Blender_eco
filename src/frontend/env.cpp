#include "env.hh"

inline double aspect_ratio = 16.0 / 9.0;
inline int default_width = 1280;
inline int default_height = static_cast<int>(default_width / aspect_ratio);

Env::Env() {
    image = Image(default_width, default_height);
    scene = Scene(image.width, image.height);
    focus_obj = nullptr;
    render();
    create_texture();
}

Env::Env(const char* filename) {
    image = load_image(filename);
    scene = Scene(image.width, image.height);
    focus_obj = nullptr;
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
    Point3 p = scene.camera.center;
    double x_ = 0;
    double y_ = sin(angle) * p.z + cos(angle) * p.y;
    double z_ = cos(angle) * p.z - sin(angle) * p.y;
    scene.camera.update_cam(Point3(x_, y_, z_) + Point3(p.x, 0, 0));
    render();
}

void Env::move_camera_y(double angle) {
    Point3 p = scene.camera.center;
    double x_ = cos(angle) * p.x - sin(angle) * p.z;
    double y_ = 0;
    double z_ = sin(angle) * p.x + cos(angle) * p.z;
    scene.camera.update_cam(Point3(x_, y_, z_) + Point3(0, p.y, 0));
    render();
}

void Env::move_camera_z(double angle) {
    Point3 p = scene.camera.center;
    double x_ = sin(angle) * p.y + cos(angle) * p.x;
    double y_ = cos(angle) * p.y - sin(angle) * p.x;
    double z_ = 0;
    scene.camera.update_cam(Point3(x_, y_, z_) + Point3(0, 0, p.z));
    render();
}

void Env::move_x(double value) {
    std::string obj_type = typeid(*scene.objects[focus_index]).name();
    if (obj_type == "class Sphere")
        ((Sphere*) scene.objects[focus_index])->center.x += value;
    else if (obj_type == "class Plane")
        ((Plane*) scene.objects[focus_index])->origin.x += value;
    else if (obj_type == "class Triangle") {
        // ((Triangle *) scene.objects[focus_index])->a.x += value;
        // ((Triangle *) scene.objects[focus_index])->b.x += value;
        // ((Triangle *) scene.objects[focus_index])->c.x += value;
    }
    render();
}

void Env::move_y(double value) {
    std::string obj_type = typeid(*scene.objects[focus_index]).name();
    if (obj_type == "class Sphere")
        ((Sphere*) scene.objects[focus_index])->center.y += value;
    else if (obj_type == "class Plane")
        ((Plane*) scene.objects[focus_index])->origin.y += value;
    else if (obj_type == "class Triangle") {
        // ((Triangle *) scene.objects[focus_index])->a.y += value;
        // ((Triangle *) scene.objects[focus_index])->b.y += value;
        // ((Triangle *) scene.objects[focus_index])->c.y += value;
    }
    render();
}

void Env::move_z(double value) {
    std::string obj_type = typeid(*scene.objects[focus_index]).name();
    if (obj_type == "class Sphere")
        ((Sphere*) scene.objects[focus_index])->center.z += value;
    else if (obj_type == "class Plane")
        ((Plane*) scene.objects[focus_index])->origin.z += value;
    else if (obj_type == "class Triangle") {
        // ((Triangle *) scene.objects[focus_index])->a.z += value;
        // ((Triangle *) scene.objects[focus_index])->b.z += value;
        // ((Triangle *) scene.objects[focus_index])->c.z += value;
    }
    render();
}

void Env::grow(double value) {
    std::string obj_type = typeid(*scene.objects[focus_index]).name();
    if (obj_type == "class Sphere")
        ((Sphere*) scene.objects[focus_index])->radius += value;
//    else if (obj_type == "class Triangle")
//        // TODO
    render();
}

void Env::shrink(double value) {
    std::string obj_type = typeid(*scene.objects[focus_index]).name();
    if (obj_type == "class Sphere")
        ((Sphere*) scene.objects[focus_index])->radius -= value;
//    else if (obj_type == "class Triangle")
//        // TODO
    render();
}

void Env::change_focus(int index, Shape *shape) {
    // TOFIX UPDATE AT EVERY CHANGE
    if (index >= 0 && index < scene.objects.size()) {
        focus_index = index;
        focus_obj = shape->get_obj_data();
    }
}