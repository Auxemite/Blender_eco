#include "env.hh"

inline double aspect_ratio = 16.0 / 9.0;
inline int default_width = 1280;
inline int default_height = static_cast<int>(default_width / aspect_ratio);

Env::Env() {
    image = Image(default_width, default_height);
    scene = Scene(image.width, image.height);
    update_texture();
}

Env::Env(const char* filename) {
    image = load_image(filename);
    scene = Scene(image.width, image.height);
    update_texture();
}

unsigned char* Env::convertDataToGLRGB(const vector<vector<Color>>& data, int width, int height)
{
    unsigned char* char_data;
    size_t size = width * height * 3;
    char_data = (unsigned char *)calloc(size + 1, sizeof(unsigned char));

    int k = 0;
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            char_data[k++] = static_cast<unsigned char>(data[i][j].r * 255);
            char_data[k++] = static_cast<unsigned char>(data[i][j].g * 255);
            char_data[k++] = static_cast<unsigned char>(data[i][j].b * 255);
        }
    }
    char_data[size] = '\0';
    return char_data;
}

void Env::update_texture() {
    unsigned char* image_data = convertDataToGLRGB(image.data, image.width, image.height);
    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);

    glDeleteTextures(1, &render_image);
    render_image = image_texture;
}

void Env::render() {
    image.render(scene);
    update_texture();
}

void Env::move_camera() {
    Point3 p = scene.camera.center;
    double angle = PI / 8;
    scene.camera.update_cam(Point3(cos(angle) * p.x - sin(angle) * p.z, 0, sin(angle) * p.x + cos(angle) * p.z) + Point3(0, 5, 0));
    render();
}

void Env::move_right() {
    scene.spheres[focus_index].center.x += 0.5;
    render();
}

void Env::move_left() {
    scene.spheres[focus_index].center.x -= 0.5;
    render();
}

void Env::grow() {
    scene.spheres[focus_index].radius += 0.2;
    render();
}

void Env::shrink() {
    scene.spheres[focus_index].radius -= 0.2;
    render();
}

void Env::change_focus() {
    focus_index++;
    if (focus_index >= scene.spheres.size())
        focus_index = 0;
}